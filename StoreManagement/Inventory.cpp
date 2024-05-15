#include "Inventory.h"
#include "Product.h"
#include <fstream>
#include <iomanip>

void Inventory::load() {
    std::ifstream file(InventoryFile);
    if (file.is_open()) {
        int id, price, quantity; std::string name;
        while (file >> id >> name >> price >> quantity) {
            Product fproduct(name, price, quantity, false); fproduct.set_ID(id);
            inventory.emplace(id, fproduct);
            file.close();
        }
    }
    else {std::cout << "Failed to load Inventory.\n";}
}

void Inventory::save() {
    std::ofstream file(InventoryFile);
    if(file.is_open()) {
      for(const auto& pair : inventory)
        file << pair.second.get_ID() << ' ' << pair.second.get_name() << ' ' << pair.second.get_price() << ' ' << pair.second.get_quantity() << std::endl;
      file.close();
    }
    else {std::cout << "Failed to save Inventory.\n";}
}

void Inventory::update(Product &product) {inventory[product.get_ID()] = product;}

Product Inventory::findProduct(int &id) {return inventory[id];}

bool Inventory::SearchByID(int &id, Product &product) {
    product = inventory[id];
    if(product.get_ID() >= 1) {return true;}
    else {return false;}

}
bool Inventory::SearchByName(std::string &name, Product &product) {
    for(const auto& pair : inventory)
        if(name == pair.second.get_name()) {product = pair.second; return true;}
    return false;
}

void Inventory::Display() {
    std::cout << std::left << std::setw(4) << "ID" << std::setw(15) <<  "Name" << std::setw(10) << "Price" << std::setw(10) << "Quantity" << std::endl;
    for(const auto& pair: inventory) {
      std::cout << std::left << std::setw(4) << pair.second.get_ID() << std::setw(15) << pair.second.get_name() << std::setw(10) << pair.second.get_price() << std::setw(10) << pair.second.get_quantity() << '\n';
    }
}