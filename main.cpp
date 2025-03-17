#include <iostream>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <ctime>
#include <string>
#include <zmq.hpp>
#include <json.hpp>

using json = nlohmann::json;
using namespace std;

// ... existing Product struct and other functions remain the same ...

json productsToJson(const vector<Product>& products, bool isPeak) {
    json j;
    j["isPeakHour"] = isPeak;
    j["products"] = json::array();
    
    for (const auto& p : products) {
        json product;
        product["id"] = p.id;
        product["name"] = p.name;
        product["basePrice"] = p.basePrice;
        product["demand"] = p.demand;
        product["stock"] = p.stock;
        product["dynamicPrice"] = p.dynamicPrice;
        j["products"].push_back(product);
    }
    return j;
}

int main() {
    zmq::context_t context(1);
    zmq::socket_t socket(context, ZMQ_REP);
    socket.bind("tcp://*:5555");

    vector<Product> products = {
        {101, "Laptop", 50000, 6, 10},
        {102, "Headphones", 1500, 9, 5},
        {103, "Smartphone", 20000, 8, 3},
        {104, "Backpack", 1200, 3, 15},
        {105, "Smartwatch", 7000, 7, 4}
    };

    cout << "Dynamic Pricing Server Started..." << endl;

    while (true) {
        zmq::message_t request;
        socket.recv(request);
        
        string requestStr(static_cast<char*>(request.data()), request.size());
        json requestJson = json::parse(requestStr);
        
        string action = requestJson["action"];
        json responseJson;

        if (action == "login") {
            string username = requestJson["username"];
            string password = requestJson["password"];
            
            bool loginSuccess = (username == "admin" && password == "1234");
            responseJson["success"] = loginSuccess;
        }
        else if (action == "getProducts") {
            bool peak = isPeakHour();
            for (auto &p : products) {
                p.calculateDynamicPrice(peak);
            }
            responseJson = productsToJson(products, peak);
        }
        else if (action == "updateProduct") {
            int id = requestJson["id"];
            int newDemand = requestJson["demand"];
            int newStock = requestJson["stock"];
            bool peak = isPeakHour();
            
            for (auto &p : products) {
                if (p.id == id) {
                    p.demand = newDemand;
                    p.stock = newStock;
                    p.calculateDynamicPrice(peak);
                    break;
                }
            }
            responseJson["success"] = true;
        }
        else if (action == "sortProducts") {
            mergeSort(products, 0, products.size() - 1);
            responseJson = productsToJson(products, isPeakHour());
        }

        string responseStr = responseJson.dump();
        zmq::message_t reply(responseStr.size());
        memcpy(reply.data(), responseStr.c_str(), responseStr.size());
        socket.send(reply, zmq::send_flags::none);
    }

    return 0;
}
