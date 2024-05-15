#include "SalesManagement.h"
#include "Order.h"
#include <fstream>
#include <iomanip>


bool SalesManagement::load() {
    std::ifstream file(fileName);
    if(file.is_open()) {
        Order order;
        while(file >> order.date >> order.ID >> order.quantity >> order.price) {SalesData[order.date] = order;}
        file.close();
        return true;
    }
    else {return false;}
}

bool SalesManagement::add(Order order) {
    std::ofstream file(fileName, std::ios::app);
    if(file.is_open()) {
        file << order.date << ' ' <<order.ID << ' ' << order.quantity << ' ' << order.price << '\n';
        file.close();
        SalesData[order.date] = order;
        return true;
    }
    else {return false;}
    }

double SalesManagement::turnover(std::string &startDate, std::string &endDate) {
    double turnover = 0.0;
    for(const auto &pair: SalesData) {
        if(pair.first >= startDate && pair.first <= endDate)
          turnover += pair.second.quantity * pair.second.price;
    }
    return turnover;
}

void SalesManagement::display(Inventory &inventory) {
    std::cout << std::left << std::setw(15) << "Date" << std::setw(15) <<  "Name" << std::setw(10) << "Price" << std::setw(10) << "Quantity" << std::setw(10) << "Total" << std::endl;
    for(const auto& pair: SalesData) {
      int id = pair.second.ID;
      Product product = inventory.findProduct(id);
      std::cout << std::left <<std::setw(15) << pair.second.date << std::setw(15) << product.get_name() << std::setw(10) << pair.second.price << std::setw(10) << pair.second.quantity << std::setw(10) << pair.second.price * pair.second.quantity << '\n';
    }
}