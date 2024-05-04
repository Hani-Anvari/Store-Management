#include "CustomerList.h"
#include "Customer.h"
#include "CustomerData.h"
#include <fstream>

    
void CustomerList::load() {
    std::ifstream file(CustomersFile);
    if(file.is_open()) {
        CustomerData customer;
        while(file >> customer.ID >> customer.name >> customer.family >> customer.total) {customerList[customer.ID] = customer;}
        file.close();
    }
}

void CustomerList::save() {
    std::ofstream file(CustomersFile);
    if(file.is_open()) {
        CustomerData customer;
        for(const auto& pair: customerList) {
            file << pair.second.ID << ' ' << pair.second.name << ' ' << pair.second.family << ' ' << pair.second.total << std::endl;
        }
        file.close();
    }
}

void CustomerList::update(CustomerData customer) {customerList[customer.ID] = customer;}

CustomerData CustomerList::find(int& id) {return customerList[id];}

bool CustomerList::SearchByID(int &id, Customer &customer) {
    CustomerData customerdata = customerList[id];
    if(customerdata.ID >= 1) {
        customer.set_ID(customerdata.ID); customer.set_name(customerdata.name); customer.set_family(customerdata.family); customer.set_total(customerdata.total); customer.set_cartName();
        return true;
    }
    else {return false;}
}

bool CustomerList::SearchByName(std::string &name, std::string &family, Customer &customer) {
    for(const auto& pair : customerList)
        if(name == pair.second.name && family == pair.second.family) {
            CustomerData customerdata = pair.second;
            customer.set_ID(customerdata.ID); customer.set_name(customerdata.name); customer.set_family(customerdata.family); customer.set_total(customerdata.total); customer.set_cartName();
        return true;
        }
    return false;
}

void CustomerList::Display() {
    std::cout << "ID\tName\tFamily\tTotal\n";
    for(const auto& pair: customerList) {
      std::cout << pair.second.ID << '\t' << pair.second.name << '\t' << pair.second.family << '\t' << pair.second.total << '\n'; 
    }
}