#ifndef INVENTORY_H
#define INVENTORY_H

#include "Product.h"
#include <map>
#include <iostream>

class Inventory {
  private:
    std::map<int, Product> inventory;
    const std::string InventoryFile;
  public:
    Inventory(std::string &IF): InventoryFile(IF) {}
    void load();
    void save();
    void update(Product &product);
    Product findProduct(int &id);
    bool SearchByID(int &id, Product &product);
    bool SearchByName(std::string &name, Product &product);
    void Display();
};

#endif