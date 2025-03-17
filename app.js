import React, { useState, useEffect } from 'react';
import Login from './components/Login';
import ProductList from './components/ProductList';
import './App.css';

function App() {
  const [isLoggedIn, setIsLoggedIn] = useState(false);
  const [products, setProducts] = useState([]);
  const [loading, setLoading] = useState(false);
  const [error, setError] = useState(null);

  const handleLogin = async (credentials) => {
    try {
      setLoading(true);
      const response = await fetch('http://localhost:5000/api/login', {
        method: 'POST',
        headers: {
          'Content-Type': 'application/json',
        },
        body: JSON.stringify(credentials),
      });
      
      const data = await response.json();
      if (data.success) {
        setIsLoggedIn(true);
        fetchProducts();
      } else {
        setError('Invalid credentials');
      }
    } catch (err) {
      setError('Server connection failed');
    } finally {
      setLoading(false);
    }
  };

  const fetchProducts = async () => {
    try {
      const response = await fetch('http://localhost:5000/api/products');
      const data = await response.json();
      setProducts(data.products);
    } catch (err) {
      setError('Failed to fetch products');
    }
  };

  const handleUpdateProduct = async (id, demand, stock) => {
    try {
      const response = await fetch('http://localhost:5000/api/products/update', {
        method: 'POST',
        headers: {
          'Content-Type': 'application/json',
        },
        body: JSON.stringify({ id, demand, stock }),
      });
      
      if (response.ok) {
        fetchProducts(); // Refresh products after update
      }
    } catch (err) {
      setError('Failed to update product');
    }
  };

  const handleSortProducts = async () => {
    try {
      const response = await fetch('http://localhost:5000/api/products/sort', {
        method: 'POST',
        headers: {
          'Content-Type': 'application/json',
        },
      });
      
      const data = await response.json();
      setProducts(data.products);
    } catch (err) {
      setError('Failed to sort products');
    }
  };

  // Auto-refresh products every minute to update peak hour pricing
  useEffect(() => {
    if (isLoggedIn) {
      const interval = setInterval(fetchProducts, 60000);
      return () => clearInterval(interval);
    }
  }, [isLoggedIn]);

  if (loading) {
    return <div className="loading">Loading...</div>;
  }

  return (
    <div className="App">
      {!isLoggedIn ? (
        <Login onLogin={handleLogin} error={error} />
      ) : (
        <ProductList 
          products={products}
          onUpdateProduct={handleUpdateProduct}
          onSortProducts={handleSortProducts}
          error={error}
        />
      )}
    </div>
  );
}

export default App;
