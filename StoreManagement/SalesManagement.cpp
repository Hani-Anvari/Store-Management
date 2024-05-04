#include "SalesManagement.h"
#include "Order.h"
#include <fstream>


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