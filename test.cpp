#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <iomanip>

// Structure to represent a customer order
struct Order {
    std::string order_id;
    std::string customer_name;
    double amount;
    std::string status;
};

// Function to write an order to a CSV file
bool writeOrderToFile(const std::string& file_name, const Order& order) {
    std::ofstream file(file_name, std::ios_base::app); // Append mode
    if (!file.is_open()) {
        std::cerr << "Error opening file" << std::endl;
        return false;
    }
    file << order.order_id << "," << order.customer_name << "," << order.amount << "," << order.status << "\n";
    file.close();
    return true;
}

// Function to display all orders
void displayOrders(const std::vector<Order>& orders) {
    std::cout << std::setw(12) << "Order ID" << std::setw(20) << "Customer Name"
              << std::setw(10) << "Amount" << std::setw(10) << "Status" << std::endl;
    std::cout << std::string(55, '-') << std::endl;
    for (const auto& order : orders) {
        std::cout << std::setw(12) << order.order_id
                  << std::setw(20) << order.customer_name
                  << std::setw(10) << std::fixed << std::setprecision(2) << order.amount
                  << std::setw(10) << order.status << std::endl;
    }
}

// Function to read orders from the CSV file
std::vector<Order> readOrdersFromFile(const std::string& file_name) {
    std::ifstream file(file_name);
    std::vector<Order> orders;
    if (!file.is_open()) {
        std::cerr << "Error opening file for reading" << std::endl;
        return orders;
    }

    std::string line;
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string order_id, customer_name, amount_str, status;
        if (std::getline(ss, order_id, ',') && std::getline(ss, customer_name, ',') &&
            std::getline(ss, amount_str, ',') && std::getline(ss, status, ',')) {
            orders.push_back({order_id, customer_name, std::stod(amount_str), status});
        }
    }
    file.close();
    return orders;
}

// Function to update the status of an order
bool updateOrderStatus(const std::string& file_name, const std::string& target_order_id, const std::string& new_status) {
    std::vector<Order> orders = readOrdersFromFile(file_name);
    bool orderFound = false;

    for (auto& order : orders) {
        if (order.order_id == target_order_id) {
            order.status = new_status;
            orderFound = true;
        }
    }

    // Rewrite the file with updated orders
    if (orderFound) {
        std::ofstream outfile(file_name);
        for (const auto& order : orders) {
            outfile << order.order_id << "," << order.customer_name << "," << order.amount << "," << order.status << "\n";
        }
    }
    return orderFound;
}

int main() {
    std::string file_name = "customer_orders.csv";
    
    // Adding new customer orders
    writeOrderToFile(file_name, {"001", "John Doe", 250.50, "Pending"});
    writeOrderToFile(file_name, {"002", "Alice Johnson", 100.75, "Pending"});
    writeOrderToFile(file_name, {"003", "Bob Smith", 300.00, "Paid"});
    
    // Display orders
    std::vector<Order> orders = readOrdersFromFile(file_name);
    displayOrders(orders);
    
    // Update order status
    std::cout << "\nUpdating order status for Order ID 001..." << std::endl;
    updateOrderStatus(file_name, "001", "Paid");
    
    // Display updated orders
    orders = readOrdersFromFile(file_name);
    displayOrders(orders);

    return 0;
}
