#ifndef ORDER_H
#define ORDER_H

#include <string>

struct Order {
  int ID, quantity, price;
  std::string date;
};

#endif