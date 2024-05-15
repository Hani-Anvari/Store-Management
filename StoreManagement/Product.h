#ifndef PRODUCT_H
#define PRODUCT_H
#include <iostream>


class Product {
  private:
    static int counter;
    int ID, quantity, price;
    std::string name;
  public:
    Product(){}
    Product(std::string &n, int &p, int &q, bool NewProduct = true): name(n), price(p), quantity(q) {if(NewProduct){ID = counter++;}}

    //set functions
    void set_name(std::string &name);
    void set_quantity(int &quantity);
    void set_price(int &price);
    void set_ID(int &ID);
    //get functions
    std::string get_name() const;
    int get_quantity() const;
    int get_price() const;
    int get_ID() const;
    //Inventory related functions
    bool sell(int &num);
    void restock(int &num);
    //counter save&load
    static void save_counter();
    static void load_counter();
};

#endif