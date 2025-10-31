#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <iomanip>
#include <map>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <memory>
#include <limits> // Include for numeric_limits
using namespace std;
// --- 1. ENUMS AND CONSTANTS ---
// -------------------------------------------------------------

enum class CuisineType { INDIAN, ITALIAN, CHINESE, MEXICAN, JAPANESE, OTHER };
enum class CourseType { BREAKFAST, BRUNCH, LUNCH, SNACKS, DINNER, DESSERT, ANY };
enum class DishType { VEG, NON_VEG, BOTH };
enum class PaymentMode { UPI, CREDIT_CARD, COD, NONE };
enum class OrderStatus { PENDING, PREPARING, OUT_FOR_DELIVERY, DELIVERED, CANCELLED };

// Helper functions for string conversion
string cuisineToString(CuisineType c) {
    switch (c) {
        case CuisineType::INDIAN: return "Indian";
        case CuisineType::ITALIAN: return "Italian";
        case CuisineType::CHINESE: return "Chinese";
        case CuisineType::MEXICAN: return "Mexican";
        case CuisineType::JAPANESE: return "Japanese";
        default: return "Other";
    }
}
string courseToString(CourseType c) {
    switch (c) {
        case CourseType::BREAKFAST: return "Breakfast";
        case CourseType::BRUNCH: return "Brunch";
        case CourseType::LUNCH: return "Lunch";
        case CourseType::SNACKS: return "Snacks";
        case CourseType::DINNER: return "Dinner";
        case CourseType::DESSERT: return "Dessert";
        default: return "Any";
    }
}
string dishTypeToString(DishType t) {
    return (t == DishType::VEG ? "Veg" : (t == DishType::NON_VEG ? "Non-Veg" : "Both"));
}
string statusToString(OrderStatus s) {
    switch (s) {
        case OrderStatus::PENDING: return "Pending";
        case OrderStatus::PREPARING: return "Preparing";
        case OrderStatus::OUT_FOR_DELIVERY: return "Out for Delivery";
        case OrderStatus::DELIVERED: return "Delivered";
        case OrderStatus::CANCELLED: return "Cancelled";
        default: return "Unknown";
    }
}


// --- 2. FORWARD DECLARATIONS ---
// -------------------------------------------------------------
class User;
class Customer;
class RestaurantOwner;
class DeliveryPartner;
class Dish;
class Menu;
class Restaurant;
class Offer;
class Cart;
class Order;
class Payment;
class SystemManager;
class Rating;

// --- 3. UTILITY CLASSES ---
// -------------------------------------------------------------
class IDGenerator {
private:
    static long long userCounter;
    static long long restaurantCounter;
    static long long orderCounter;
public:
    static string generateUserID() { return "U" + to_string(++userCounter); }
    static string generateRestaurantID() { return "R" + to_string(++restaurantCounter); }
    static string generateOrderID() { return "O" + to_string(++orderCounter); }
};
long long IDGenerator::userCounter = 1000;
long long IDGenerator::restaurantCounter = 500;
long long IDGenerator::orderCounter = 100;

class Notification {
public:
    void sendNotification(const string& userId, const string& message) const {
        cout << "\n[Notification to " << userId << "]: " << message << endl;
    }
};

// --- 4. DISH AND MENU ---
// -------------------------------------------------------------
class Dish {
private:
    string dishId;
    string name;
    double price;
    DishType type;
    CuisineType cuisine;
    CourseType course;
    double rating;
    int ratingCount;
public:
    Dish(const string& n, double p, DishType t, CuisineType c, CourseType cs)
        : dishId("D" + to_string(rand() % 1000 + 100)), name(n), price(p), type(t), cuisine(c), course(cs), rating(0.0), ratingCount(0) {}

    const string& getName() const { return name; }
    const string& getId() const { return dishId; }
    double getPrice() const { return price; }
    DishType getType() const { return type; }
    CuisineType getCuisine() const { return cuisine; }
    CourseType getCourse() const { return course; }
    double getRating() const { return rating; }

    void updateRating(int newRating) {
        rating = (rating * ratingCount + newRating) / (ratingCount + 1);
        ratingCount++;
    }

    void display() const {
        cout << fixed << setprecision(2)
                  << "    - [" << dishId << "] " << name
                  << " (" << dishTypeToString(type) << ")"
                  << " | Price: $" << price
                  << " | Rating: " << (ratingCount > 0 ? to_string(rating).substr(0, 3) : "N/A")
                  << endl;
    }
};

bool operator<(const Dish& a, const Dish& b) {
    return a.getName() < b.getName();
}

class Menu {
private:
    vector<Dish> dishes;
public:
    void addDish(const Dish& dish) {
        dishes.push_back(dish);
        // Removed repetitive cout from here, it's already in the owner flow
    }

    void removeDish(const string& dishName) {
        dishes.erase(remove_if(dishes.begin(), dishes.end(),
                                     [&](const Dish& d){ return d.getName() == dishName; }),
                                     dishes.end());
    }

    vector<Dish> filterDishes(CuisineType c, CourseType cs, DishType t) const {
        vector<Dish> result;
        for (const auto& dish : dishes) {
            bool cuisineMatch = (c == CuisineType::OTHER) || (dish.getCuisine() == c);
            bool courseMatch = (cs == CourseType::ANY) || (dish.getCourse() == cs);
            bool typeMatch = (t == DishType::BOTH) || (dish.getType() == t);

            if (cuisineMatch && courseMatch && typeMatch) {
                result.push_back(dish);
            }
        }
        return result;
    }
    
    Dish* getDishByName(const string& name) {
        auto it = find_if(dishes.begin(), dishes.end(),
                                [&](const Dish& d){ return d.getName() == name; });
        return (it != dishes.end() ? &(*it) : nullptr);
    }
    
    const vector<Dish>& getAllDishes() const { return dishes; }
};

// --- 5. RESTAURANT ---
// -------------------------------------------------------------
class Restaurant {
private:
    string restaurantId;
    string name;
    CuisineType cuisine;
    double rating;
    int ratingCount;
    vector<string> branches;
    string contactEmail;
    Menu menu;
public:
    Restaurant(const string& n, CuisineType c, const string& email)
        : restaurantId(IDGenerator::generateRestaurantID()), name(n), cuisine(c), rating(4.5), ratingCount(1), contactEmail(email) {
        branches.push_back("Main Street Branch");
    }

    const string& getId() const { return restaurantId; }
    const string& getName() const { return name; }
    CuisineType getCuisine() const { return cuisine; }
    double getRating() const { return rating; }
    Menu& getMenu() { return menu; }

    void displayInfo() const {
        cout << fixed << setprecision(1)
                  << "[" << restaurantId << "] " << name << " (" << cuisineToString(cuisine) << ")"
                  << " | Rating: " << rating << "⭐"
                  << " | Branches: " << branches.size()
                  << endl;
    }

    void updateRating(int newRating) {
        rating = (rating * ratingCount + newRating) / (ratingCount + 1);
        ratingCount++;
    }
};

// --- 6. USER HIERARCHY (ABSTRACTION, INHERITANCE, POLYMORPHISM) ---
// -------------------------------------------------------------
class User {
protected:
    string userId;
    string name;
    string password;
    bool loggedIn;
public:
    User(const string& n, const string& p)
        : userId(IDGenerator::generateUserID()), name(n), password(p), loggedIn(false) {}
    virtual ~User() = default;

    virtual bool login(const string& id, const string& pass) = 0;
    virtual void viewProfile() const = 0;
    virtual bool registerUser() = 0;

    void logout() {
        if (loggedIn) {
            cout << "\n" << name << " (" << userId << ") logged out successfully." << endl;
            loggedIn = false;
        }
    }

    const string& getId() const { return userId; }
    const string& getName() const { return name; }
    bool isLoggedIn() const { return loggedIn; }
};

class Customer : public User {
private:
    string deliveryAddress;
    vector<Order*> orderHistory;
    double loyaltyPoints;
public:
    Customer(const string& n, const string& p, const string& addr)
        : User(n, p), deliveryAddress(addr), loyaltyPoints(0.0) {}

    bool registerUser() override {
        cout << "\n✅ Customer " << name << " registered successfully with ID: " << userId << endl;
        return true;
    }

    bool login(const string& id, const string& pass) override {
        if (userId == id && password == pass) {
            loggedIn = true;
            cout << "\n👋 Welcome back, Customer " << name << "!" << endl;
            return true;
        }
        return false;
    }

    void viewProfile() const override {
        cout << "\n--- Customer Profile ---" << endl;
        cout << "ID: " << userId << endl;
        cout << "Name: " << name << endl;
        cout << "Address: " << deliveryAddress << endl;
        cout << "Loyalty Points: " << loyaltyPoints << endl;
        cout << "Past Orders: " << orderHistory.size() << endl;
    }

    void addOrderToHistory(Order* order) {
        orderHistory.push_back(order);
    }
    
    const string& getAddress() const { return deliveryAddress; }
    double getLoyaltyPoints() const { return loyaltyPoints; }
    void addLoyaltyPoints(double points) { loyaltyPoints += points; }
};

class RestaurantOwner : public User {
private:
    vector<Restaurant*> ownedRestaurants;
public:
    RestaurantOwner(const string& n, const string& p)
        : User(n, p) {}

    bool registerUser() override {
        cout << "\n✅ Restaurant Owner " << name << " registered successfully with ID: " << userId << endl;
        return true;
    }

    bool login(const string& id, const string& pass) override {
        if (userId == id && password == pass) {
            loggedIn = true;
            cout << "\n📋 Welcome to your dashboard, Owner " << name << "!" << endl;
            return true;
        }
        return false;
    }

    void viewProfile() const override {
        cout << "\n--- Restaurant Owner Profile ---" << endl;
        cout << "ID: " << userId << endl;
        cout << "Name: " << name << endl;
        cout << "Owned Restaurants: " << ownedRestaurants.size() << endl;
        for (const auto& r : ownedRestaurants) {
            cout << "  - " << r->getName() << " (" << r->getId() << ")" << endl;
        }
    }

    void addRestaurant(Restaurant* r) {
        ownedRestaurants.push_back(r);
        cout << "Restaurant '" << r->getName() << "' added to your portfolio." << endl;
    }

    const vector<Restaurant*>& getOwnedRestaurants() const { return ownedRestaurants; }
};

class DeliveryPartner : public User {
private:
    string vehicleType;
    double totalEarnings;
    double averageRating;
    int ratingCount;
    bool isAvailable;
public:
    DeliveryPartner(const string& n, const string& p, const string& vehicle)
        : User(n, p), vehicleType(vehicle), totalEarnings(0.0), averageRating(5.0), ratingCount(1), isAvailable(true) {}

    bool registerUser() override {
        cout << "\n✅ Delivery Partner " << name << " registered successfully with ID: " << userId << endl;
        return true;
    }

    bool login(const string& id, const string& pass) override {
        if (userId == id && password == pass) {
            loggedIn = true;
            cout << "\n🏍️ Ready to deliver, Partner " << name << "!" << endl;
            return true;
        }
        return false;
    }

    void viewProfile() const override {
        cout << "\n--- Delivery Partner Profile ---" << endl;
        cout << "ID: " << userId << endl;
        cout << "Name: " << name << endl;
        cout << "Vehicle: " << vehicleType << endl;
        cout << "Earnings: $" << fixed << setprecision(2) << totalEarnings << endl;
        cout << "Rating: " << fixed << setprecision(1) << averageRating << "⭐" << endl;
        cout << "Status: " << (isAvailable ? "Available" : "On Delivery") << endl;
    }

    void completeDelivery(double earnings, int rating) {
        totalEarnings += earnings;
        averageRating = (averageRating * ratingCount + rating) / (ratingCount + 1);
        ratingCount++;
        isAvailable = true;
    }
    
    void startDelivery() { isAvailable = false; }
    bool isCurrentlyAvailable() const { return isAvailable; }
};

// --- 7. OFFERS, CART, ORDER ---
// -------------------------------------------------------------
class Offer {
private:
    string promoCode;
    double discountValue;
    bool isPercentage;
    int minOrderValue;
public:
    Offer(const string& code, double value, bool isP, int minVal)
        : promoCode(code), discountValue(value), isPercentage(isP), minOrderValue(minVal) {}

    const string& getCode() const { return promoCode; }

    double applyDiscount(double subtotal, const Customer* cust) const {
        if (subtotal < minOrderValue) {
            cout << "    [Offer Failed] Minimum order value of $" << minOrderValue << " not met." << endl;
            return 0.0;
        }
        if (promoCode == "LOYALTY50" && cust && cust->getLoyaltyPoints() < 10.0) {
            cout << "    [Offer Failed] Not enough loyalty points." << endl;
            return 0.0;
        }

        if (isPercentage) {
            double discount = subtotal * (discountValue / 100.0);
            cout << "    [Offer Applied] " << discountValue << "% off: -$" << fixed << setprecision(2) << discount << endl;
            return discount;
        } else {
            cout << "    [Offer Applied] $" << discountValue << " off: -$" << fixed << setprecision(2) << discountValue << endl;
            return discountValue;
        }
    }
};

class Cart {
private:
    map<Dish, int> items;
public:
    void addItem(const Dish& dish, int quantity = 1) {
        items[dish] += quantity;
        cout << quantity << "x " << dish.getName() << " added to cart." << endl;
    }

    void removeItem(const string& dishName) {
        for (auto it = items.begin(); it != items.end(); ++it) {
            if (it->first.getName() == dishName) {
                items.erase(it);
                cout << dishName << " removed from cart." << endl;
                return;
            }
        }
    }

    double calculateSubtotal() const {
        double total = 0.0;
        for (const auto& pair : items) {
            total += pair.first.getPrice() * pair.second;
        }
        return total;
    }

    void displayCart() const {
        cout << "\n--- Your Cart ---" << endl;
        if (items.empty()) {
            cout << "Cart is empty." << endl;
            return;
        }
        for (const auto& pair : items) {
            cout << fixed << setprecision(2)
                      << pair.second << "x " << pair.first.getName()
                      << " @ $" << pair.first.getPrice()
                      << " = $" << (pair.first.getPrice() * pair.second)
                      << endl;
        }
        cout << "Subtotal: $" << calculateSubtotal() << endl;
        cout << "-----------------" << endl;
    }

    const map<Dish, int>& getItems() const { return items; }
    bool isEmpty() const { return items.empty(); }
};

class Order {
private:
    string orderID;
    string customerId;
    string restaurantId;
    string partnerId;
    Cart orderCart;
    string deliveryAddress;
    OrderStatus status;
    double subtotal;
    double discountApplied;
    double deliveryTip;
    double finalAmount;
public:
    Order(const Customer* c, const Restaurant* r, const Cart& cart)
        : orderID(IDGenerator::generateOrderID()), customerId(c->getId()), restaurantId(r->getId()),
          orderCart(cart), deliveryAddress(c->getAddress()), status(OrderStatus::PENDING),
          subtotal(cart.calculateSubtotal()), discountApplied(0.0), deliveryTip(0.0), finalAmount(subtotal) {}

    const string& getId() const { return orderID; }
    const string& getCustomerId() const { return customerId; }
    const string& getRestaurantId() const { return restaurantId; }
    OrderStatus getStatus() const { return status; }
    double getFinalAmount() const { return finalAmount; }
    const string& getPartnerId() const { return partnerId; }

    void applyOffer(const Offer& offer, const Customer* cust) {
        discountApplied = offer.applyDiscount(subtotal, cust);
        finalAmount = subtotal - discountApplied;
    }

    void addTip(double tip) {
        deliveryTip = tip;
        finalAmount += tip;
    }

    void setStatus(OrderStatus newStatus) {
        status = newStatus;
    }

    void assignPartner(const string& pId) {
        partnerId = pId;
    }

    void displayDetails() const {
        cout << "\n===================================" << endl;
        cout << "          ORDER SUMMARY" << endl;
        cout << "===================================" << endl;
        cout << "Order ID: " << orderID << endl;
        cout << "Status: " << statusToString(status) << endl;
        cout << "Delivery To: " << deliveryAddress << endl;
        cout << "Subtotal: $" << fixed << setprecision(2) << subtotal << endl;
        cout << "Discount: -$" << fixed << setprecision(2) << discountApplied << endl;
        cout << "Tip: $" << fixed << setprecision(2) << deliveryTip << endl;
        cout << "-----------------------------------" << endl;
        cout << "TOTAL: $" << fixed << setprecision(2) << finalAmount << endl;
        cout << "===================================" << endl;
    }

    const map<Dish, int>& getDishes() const { return orderCart.getItems(); }
    double getTip() const { return deliveryTip; }
};

// --- 8. PAYMENT & CHAT (ABSTRACTION/SIMULATION) ---
// -------------------------------------------------------------
class Payment {
public:
    virtual bool processPayment(double amount) const = 0;
    virtual string getMode() const = 0;
    virtual ~Payment() = default;
};

class UPIPayment : public Payment {
public:
    bool processPayment(double amount) const override {
        cout << "Processing UPI Payment of $" << fixed << setprecision(2) << amount << "..." << endl;
        return (rand() % 100 < 90);
    }
    string getMode() const override { return "UPI"; }
};

class COD : public Payment {
public:
    bool processPayment(double amount) const override {
        cout << "Cash on Delivery confirmed. Please keep $" << fixed << setprecision(2) << amount << " ready." << endl;
        return true;
    }
    string getMode() const override { return "Cash On Delivery (COD)"; }
};

class Chat {
private:
    string orderId;
    vector<pair<string, string>> messages;
public:
    Chat(const string& oId) : orderId(oId) {}

    void sendMessage(const string& sender, const string& text) {
        messages.push_back({sender, text});
        cout << "[" << sender << "]: " << text << endl;
    }

    void autoGenerateMessage(OrderStatus status) {
        string autoMessage;
        switch (status) {
            case OrderStatus::PREPARING:
                autoMessage = "Your order is being prepared by the restaurant!";
                break;
            case OrderStatus::OUT_FOR_DELIVERY:
                autoMessage = "Your food is out for delivery and should reach you shortly!";
                break;
            case OrderStatus::DELIVERED:
                autoMessage = "Enjoy your meal! Please don't forget to rate.";
                break;
            default:
                return;
        }
        sendMessage("System Bot", autoMessage);
    }

    void displayHistory() const {
        cout << "\n--- Chat History for " << orderId << " ---" << endl;
        for (const auto& msg : messages) {
            cout << "[" << msg.first << "]: " << msg.second << endl;
        }
    }
};

// --- 9. RATING ---
// -------------------------------------------------------------
class SystemManager;

class Rating {
public:
    void apply(Order* order, SystemManager& manager, int foodStars, int deliveryStars, const string& feedback);
};


// --- 10. SYSTEM MANAGER (GLOBAL DATA AND LOGIC) ---
// -------------------------------------------------------------
class SystemManager {
private:
    vector<User*> allUsers;
    vector<Restaurant*> allRestaurants;
    vector<Order*> activeOrders;
    vector<Offer> availableOffers;
    Notification notifier;

    void seedData() {
        allUsers.push_back(new Customer("Alice", "pass", "101 Maple St"));
        allUsers.push_back(new RestaurantOwner("ChefBob", "pass"));
        allUsers.push_back(new DeliveryPartner("Dan", "pass", "Bike"));
        
        Restaurant* r1 = new Restaurant("Spice Garden", CuisineType::INDIAN, "spice@mail.com");
        r1->getMenu().addDish({"Paneer Butter Masala", 12.50, DishType::VEG, CuisineType::INDIAN, CourseType::DINNER});
        r1->getMenu().addDish({"Veg Biryani", 10.00, DishType::VEG, CuisineType::INDIAN, CourseType::LUNCH});
        r1->getMenu().addDish({"Chicken Tikka", 15.00, DishType::NON_VEG, CuisineType::INDIAN, CourseType::DINNER});
        
        Restaurant* r2 = new Restaurant("Pizza Hub", CuisineType::ITALIAN, "pizza@mail.com");
        r2->getMenu().addDish({"Margherita Pizza", 18.00, DishType::VEG, CuisineType::ITALIAN, CourseType::DINNER});
        r2->getMenu().addDish({"Pepperoni Pizza", 20.00, DishType::NON_VEG, CuisineType::ITALIAN, CourseType::DINNER});

        allRestaurants.push_back(r1);
        allRestaurants.push_back(r2);

        dynamic_cast<RestaurantOwner*>(allUsers[1])->addRestaurant(r1);
        dynamic_cast<RestaurantOwner*>(allUsers[1])->addRestaurant(r2);

        availableOffers.push_back({"FIRST30", 30.0, false, 50});
        availableOffers.push_back({"LOYALTY50", 50.0, true, 20});
    }

public:
    SystemManager() {
        srand(time(0));
        seedData();
        cout << "FoodMate System Initialized." << endl;
    }

    ~SystemManager() {
        for (User* u : allUsers) delete u;
        for (Restaurant* r : allRestaurants) delete r;
        for (Order* o : activeOrders) delete o;
    }

    User* findUser(const string& id) {
        auto it = find_if(allUsers.begin(), allUsers.end(), 
                                [&](const User* u){ return u->getId() == id; });
        return (it != allUsers.end() ? *it : nullptr);
    }

    Restaurant* findRestaurant(const string& id) {
        auto it = find_if(allRestaurants.begin(), allRestaurants.end(), 
                                [&](const Restaurant* r){ return r->getId() == id; });
        return (it != allRestaurants.end() ? *it : nullptr);
    }
    
    // Public Accessors
    const vector<Restaurant*>& getRestaurants() const { return allRestaurants; }
    const vector<Offer>& getOffers() const { return availableOffers; }
    const vector<User*>& getUsers() const { return allUsers; }
    
    // User Management
    void addUser(User* u) { allUsers.push_back(u); }
    
    // FIX FOR LINE 959 (Part 1): Add a dedicated public method to add a restaurant.
    void addRestaurant(Restaurant* r) {
        allRestaurants.push_back(r);
    }

    // Order Management
    void placeOrder(Order* order) {
        activeOrders.push_back(order);
        notifier.sendNotification(order->getCustomerId(), "Order " + order->getId() + " received! Status: " + statusToString(order->getStatus()));
        
        DeliveryPartner* partner = nullptr;
        for (User* u : allUsers) {
            if (DeliveryPartner* dp = dynamic_cast<DeliveryPartner*>(u); dp && dp->isCurrentlyAvailable()) {
                partner = dp;
                break;
            }
        }
        if (partner) {
            order->assignPartner(partner->getId());
            partner->startDelivery();
            notifier.sendNotification(order->getCustomerId(), "Partner " + partner->getName() + " assigned!");
        }
    }

    void updateOrderStatus(const string& orderId, OrderStatus newStatus) {
        auto it = find_if(activeOrders.begin(), activeOrders.end(), 
                                [&](const Order* o){ return o->getId() == orderId; });
        if (it != activeOrders.end()) {
            (*it)->setStatus(newStatus);
            notifier.sendNotification((*it)->getCustomerId(), "Order " + orderId + " status updated to: " + statusToString(newStatus));
            
            if (newStatus == OrderStatus::DELIVERED) {
                Order* completedOrder = *it;
                Customer* cust = dynamic_cast<Customer*>(findUser(completedOrder->getCustomerId()));
                if (cust) {
                    cust->addOrderToHistory(completedOrder);
                    cust->addLoyaltyPoints(completedOrder->getFinalAmount() * 0.05);
                }
            }
        }
    }
    
    void finalizeOrder(const string& orderId) {
        auto it = find_if(activeOrders.begin(), activeOrders.end(), 
                                [&](const Order* o){ return o->getId() == orderId; });
        if (it != activeOrders.end()) {
             activeOrders.erase(it);
        }
    }
};

void Rating::apply(Order* order, SystemManager& manager, int foodStars, int deliveryStars, const string& feedback) {
    Restaurant* restaurant = manager.findRestaurant(order->getRestaurantId());
    if (restaurant) {
        restaurant->updateRating(foodStars);
        
        for (const auto& pair : order->getDishes()) {
            Dish* dish = restaurant->getMenu().getDishByName(pair.first.getName());
            if (dish) {
                dish->updateRating(foodStars);
            }
        }
    }

    // FIX FOR LINE 706: Ensure the public getter getPartnerId() is used, as partnerId is private.
    // This line is now correct and will compile.
    if (!order->getPartnerId().empty()) {
        DeliveryPartner* partner = dynamic_cast<DeliveryPartner*>(manager.findUser(order->getPartnerId()));
        if (partner) {
            partner->completeDelivery(order->getTip(), deliveryStars);
        }
    }

    cout << "\n⭐ Thank you for your feedback! Food Rated: " << foodStars 
              << ", Delivery Rated: " << deliveryStars << "." << endl;
    if (!feedback.empty()) {
        cout << "Your textual feedback: \"" << feedback << "\" has been recorded." << endl;
    }
    
    manager.finalizeOrder(order->getId());
}


// --- 11. MAIN APPLICATION FLOW ---
// -------------------------------------------------------------
void runCustomerFlow(Customer* customer, SystemManager& manager);
void runOwnerFlow(RestaurantOwner* owner, SystemManager& manager);
void runPartnerFlow(DeliveryPartner* partner, SystemManager& manager);
User* handleLoginOrRegister(SystemManager& manager, char userType);

void runCustomerFlow(Customer* customer, SystemManager& manager) {
    if (!customer) return;

    Restaurant* selectedRestaurant = nullptr;
    CuisineType cuisineFilter = CuisineType::OTHER;
    CourseType courseFilter = CourseType::ANY;
    DishType typeFilter = DishType::BOTH;

    cout << "\n### Welcome " << customer->getName() << "! Start Ordering ###" << endl;

    cout << "\n--- Select Restaurant ---" << endl;
    for (const auto& r : manager.getRestaurants()) {
        r->displayInfo();
    }
    string restId;
    cout << "Enter Restaurant ID (e.g., R501): ";
    cin >> restId;
    selectedRestaurant = manager.findRestaurant(restId);
    if (!selectedRestaurant) {
        cout << "Invalid Restaurant ID." << endl;
        return;
    }

    cout << "\n--- Apply Filters (Optional) ---" << endl;
    
    // *** ADD VALIDATION ***
    cout << "Cuisine (1:Indian, 2:Italian, 3:Chinese, 4:Mexican, 5:Japanese, 0:Any): ";
    int c;
    while (!(cin >> c) || c < 0 || c > 5) { // 5 is max enum val, 0 is 'Any'
        cout << "Invalid input. Please enter a number (0-5): ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    if (c > 0 && c <= 5) cuisineFilter = static_cast<CuisineType>(c-1);
    
    // *** ADD VALIDATION ***
    cout << "Course (1:Lunch, 2:Dinner, 0:Any): ";
    int cs;
    while (!(cin >> cs) || (cs != 0 && cs != 1 && cs != 2)) {
        cout << "Invalid input. Please enter 0, 1, or 2: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    if (cs == 1) courseFilter = CourseType::LUNCH;
    if (cs == 2) courseFilter = CourseType::DINNER;
    
    // *** ADD VALIDATION ***
    cout << "Type (1:Veg, 2:Non-Veg, 0:Both): ";
    int t;
    while (!(cin >> t) || t < 0 || t > 2) {
        cout << "Invalid input. Please enter 0, 1, or 2: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    if (t == 1) typeFilter = DishType::VEG;
    if (t == 2) typeFilter = DishType::NON_VEG;

    Cart customerCart;
    vector<Dish> availableDishes = selectedRestaurant->getMenu().filterDishes(cuisineFilter, courseFilter, typeFilter);
    
    cout << "\n--- Available Dishes at " << selectedRestaurant->getName() << " ---" << endl;
    if (availableDishes.empty()) {
        cout << "No dishes match your filters." << endl;
        return;
    }
    for (const auto& dish : availableDishes) {
        dish.display();
    }
    
    string dishInput;
    // *** FIX 2: Change prompt to ask for ID ***
    cout << "Enter dish ID to add (e.g., D257) (or 'DONE'): ";
    cin.ignore(); // Clear the \n from the last cin >> t;
    
    getline(cin, dishInput);
    // *** DISH NOT FOUND FIX 1 ***
    // Remove trailing carriage return '\r' if it exists (common on Windows)
    if (!dishInput.empty() && dishInput.back() == '\r') {
        dishInput.pop_back();
    }

    while (dishInput != "DONE") {
        // *** FIX 3: Change find_if to use getId() ***
        auto it = find_if(availableDishes.begin(), availableDishes.end(), 
                                [&](const Dish& d){ return d.getId() == dishInput; });
        if (it != availableDishes.end()) {
            customerCart.addItem(*it);
        } else {
            cout << "Dish not found." << endl;
        }
        // *** FIX 4: Change prompt to ask for ID ***
        cout << "Enter dish ID to add (e.g., D257) (or 'DONE'): ";
        
        getline(cin, dishInput);
        // *** DISH NOT FOUND FIX 2 ***
        // Also apply the fix inside the loop
        if (!dishInput.empty() && dishInput.back() == '\r') {
            dishInput.pop_back();
        }
    }

    if (customerCart.isEmpty()) {
        cout << "Order cancelled." << endl;
        return;
    }

    Order* newOrder = new Order(customer, selectedRestaurant, customerCart);
    cout << "\n--- Offers ---" << endl;
    for (const auto& offer : manager.getOffers()) {
        cout << "- Code: " << offer.getCode() << endl;
    }
    string promo;
    cout << "Enter promo code (or 'NONE'): ";
    cin >> promo;
    if (promo != "NONE") {
        auto it = find_if(manager.getOffers().begin(), manager.getOffers().end(), 
                                [&](const Offer& o){ return o.getCode() == promo; });
        if (it != manager.getOffers().end()) {
            newOrder->applyOffer(*it, customer);
        } else {
            cout << "Invalid promo code." << endl;
        }
    }

    newOrder->displayDetails();
    cout << "\n--- Payment ---" << endl;
    cout << "1. UPI\n2. COD\nSelect payment mode: ";
    
    // *** ADD VALIDATION ***
    int paymentChoice;
    while (!(cin >> paymentChoice) || (paymentChoice != 1 && paymentChoice != 2)) {
        cout << "Invalid choice. Please enter 1 for UPI or 2 for COD: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    Payment* paymentMethod = nullptr;
    if (paymentChoice == 1) paymentMethod = new UPIPayment();
    else if (paymentChoice == 2) paymentMethod = new COD();
    else { cout << "Invalid payment mode." << endl; delete newOrder; return; } // This line is now redundant but safe

    if (paymentMethod->processPayment(newOrder->getFinalAmount())) {
        cout << "✅ Payment successful via " << paymentMethod->getMode() << "!" << endl;
        manager.placeOrder(newOrder);
    } else {
        cout << "❌ Payment failed. Order cancelled." << endl;
        delete newOrder;
        return;
    }
    delete paymentMethod;

    manager.updateOrderStatus(newOrder->getId(), OrderStatus::PREPARING);
    Chat chat(newOrder->getId());
    chat.autoGenerateMessage(OrderStatus::PREPARING);

    cout << "\n[Simulating Delivery Process...]" << endl;
    manager.updateOrderStatus(newOrder->getId(), OrderStatus::OUT_FOR_DELIVERY);
    chat.autoGenerateMessage(OrderStatus::OUT_FOR_DELIVERY);

    chat.sendMessage(customer->getName(), "Hi, please come to gate 3.");
    chat.sendMessage("DeliveryPartner", "Sure, on the way, arriving in 5 mins!");
    chat.displayHistory();

    manager.updateOrderStatus(newOrder->getId(), OrderStatus::DELIVERED);
    chat.autoGenerateMessage(OrderStatus::DELIVERED);

    // *** ADD VALIDATION ***
    int tip;
    cout << "\n--- Tip Delivery Partner ---" << endl;
    cout << "Tip (e.g., 5, 10, 20): $";
    while (!(cin >> tip) || tip < 0) {
        cout << "Invalid amount. Please enter a positive number (or 0): $";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    newOrder->addTip(tip);
    cout << "Tip of $" << tip << " added to final bill." << endl;

    // *** ADD VALIDATION ***
    int foodRating, deliveryRating;
    string feedback;
    cout << "\n--- Rate Your Experience (1-5 Stars) ---" << endl;
    cout << "Food Rating: ";
    while (!(cin >> foodRating) || foodRating < 1 || foodRating > 5) {
        cout << "Invalid rating. Please enter a number between 1 and 5: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    cout << "Delivery Rating: ";
    while (!(cin >> deliveryRating) || deliveryRating < 1 || deliveryRating > 5) {
        cout << "Invalid rating. Please enter a number between 1 and 5: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    cout << "Write feedback (one line): ";
    cin.ignore();
    getline(cin, feedback);

    // *** FIX 5: Also add carriage return fix for feedback string ***
    if (!feedback.empty() && feedback.back() == '\r') {
        feedback.pop_back();
    }

    Rating().apply(newOrder, manager, foodRating, deliveryRating, feedback);

    cout << "\nThank you for ordering from FoodMate! Have a great day!" << endl;
}

void runOwnerFlow(RestaurantOwner* owner, SystemManager& manager) {
    if (!owner) return;
    
    cout << "\n### Restaurant Owner Dashboard ###" << endl;
    owner->viewProfile();
    
    if (owner->getOwnedRestaurants().empty()) {
        cout << "\nNo restaurants owned. Adding a sample restaurant for you." << endl;
        Restaurant* newRest = new Restaurant("New Cafe", CuisineType::OTHER, owner->getName() + "@mail.com");
        // FIX FOR LINE 959 (Part 2): Use the new public method to add the restaurant.
        manager.addRestaurant(newRest);
        owner->addRestaurant(newRest);
    }

    Restaurant* myRest = owner->getOwnedRestaurants()[0];

    cout << "\nManaging Menu for: " << myRest->getName() << endl;
    
    // *** ADD VALIDATION ***
    cout << "1. Add Dish\n2. View Menu\n3. Back\nSelect option: ";
    int choice;
    while (!(cin >> choice) || choice < 1 || choice > 3) {
        cout << "Invalid choice. Please enter 1, 2, or 3: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    
    // *** ADD VALIDATION AND FIX ENUM LOGIC ***
    if (choice == 1) {
        string n; double p; int t_in, c_in, cs_in;
        cout << "Dish Name: "; cin.ignore(); getline(cin, n);
        // *** FIX 6: Also add carriage return fix for this string ***
        if (!n.empty() && n.back() == '\r') {
            n.pop_back();
        }
        
        cout << "Price: $";
        while (!(cin >> p) || p <= 0) {
            cout << "Invalid input. Please enter a valid price (e.g., 12.50): $";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }

        cout << "Type (1:Veg, 2:Non-Veg): ";
        while (!(cin >> t_in) || (t_in != 1 && t_in != 2)) {
            cout << "Invalid input. Please enter 1 for Veg or 2 for Non-Veg: ";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
        DishType dishT = (t_in == 1) ? DishType::VEG : DishType::NON_VEG;


        cout << "Cuisine (0:Indian, 1:Italian, 2:Chinese, 3:Mexican, 4:Japanese, 5:Other): ";
        while (!(cin >> c_in) || c_in < 0 || c_in > 5) { // Fixed range 0-5
            cout << "Invalid input. Please enter a number between 0 and 5: ";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
        CuisineType dishC = static_cast<CuisineType>(c_in);

        cout << "Course (0:Breakfast, 1:Brunch, 2:Lunch, 3:Snacks, 4:Dinner, 5:Dessert, 6:Any): ";
        while (!(cin >> cs_in) || cs_in < 0 || cs_in > 6) { // Fixed range 0-6
            cout << "Invalid input. Please enter a number between 0 and 6: ";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
        CourseType dishCS = static_cast<CourseType>(cs_in);
        
        myRest->getMenu().addDish({n, p, dishT, dishC, dishCS});
        cout << "Dish '" << n << "' added to the menu." << endl; // Added feedback here
    } else if (choice == 2) {
        cout << "\n--- Current Menu ---" << endl;
        for (const auto& dish : myRest->getMenu().getAllDishes()) {
            dish.display();
        }
    }
}

void runPartnerFlow(DeliveryPartner* partner, SystemManager& manager) {
    if (!partner) return;
    
    cout << "\n### Delivery Partner Dashboard ###" << endl;
    partner->viewProfile();
    cout << "\nNo new delivery assignments in the current simulation." << endl;
}

User* handleLoginOrRegister(SystemManager& manager, char userType) {
    string id, pass, name, extra, address;
    User* currentUser = nullptr;

    // *** ADD VALIDATION ***
    cout << "\n---\n1. Login\n2. Register\nSelect Option: ";
    int choice;
    while (!(cin >> choice) || (choice != 1 && choice != 2)) {
        cout << "Invalid choice. Please enter 1 or 2: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    if (choice == 2) {
        cout << "Enter Name: "; cin >> name;
        cout << "Enter Password: "; cin >> pass;

        if (userType == 'a') {
            cout << "Enter Delivery Address: "; cin.ignore(); getline(cin, address);
            // *** FIX 7: Also add carriage return fix for this string ***
            if (!address.empty() && address.back() == '\r') {
                address.pop_back();
            }
            currentUser = new Customer(name, pass, address);
        } else if (userType == 'b') {
            currentUser = new RestaurantOwner(name, pass);
        } else if (userType == 'c') {
            cout << "Enter Vehicle Type (Bike/Car): "; cin >> extra;
            currentUser = new DeliveryPartner(name, pass, extra);
        }
        
        if (currentUser && currentUser->registerUser()) {
            manager.addUser(currentUser);
            currentUser->login(currentUser->getId(), pass);
            return currentUser;
        }
        return nullptr;

    } else if (choice == 1) {
        cout << "Enter User ID: "; cin >> id;
        cout << "Enter Password: "; cin >> pass;
        
        User* tempUser = manager.findUser(id);
        if (tempUser && tempUser->login(id, pass)) {
            currentUser = tempUser;
            if ((userType == 'a' && dynamic_cast<Customer*>(currentUser)) ||
                (userType == 'b' && dynamic_cast<RestaurantOwner*>(currentUser)) ||
                (userType == 'c' && dynamic_cast<DeliveryPartner*>(currentUser))) {
                return currentUser;
            }
            cout << "Login failed: User type mismatch or ID not found." << endl;
            currentUser->logout();
            return nullptr;
        }
        cout << "Login failed: Invalid ID or Password." << endl;
        return nullptr;
    }
    return nullptr;
}


int main() {
    SystemManager manager;
    char userTypeChoice;
    User* loggedInUser = nullptr;

    cout << "\n=======================================" << endl;
    cout << "   ✨ Welcome to FoodMate! (C++ OOP)" << endl;
    cout << "=======================================" << endl;

    // *** FIX: Add a main application loop ***
    bool appRunning = true;
    while (appRunning) {
        loggedInUser = nullptr; // Reset loggedInUser at the start of each loop

        cout << "\n--- Main Menu ---" << endl;
        cout << "1. Login as:\n   a) Customer\n   b) Restaurant Owner\n   c) Delivery Partner\n   q) Quit Application\nSelect User Type (a/b/c/q): ";
        cin >> userTypeChoice;
        
        // Clear buffer
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); 

        if (userTypeChoice == 'q') {
            appRunning = false;
            cout << "\nThank you for using FoodMate. Goodbye!" << endl;
            continue; // Skip to next loop iteration, which will exit
        }

        if (userTypeChoice == 'a' || userTypeChoice == 'b' || userTypeChoice == 'c') {
            loggedInUser = handleLoginOrRegister(manager, userTypeChoice);
        } else {
            cout << "Invalid choice. Please select 'a', 'b', 'c', or 'q'." << endl;
            continue; // Go back to the start of the while(appRunning) loop
        }

        // If login was successful, run the appropriate flow
        if (loggedInUser) {
            if (Customer* cust = dynamic_cast<Customer*>(loggedInUser)) {
                runCustomerFlow(cust, manager);
            } else if (RestaurantOwner* owner = dynamic_cast<RestaurantOwner*>(loggedInUser)) {
                runOwnerFlow(owner, manager);
            } else if (DeliveryPartner* partner = dynamic_cast<DeliveryPartner*>(loggedInUser)) {
                runPartnerFlow(partner, manager);
            }
            
            // Log out the user after their flow is complete
            loggedInUser->logout();
            loggedInUser = nullptr; // Explicitly set to null
        }
        // The loop will now repeat, showing the main menu again
    }
    
    return 0;
}
