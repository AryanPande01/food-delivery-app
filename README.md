# 🍔 FoodMate — Console-Based Food Ordering System (C++ OOP)

FoodMate is a fully object-oriented, console-based food ordering and delivery simulation built using modern C++.  
The project models a real-world food delivery platform (like Zomato / Swiggy) and demonstrates strong use of OOP principles, data structures, and system-level design.

---

## 🚀 Features

### 👤 Multi-User System
FoodMate supports three distinct user roles:
- **Customer** – Browse restaurants, order food, apply offers, pay, track orders, chat, and rate
- **Restaurant Owner** – Manage restaurants and menus
- **Delivery Partner** – Handle deliveries, ratings, and earnings

---

### 🍽️ Restaurant & Menu Management
- Multiple restaurants with unique cuisines
- Rich menu system with:
  - Cuisine filtering (Indian, Italian, Chinese, etc.)
  - Course filtering (Lunch, Dinner, Dessert, etc.)
  - Veg / Non-Veg classification
- Dynamic dish ratings based on customer feedback

---

### 🛒 Cart & Order Processing
- Add / remove dishes from cart
- Automatic subtotal calculation
- Order lifecycle tracking:
  - Pending → Preparing → Out for Delivery → Delivered
- Delivery partner auto-assignment

---

### 🎁 Offers & Loyalty System
- Promo codes with:
  - Flat discounts
  - Percentage discounts
  - Minimum order constraints
- Loyalty points awarded after successful orders
- Loyalty-based exclusive offers

---

### 💳 Payment Simulation
- UPI payment simulation (random success/failure)
- Cash on Delivery (COD)
- Payment abstraction using polymorphism

---

### 💬 In-App Chat & Notifications
- Order-based chat system between customer, delivery partner, and system
- Auto-generated system messages based on order status
- Notification service for order updates

---

### ⭐ Ratings & Feedback
- Rate food and delivery separately
- Restaurant and dish ratings update dynamically
- Delivery partner rating and earnings tracking
- Textual feedback support

---

## 🧠 OOP Concepts Demonstrated

- **Abstraction** (User, Payment interfaces)
- **Inheritance** (Customer, RestaurantOwner, DeliveryPartner)
- **Polymorphism** (dynamic_cast, virtual functions)
- **Encapsulation** (private data with public getters)
- **Composition** (Restaurant → Menu → Dish)
- **STL Usage** (vector, map, string, algorithms)
- **Memory Management** (dynamic allocation & cleanup)

---

## 🛠️ Tech Stack

- Language: **C++ (C++11+)**
- Concepts: **OOP, STL, System Design**
- Interface: **Console-based**

---

## ▶️ How to Run

1. Clone the repository
2. Compile using any modern C++ compiler:


---

## 📌 Sample Workflow

1. Login/Register as Customer
2. Browse restaurants and apply filters
3. Add dishes to cart
4. Apply promo codes
5. Make payment
6. Track order & chat
7. Rate food and delivery

---

## 📈 Why This Project?

This project was built to:
- Simulate a real-world system using pure C++
- Practice large-scale OOP design
- Strengthen system thinking beyond simple DSA problems

---

## 👨‍💻 Author

Built with ❤️ by **Aryan**  
A hands-on C++ & Full Stack Developer exploring system design through real-world simulations.
