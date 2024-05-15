#ifndef SALESMANAGEMENT_H
#define SALESMANAGEMENT_H

#include "Order.h"
#include "Inventory.h"
#include <string>
#include <unordered_map>

class SalesManagement {
  private:
    std::unordered_map<std::string, Order> SalesData;
    std::string fileName;
  public:
   SalesManagement(const std::string &file): fileName(file) {}

    bool load();
    bool add(Order order);
    
    double turnover(std::string &startDate, std::string &endDate);
    void display(Inventory &inventory);
};

#endif