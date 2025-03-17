import React, { useState, useEffect } from 'react';
import './ProductList.css';

const ProductList = () => {
    const [products, setProducts] = useState([]);
    const [loading, setLoading] = useState(true);
    const [error, setError] = useState(null);

    useEffect(() => {
        const fetchProducts = async () => {
            try {
                const response = await fetch('http://localhost:5000/api/products');
                if (!response.ok) {
                    throw new Error('Failed to fetch products');
                }
                const data = await response.json();
                setProducts(data);
                setLoading(false);
            } catch (err) {
                setError(err.message);
                setLoading(false);
            }
        };

        fetchProducts();
        const interval = setInterval(fetchProducts, 60000); // Refresh every minute

        return () => clearInterval(interval);
    }, []);

    if (loading) return <div className="loading">Loading...</div>;
    if (error) return <div className="error">{error}</div>;

    return (
        <div className="product-container">
            <h2>Dynamic Pricing Dashboard</h2>
            <div className="product-grid">
                {products.map(product => (
                    <div key={product.id} className="product-card">
                        <h3>{product.name}</h3>
                        <div className="price-info">
                            <p>Base Price: ₹{product.basePrice}</p>
                            <p className="dynamic-price">
                                Dynamic Price: ₹{product.dynamicPrice}
                            </p>
                        </div>
                        <div className="product-stats">
                            <p>Demand: {product.demand}/10</p>
                            <p>Stock: {product.stock}</p>
                        </div>
                    </div>
                ))}
            </div>
        </div>
    );
};

export default ProductList;
