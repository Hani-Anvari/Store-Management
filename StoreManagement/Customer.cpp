#include "Customer.h"
#include "Product.h"
#include "CustomerData.h"
#include "Order.h"
#include <fstream>
#include <string>


int Customer::counter = 1;

void Customer::set_name(std::string &name) {cData.name = name;}
void Customer::set_family(std::string &family) {cData.family = family;}
void Customer::set_total(int &total) {cData.total = total;}
void Customer::set_ID(int &ID) {cData.ID = ID;}
void Customer::set_cartName() {cartName = "Cart" + std::to_string(cData.ID) + ".txt";}

int Customer::get_ID() const {return cData.ID;}
std::string Customer::get_name() const {return cData.name;}
std::string Customer::get_family() const {return cData.family;}
int Customer::get_total() const {return cData.total;}
CustomerData Customer::get_cData() const {return cData;}

bool Customer::buy(Product &p, Order &order) {
  if(p.sell(order.quantity)) {
    cData.total += p.get_price() * order.quantity;
    cart.push_back(order);
    return true;
  }
  else{return false;}
}
//void Cusomer::pay() {total = 0;}

void Customer::loadCart() {
  std::ifstream file(cartName);
  if(file.is_open()) {
    Order order;
    while(file >> order.date >> order.ID >> order.quantity >> order.price)
      cart.push_back(order);
    file.close();
  }
  else {std::cout << "Failed to load the cart.\n";}
}
void Customer::saveCart() {
  std::ofstream file(cartName);
  if(file.is_open()) {
    for(const Order& order: cart) 
      file << order.date << ' ' << order.ID << ' ' << order.quantity << ' ' << order.price << '\n';
    file.close();
  }
  else {std::cout << "Failed to save the cart.\n";}
}

void Customer::save_counter() {
  std::ofstream file("pid.txt");
  if(file.is_open()) {
    file << counter;
    file.close();
  }
  else {std::cout << "Failed to save customer counter.\n";}
}
void Customer::load_counter() {
  std::ifstream file("pid.txt");
  if(file.is_open()) {
    file >> counter;
    file.close();
  }
  else {std::cout << "Failed to load customer counter.\n";}
}