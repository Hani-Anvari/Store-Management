#ifndef CUSTOMERLIST_H
#define CUSTOMERLIST_H

#include "CustomerData.h"
#include "Customer.h"
#include <map>
#include <string>

class CustomerList {
  private:
    std::map<int, CustomerData> customerList;
    std::string CustomersFile;
  public:
    CustomerList(std::string &CF): CustomersFile(CF) {}
    void load();
    void save();
    void update(CustomerData customer);
    CustomerData find(int &id);
    bool SearchByID(int &id, Customer &customer);
    bool SearchByName(std::string &name, std::string &family, Customer &customer);
    void Display();
};

#endif 