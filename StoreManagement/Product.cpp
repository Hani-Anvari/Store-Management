#include "Product.h"
#include <fstream>

int Product::counter = 1;

void Product::set_name(std::string &name) {this->name = name;}
void Product::set_quantity(int &quantity) {this->quantity = quantity;}
void Product::set_price(int &price) {this->price = price;}
void Product::set_ID(int &ID) {this->ID = ID;}

std::string Product::get_name() const {return name;}
int Product::get_quantity() const {return quantity;}
int Product::get_price() const {return price;}
int Product::get_ID() const {return ID;}

bool Product::sell(int &num) {
  if(num > quantity) {return false;}
  else{quantity -=num; return true;}
}
void Product::restock(int &num) {quantity += num;}

//counter save&load
void Product::save_counter() {
  std::ofstream file("pid.txt");
  if(file.is_open()) {
    file << counter;
    file.close();
  }
  else {std::cout << "Failed to save Product counter.\n";}
}
void Product::load_counter() {
  std::ifstream file("pid.txt");
  if(file.is_open()) {
    file >> counter;
    file.close();
  }
  else {std::cout << "Failed to load Product counter.\n";}
}
