#ifndef CUSTOMER_H
#define CUSTOMER_H

#include "Product.h"
#include "CustomerData.h"
#include "Order.h"
#include "Inventory.h"
#include <iostream>
#include <vector>

class Customer {
  private:
    CustomerData cData;
    std::vector<Order> cart;
    std::string cartName;
    static int counter;
  public:
    Customer() {}
    Customer(std::string &n, std::string &f) {
      cData.name = n;
      cData.family = f;
      cData.total = 0;
      cData.ID = counter++;
      cartName = "Cart" + std::to_string(cData.ID) + ".txt";
    }

    //set functions
    void set_name(std::string &name);
    void set_family(std::string &family);
    void set_total(int &total);
    void set_ID(int &ID);
    void set_cartName();

    //get functions
    int get_ID() const;
    std::string get_name() const;
    std::string get_family() const;
    int get_total() const;
    CustomerData get_cData() const;

    bool buy(Product &p, Order &order);
    //void pay();
    void displayCart(Inventory &inventory);

    void loadCart();
    void saveCart();

    static void save_counter();
    static void load_counter();
};

#endif