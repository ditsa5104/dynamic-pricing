#include <iostream>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <ctime>
#include <string>

using namespace std;

struct Product {
    int id;
    string name;
    double basePrice;
    int demand;
    int stock;
    double dynamicPrice;

    void calculateDynamicPrice(bool peakHours) {
        double demandFactor = 1 + (0.05 * demand);
        double stockFactor = 1 + ((10.0 - min(stock, 10)) * 0.03);
        double timeFactor = peakHours ? 1.10 : 1.00; // 10% increase during peak hours

        dynamicPrice = basePrice * demandFactor * stockFactor * timeFactor;
    }
};

// Merge sort for sorting by dynamic price
void merge(vector<Product> &products, int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;

    vector<Product> L(n1), R(n2);

    for (int i = 0; i < n1; ++i)
        L[i] = products[left + i];
    for (int i = 0; i < n2; ++i)
        R[i] = products[mid + 1 + i];

    int i = 0, j = 0, k = left;

    while (i < n1 && j < n2) {
        if (L[i].dynamicPrice <= R[j].dynamicPrice)
            products[k++] = L[i++];
        else
            products[k++] = R[j++];
    }

    while (i < n1)
        products[k++] = L[i++];
    while (j < n2)
        products[k++] = R[j++];
}

void mergeSort(vector<Product> &products, int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        mergeSort(products, left, mid);
        mergeSort(products, mid + 1, right);
        merge(products, left, mid, right);
    }
}

bool isPeakHour() {
    time_t now = time(0);
    tm *ltm = localtime(&now);
    int hour = ltm->tm_hour;
    return (hour >= 18 && hour <= 22); // 6PM to 10PM
}

void displayProducts(const vector<Product> &products) {
    cout << "\nID\tProduct\t\tBase Price\tDynamic Price\tDemand\tStock\n";
    cout << "-------------------------------------------------------------------\n";
    for (const auto &p : products) {
        cout << p.id << "\t" << setw(10) << left << p.name
             << "\t₹" << fixed << setprecision(2) << p.basePrice
             << "\t₹" << p.dynamicPrice
             << "\t" << p.demand << "\t" << p.stock << "\n";
    }
}

void updateProduct(vector<Product> &products, int id, bool peak) {
    for (auto &p : products) {
        if (p.id == id) {
            cout << "\nUpdating product: " << p.name << "\n";
            cout << "Enter new demand (1-10): ";
            cin >> p.demand;
            cout << "Enter new stock quantity: ";
            cin >> p.stock;
            p.calculateDynamicPrice(peak);
            return;
        }
    }
    cout << "Product ID not found.\n";
}

bool adminLogin() {
    string username, password;
    cout << "\n=== Admin Login ===\n";
    cout << "Username: ";
    cin >> username;
    cout << "Password: ";
    cin >> password;

    if (username == "admin" && password == "1234") {
        cout << "Login successful!\n";
        return true;
    } else {
        cout << "Incorrect credentials!\n";
        return false;
    }
}

int main() {
    vector<Product> products = {
        {101, "Laptop", 50000, 6, 10},
        {102, "Headphones", 1500, 9, 5},
        {103, "Smartphone", 20000, 8, 3},
        {104, "Backpack", 1200, 3, 15},
        {105, "Smartwatch", 7000, 7, 4}
    };

    bool peak = isPeakHour();
    for (auto &p : products)
        p.calculateDynamicPrice(peak);

    if (!adminLogin()) return 0;

    int choice;
    do {
        cout << "\n=== Dynamic Pricing System ===";
        cout << "\n1. Display Products with Dynamic Pricing";
        cout << "\n2. Update Product Demand/Stock";
        cout << "\n3. Sort Products by Dynamic Price (Low → High)";
        cout << "\n4. Exit\nEnter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                displayProducts(products);
                break;
            case 2: {
                int id;
                cout << "Enter Product ID to update: ";
                cin >> id;
                updateProduct(products, id, peak);
                break;
            }
            case 3:
                mergeSort(products, 0, products.size() - 1);
                cout << "Products sorted by dynamic price.\n";
                break;
            case 4:
                cout << "Exiting system. Thank you!\n";
                break;
            default:
                cout << "Invalid choice. Try again.\n";
        }
    } while (choice != 4);

    return 0;
}