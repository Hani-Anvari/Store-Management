/////////////////////////////////////////////////////////////////////////////////////////////////
// Email: hani.anvari@ut.ac.ir                                                                 //
// Github: https://github.com/Hani-Anvari/Store-Management.git                                 //
//                                                                                             //
//                                                                                             //
//                                                                                             //
//                                                                                             //
// *THIS PROGRAM HAS A DEFAULT DATA BASE ON GITHUB FOR TEST PURPOSES.                          //
// *THIS PROGRAM HAS ONLY EVER BEEN TESTED ON WINDOWS. IT MAY OR MAY NOT WORK ON LINUX.        //
/////////////////////////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include <fstream>
#include <map>
#include <unordered_map>
#include <string>
#include <vector>
#include <iomanip>

//file Names
const std::string InventoryFile  ="ProductsData.txt";
const std::string CustomersFile = "CustomersData.txt";
const std::string SalesFile     = "SalesData.txt";

struct CustomerData {
    std::string name, family;
    int ID, total;
};

struct Order {
  int ID, quantity, price;
  std::string date;
};

class Product {
  private:
    static int counter;
    int ID, quantity, price;
    std::string name;
  public:
  Product() {}
  Product(std::string &n, int &p, int &q, bool NewProduct = true): name(n), price(p), quantity(q) {if(NewProduct){ID = counter++;}}

  void set_name(std::string &name) {this->name = name;}
  void set_quantity(int &quantity) {this->quantity = quantity;}
  void set_price(int &price) {this->price = price;}
  void set_ID(int &ID) {this->ID = ID;}

  std::string get_name() const {return name;}
  int get_quantity() const {return quantity;}
  int get_price() const {return price;}
  int get_ID() const {return ID;}

  bool sell(int &num) {
    if(num > quantity) {return false;}
    else{quantity -=num; return true;}
  }
  void restock(int &num) {quantity += num;}

  //counter save&load
  static void save_counter() {
    std::ofstream file("pid.txt");
    if(file.is_open()) {
      file << counter;
      file.close();
    }
    else {std::cout << "Failed to save Product counter.\n";}
  }
  static void load_counter() {
    std::ifstream file("pid.txt");
    if(file.is_open()) {
      file >> counter;
      file.close();
    }
    else {std::cout << "Failed to load Product counter.\n";}
  }
};
int Product::counter = 1;


class Inventory {
  private:
    std::map<int, Product> inventory;
    const std::string InventoryFile;
  public:
  Inventory(const std::string &IF): InventoryFile(IF) {}

  void load() {
    std::ifstream file(InventoryFile);
    if(file.is_open()) {
      int id, price, quantity; std::string name;
      while(file >> id >> name >> price >> quantity) {
        Product fproduct(name, price, quantity, false); fproduct.set_ID(id);
        inventory[id] = fproduct;
      }
      file.close();
    }
    else {std::cout << "Failed to load Inventory.\n";}
 }

 void save() {
    std::ofstream file(InventoryFile);
    if(file.is_open()) {
      for(const auto& pair : inventory)
        file << pair.second.get_ID() << ' ' << pair.second.get_name() << ' ' << pair.second.get_price() << ' ' << pair.second.get_quantity() << std::endl;
      file.close();
    }
    else {std::cout << "Failed to save Inventory.\n";}
  }

  void update(Product &product) {inventory[product.get_ID()] = product;}



  Product findProduct(int &id) {return inventory[id];}

  bool SearchByID(int &id, Product &product) {
    product = inventory[id];
    if(product.get_ID() >= 1) {return true;}
    else {return false;}

  }
  bool SearchByName(std::string &name, Product &product) {
    for(const auto& pair : inventory)
      if(name == pair.second.get_name()) {product = pair.second; return true;}
    return false;
  }

  void Display() {
    std::cout << std::left << std::setw(4) << "ID" << std::setw(15) <<  "Name" << std::setw(10) << "Price" << std::setw(10) << "Quantity" << std::endl;
    for(const auto& pair: inventory) {
      std::cout << std::left << std::setw(4) << pair.second.get_ID() << std::setw(15) << pair.second.get_name() << std::setw(10) << pair.second.get_price() << std::setw(10) << pair.second.get_quantity() << '\n';
    }
  }
};

class SalesManagement {
  private:
    std::unordered_map<std::string, Order> SalesData;
    std::string fileName;
  public:
  SalesManagement(const std::string &file): fileName(file) {}

  bool load() {
    std::ifstream file(fileName);
    if(file.is_open()) {
      Order order;
      while(file >> order.date >> order.ID >> order.quantity >> order.price) {SalesData[order.date] = order;}
      file.close();
      return true;
    }
    else {std::cout << "Failed to load Sales Data.\n"; return false;}
}

  bool add(Order order) {
    std::ofstream file(fileName, std::ios::app);
    if(file.is_open()) {
      file << order.date << ' ' <<order.ID << ' ' << order.quantity << ' ' << order.price << '\n';
      file.close();
      SalesData[order.date] = order;
      return true;
    }
    else {std::cout << "Failed to add to Sales Data.\n"; return false;}
    }

  int turnover(std::string &startDate, std::string &endDate) {
    int turnover = 0.0;
    for(const auto &pair: SalesData) {
      if(pair.second.date >= startDate && pair.second.date <= endDate) {
        turnover += pair.second.price * pair.second.quantity;
      }
    }
    return turnover;
  }

  void display(Inventory &inventory) {
    std::cout << std::left << std::setw(15) << "Date" << std::setw(15) <<  "Name" << std::setw(10) << "Price" << std::setw(10) << "Quantity" << std::setw(10) << "Total" << std::endl;
    for(const auto& pair: SalesData) {
      int id = pair.second.ID;
      Product product = inventory.findProduct(id);
      std::cout << std::left <<std::setw(15) << pair.second.date << std::setw(15) << product.get_name() << std::setw(10) << pair.second.price << std::setw(10) << pair.second.quantity << std::setw(10) << pair.second.price * pair.second.quantity << '\n';
    }
  }
};

class Customer {
  private:
    CustomerData cData;
    std::vector<Order> cart;
    std::string cartName;
    static int counter;
  public:
    Customer() {}
    Customer(std::string &n, std::string &f) {
      cData.name = n;
      cData.family = f;
      cData.total = 0;
      cData.ID = counter++;
      cartName = "Cart" + std::to_string(cData.ID) + ".txt";
  }

  void set_name(std::string &name) {cData.name = name;}
  void set_family(std::string &family) {cData.family = family;}
  void set_total(int &total) {cData.total = total;}
  void set_ID(int &ID) {cData.ID = ID;}
  void set_cartName() {cartName = "Cart" + std::to_string(cData.ID) + ".txt";}

  int get_ID() const {return cData.ID;}
  std::string get_name() const {return cData.name;}
  std::string get_family() const {return cData.family;}
  int get_total() const {return cData.total;}
  CustomerData get_cData() const {return cData;}

  bool buy(Product &p, Order &order) {
    if(p.sell(order.quantity)) {
      cData.total += p.get_price() * order.quantity;
      cart.push_back(order);
      return true;
    }
    else{return false;}
  }
//void Cusomer::pay() {total = 0;}

  void displayCart(Inventory &inventory) {
    if (cart.empty()) {
      std::cout << "Your cart is empty." << std::endl;
      return;
    }
    else {
      int TotalPrice = 0;
      std::cout << std::left << std::setw(15) << "Date" << std::setw(15) <<  "Name" << std::setw(10) << "Price" << std::setw(10) << "Quantity" << std::setw(10) << "Total" << std::endl;
      for(const Order& order: cart) {
        int id = order.ID;
        TotalPrice += order.price * order.quantity;
        Product product = inventory.findProduct(id);
        std::cout << std::left << std::setw(15) << order.date << std::setw(15) << product.get_name() << std::setw(10) << order.price << std::setw(10) << order.quantity << std::setw(10) << order.price * order.quantity << '\n';
      }
      std::cout << "Total Price of this Cart: " << TotalPrice << '\n';
    }
  }

  void loadCart() {
    std::ifstream file(cartName);
    if(file.is_open()) {
      Order order;
      while(file >> order.date >> order.ID >> order.quantity >> order.price)
        cart.push_back(order);
      file.close();
    }
    else {std::cout << "Failed to load the cart.\n";}
  }

  void saveCart() {
    std::ofstream file(cartName, std::ios::app);
    if(file.is_open()) {
      for(const Order& order: cart)
        file << order.date << ' ' << order.ID << ' ' << order.quantity << ' ' << order.price << '\n';
      file.close();
    }
    else {std::cout << "Failed to save the cart.\n";}
  }


  static void save_counter() {
    std::ofstream file("cid.txt");
    if(file.is_open()) {
      file << counter;
      file.close();
  }
    else {std::cout << "Failed to save customer counter.\n";}
  }

  static void load_counter() {
    std::ifstream file("cid.txt");
    if(file.is_open()) {
      file >> counter;
      file.close();
    }
    else {std::cout << "Failed to load customer counter.\n";}
  }
};
int Customer::counter = 1;

class CustomerList {
  private:
    std::map<int, CustomerData> customerList;
    std::string CustomersFile;
  public:
  CustomerList(const std::string &CF): CustomersFile(CF) {}

  void load() {
    std::ifstream file(CustomersFile);
    if(file.is_open()) {
      CustomerData customer;
      while(file >> customer.ID >> customer.name >> customer.family >> customer.total) {customerList[customer.ID] = customer;}
      file.close();
    }
    else {std::cout << "Failed to load customer list.\n";}
  }

  void save() {
    std::ofstream file(CustomersFile);
    if(file.is_open()) {
      CustomerData customer;
      for(const auto& pair: customerList)
        file << pair.second.ID << ' ' << pair.second.name << ' ' << pair.second.family << ' ' << pair.second.total << '\n';
      file.close();
    }
    else {std::cout << "Failed to save customer list.\n";}
  }

  void update(CustomerData customer) {customerList[customer.ID] = customer;}
  CustomerData find(int &id, Customer &customer) {return customerList[id];}

  bool SearchByID(int &id, Customer &customer) {
    CustomerData customerdata = customerList[id];
    if(customerdata.ID >= 1) {
        customer.set_ID(customerdata.ID); customer.set_name(customerdata.name); customer.set_family(customerdata.family); customer.set_total(customerdata.total); customer.set_cartName();
        return true;
    }
    else {return false;}
}

  bool SearchByName(std::string &name, std::string &family, Customer &customer) {
    for(const auto& pair : customerList)
        if(name == pair.second.name && family == pair.second.family) {
            CustomerData customerdata = pair.second;
            customer.set_ID(customerdata.ID); customer.set_name(customerdata.name); customer.set_family(customerdata.family); customer.set_total(customerdata.total); customer.set_cartName();
        return true;
        }
    return false;
  }

  void Display() {
    std::cout << std::left << std::setw(4) << "ID" << std::setw(15) << "Name" << std::setw(15) << "Family" << std::setw(10) << "Total" << std::endl;
    for(const auto& pair: customerList) {
      std::cout << std::left << std::setw(4) << pair.second.ID << std::setw(15) << pair.second.name << std::setw(15) << pair.second.family << std::setw(10) << pair.second.total << '\n';
    }
  }
};

void displayMainMenu();
void displayProductMenu();
void displayCustomerMenu();
void displaySalesDataMenu();
bool addCustomer(Inventory &inventory, SalesManagement &notebook, CustomerList &cList);
bool addProduct(Inventory &inventory);
bool addPurchase(Customer &customer, Inventory &inventory, SalesManagement &notebook, CustomerList &cList);
bool DisplayProduct (Inventory &inventory);
bool EditProduct(Inventory &inventory ,Product &product);
bool DisplayCustomer(CustomerList &cList, Inventory &inventory, SalesManagement &notebook);
bool EditCustomer(CustomerList cList, Customer customer);
bool calTurnover(SalesManagement &notebook);
bool displaySalesData(SalesManagement &notebook, Inventory &inventory);
void dateManip(std::string &startDate, std::string &endDate,const std::string &frame);

int main() {
  //loading App's data
  Product::load_counter();
  Customer::load_counter();
  Inventory inventory(InventoryFile);
  inventory.load();
  CustomerList cList(CustomersFile);
  cList.load();
  SalesManagement notebook(SalesFile);
  notebook.load();

  bool running = true;
  std::string command;
do {
  //Welcome
  displayMainMenu();
command:
  std::cin >> command;

  //Product Section
  if(command == "1") {
    displayProductMenu();
  Pcommand:
    std::cin >> command;
    if(command == "1") {
      running = addProduct(inventory);
    } else if(command == "2") {
      running = DisplayProduct(inventory);
    }
    else if(command == "0") {std::system("CLS"); running = true;}
    else if(command == "00") {std::system("CLS"); running = false;}
    else {std::cout << "Invalid choice. Please try again.\n"; goto Pcommand;}
  }

  //Customer Section
  else if(command == "2") {
    displayCustomerMenu();
  Ccommand:
    std::cin >> command;
    if(command == "1") {
      running = addCustomer(inventory, notebook, cList);
    }
    else if(command == "2") {
      running = DisplayCustomer(cList, inventory, notebook);
    }
    else if(command == "0") {std::system("CLS"); running = true;}
    else if(command == "00") {std::system("CLS"); running = false;}
    else {std::cout << "Invalid choice. Please try again.\n"; goto Ccommand;}
  }

  //SalesData Section
  else if(command == "3") {
    displaySalesDataMenu();
  Scommand:
    std::cin >> command;
    if(command == "1") {
      running = calTurnover(notebook);
    }
    else if(command == "2") {
      running = displaySalesData(notebook, inventory);
    }
    else if(command == "0") {std::system("CLS"); running = true;}
    else if(command == "00") {std::system("CLS"); running = false;}
    else {std::cout << "Invalid choice. Please try again.\n"; goto Scommand;}
  }

  else if(command == "0") {std::system("CLS"); return true;}
  else if(command == "00") {std::system("CLS"); return false;}
  else {std::cout << "Invalid choice. Please try again.\n"; goto command;}

} while(running);

//Saving App's data
  inventory.save();
  cList.save();
  Product::save_counter();
  Customer::save_counter();
}

void displayMainMenu() {
std::cout << "Welcome to the Product Management App.\n\n"
          << "Please choose a section:\n"
          << "1.Product\n"
          << "2.Customer\n"
          << "3.SalesData\n"
          << "0.Restart the App\n"
          << "00.Exit the App\n"
          << "Enter the Number of the section you want: ";
}

void displayProductMenu() {
std::system("CLS");
std::cout << "Welcome to the Product section.\n\n"
          << "Please choose an action:\n"
          << "1.Add Product\n"
          << "2.Display Products\n"
          << "0.Restart the App\n"
          << "00.Exit the App\n"
          << "Enter the Number of the action you want: ";
}

void displayCustomerMenu() {
std::system("CLS");
std::cout << "Welcome to the Customer section.\n\n"
          << "Please choose an action:\n"
          << "1.Add Customer\n"
          << "2.Display Customers List\n"
          << "0.Restart the App\n"
          << "00.Exit the App\n"
          << "Enter the Number of the action you want: ";
}

void displaySalesDataMenu() {
std::system("CLS");
std::cout << "Welcome to the Sales Data section.\n\n"
          << "Please choose an action:\n"
          << "1.Calculate your business turnover.\n"
          << "2.Display Sales Data\n"
          << "0.Restart the App\n"
          << "00.Exit the App\n"
          << "Enter the Number of the action you want: ";
}

bool addCustomer(Inventory &inventory, SalesManagement &notebook, CustomerList &cList) {
AddCustomer:
  std::system("CLS");
  std::string command;
  CustomerData customer;
std::cout << "Here you can Add your customers data and save them for later use.\n"
          << "Customer's Name: ";   std::cin >> customer.name;
std::cout << "Customer's Family: "; std::cin >> customer.family;
    Customer newcustomer(customer.name, customer.family); customer.ID = newcustomer.get_ID();
    cList.update(customer);
std::cout << "your customer is added to the Customers List.\n"
          << "This customer's ID is: " << customer.ID << '\n'
          << "=================================\n"
          << "1.Add purchases for this customer.\n"
          << "2.Add another customer.\n"
          << "0.Return to the Main Menu.\n"
          << "00.Exit the app.\n"
          << "Enter the Number of the action you want: ";
  command2:
    std::cin >> command;
    if(command == "1") {
      if(addPurchase(newcustomer, inventory, notebook, cList)) {return true;}
      else {return false;}
    } else if(command == "2") {std::system("CLS"); goto AddCustomer;
    } else if(command == "0") {std::system("CLS"); return true;
    } else if(command == "00") {std::system("CLS"); return false;
    } else {std::cout << "Invalid choice. Please try again.\n"; goto command2;}
}

bool addProduct(Inventory &inventory) {
AddProduct:
  std::system("CLS");
  std::string command;
std::cout << "Here you can Add your products data and save them for later use.\n"
          << "Product's Name: ";     std::string name; std::cin >> name;
std::cout << "Product's price: ";         int price;   std::cin >> price;
std::cout << "Product's quantity: ";      int quantity;std::cin >> quantity;
    Product newproduct(name, price, quantity, true);
    inventory.update(newproduct);
std::cout << "your product is added to the inventory.\n"
          << "This product's ID is: " << newproduct.get_ID() << '\n'
          << "=================================\n"
          << "1.Add another product.\n"
          << "0.return to the main menu.\n"
          << "00.Exit the app.\n"
          << "Enter the Number of the action you want: ";
    command1:
      std::cin >> command;
      if(command == "1") {std::system("CLS"); goto AddProduct;}
      else if(command == "0") {std::system("CLS"); return true;}
      else if(command == "00") {std::system("CLS"); return false;}
      else {std::cout << "Invalid choice. Please try again.\n"; goto command1;}
}

bool addPurchase(Customer &customer, Inventory &inventory, SalesManagement &notebook, CustomerList &cList) {
  customer.loadCart();
AddPurchase:
  std::system("CLS");
  std::string command;
    Order order;
std::cout << "Here you can record the purchases of the customer: " << customer.get_name() << ' ' << customer.get_family() << " Customer ID: " << customer.get_ID() << '\n'
          << "please provide the purchase's details:\n"
          << "Date of purchase(y-m-d\\e.g. 2024-04-05): "; std::cin >> order.date;
ChangeProduct:
std::cout << "Product's ID: "; std::cin >> order.ID;
    Product product = inventory.findProduct(order.ID);
std::cout << "Product's details:\n"<< std::left << std::setw(4) << "ID" << std::setw(15) << "Name" << std::setw(10) << "Price" << std::setw(10) << "Quantity" << std::endl
                      << std::setw(4) << product.get_ID() << std::setw(15) << product.get_name() << std::setw(10) << product.get_price() << std::setw(10) << product.get_quantity() << '\n'
          << "1.Continue\n"
          << "2.Change Product\n"
          << "0.Return to the main menu.\n"
          << "00.Exit the App\n"
          << "Enter the Number of the action you want: "; apcommand: std::cin >> command;
          if(command == "1") {
            std::cout << "Amount of the order: "; amount: std::cin >> order.quantity; order.price = product.get_price();
            if(customer.buy(product, order)) {
              std::system("CLS");
              cList.update(customer.get_cData());
              notebook.add(order);
              inventory.update(product);
              std::cout << "Purchase recorded successfully.\n"
                        << "=================================\n"
                        << "1.Add another purchase.\n"
                        << "0.Return to the main menu.\n"
                        << "00.Exit the App"
                        << "Enter the Number of the action you want: ";
                    command4:
              std::cin >> command;
              if(command == "1") {std::system("CLS"); goto AddPurchase;}
              else if(command == "0") {std::system("CLS"); customer.saveCart(); return true;}
              else if(command == "00") {std::system("CLS"); customer.saveCart(); return false;}
              else {std::cout << "Invalid choice. Please try again.\n"; goto command4;}
            }
            else{
          std::cout << "Amount of the order exceedes the remaining quantity of this product in the inventory.\n"
                    << "=================================\n"
                    << "1.Edit amount of the order\n"
                    << "2.Add another purchase\n"
                    << "0.Return to the main menu\n"
                    << "00.Exit the App"
                    << "Enter the Number of the action you want: ";
            command3:
              std::cin >> command;
              if(command == "1") {goto amount;}
              else if(command == "2") {std::system("CLS"); goto AddPurchase;}
              else if(command == "0") {std::system("CLS"); customer.saveCart(); return true;}
              else if(command == "00") {std::system("CLS"); customer.saveCart(); return false;}
              else {std::cout << "Invalid choice. Please try again.\n"; goto command3;}
            }
          }
          else if(command == "2") {std::system("CLS"); goto ChangeProduct;}
          else if(command == "0") {std::system("CLS"); return true;}
          else if(command == "00") {std::system("CLS"); return false;}
          else {std::cout << "Invalid choice. Please try again.\n"; goto apcommand;}

}
bool DisplayProduct (Inventory &inventory) {
  std::system("CLS");
  std::cout << "Here you can search for or review your products' data.\n"
            << "Please choose an action:\n"
            << "1.Search\n"
            << "2.Display Inventory\n"
            << "0.Return to the main menu\n"
            << "00.Exit the App\n"
            << "Enter the number of the action you want: "; dpcommand: std::string command; std::cin>>command;
    if(command == "1") {
    Search:
      std::system("CLS");
      std::cout << "Search section\n\n"
                << "Please choose a search method:\n"
                << "1.Search by ID\n"
                << "2.Search by Name\n"
                << "0.Return to the main menu\n"
                << "00.Exit the App\n"
                << "Enter the number of the action you want: "; dpcommand2: std::cin>>command;
        if(command == "1") {
          std::system("CLS");
          std::cout << "Please Enter an ID: "; int id; std::cin>>id;
          Product product; bool flag;
          flag = inventory.SearchByID(id, product);
          if(!flag) {
            std::cout << "No Product found!\n"
                      << "Type anything to return.";
            std::cin >> command; goto Search;
          }
          else {
            std::cout << "Product found! \n"
                      << std::left << std::setw(4) << "ID" << std::setw(15) << "Name" << std::setw(10) << "Price" << std::setw(10) << "Quantity" << std::endl
                      << std::setw(4) << product.get_ID() << std::setw(15) << product.get_name() << std::setw(10) << product.get_price() << std::setw(10) << product.get_quantity() << '\n'
                      << "=================================\n"
                      << "1.Edit this product\n"
                      << "2.Search again\n"
                      << "0.Return to the main menu\n"
                      << "00.Exit the App\n"
                      << "Enter the number of the action you want: "; dpcommand3: std::cin>>command;
            if(command == "1") {
              if(EditProduct(inventory, product)) {return true;}
              else {return false;}
            }
            else if(command == "2") {goto Search;}
            else if(command == "0") {std::system("CLS"); return true;}
            else if(command == "00") {std::system("CLS"); return false;}
            else {std::cout << "Invalid choice. Please try again.\n"; goto dpcommand3;}
          }
        }
        else if(command == "2") {
          std::system("CLS");
          std::cout << "Please Enter a Name: "; std::string name; std::cin>>name;
          Product product; bool flag;
          flag = inventory.SearchByName(name, product);
          if(!flag) {
            std::cout << "No Product found!\n"
                      << "Type anything to return.";
            std::cin >> command; goto Search;
          }
          else {
            std::cout << "Product found! \n"
                      << std::left << std::setw(4) << "ID" << std::setw(15) << "Name" << std::setw(10) << "Price" << std::setw(10) << "Quantity" << std::endl
                      << std::setw(4) << product.get_ID() << std::setw(15) << product.get_name() << std::setw(10) << product.get_price() << std::setw(10) << product.get_quantity() << '\n'
                      << "=================================\n"
                      << "1.Edit this product\n"
                      << "2.Search again\n"
                      << "0.Return to the main menu\n"
                      << "00.Exit the App\n"
                      << "Enter the number of the action you want: "; dpcommand4: std::cin>>command;
            if(command == "1") {
              if(EditProduct(inventory, product)) {return true;}
              else {return false;}
            }
            else if(command == "2") {goto Search;}
            else if(command == "0") {std::system("CLS"); return true;}
            else if(command == "00") {std::system("CLS"); return false;}
            else {std::cout << "Invalid choice. Please try again.\n"; goto dpcommand4;}
          }
        }
        else if(command == "0") {std::system("CLS"); return true;}
        else if(command == "00") {std::system("CLS"); return false;}
        else {std::cout << "Invalid choice. Please try again.\n"; goto dpcommand2;}
    }
    else if(command == "2") {
      std::system("CLS");
      std::cout << "Inventory section\n\n";
      inventory.Display();
      std::cout << "=================================\n"
                << "0.Return to the main menu\n"
                << "00.Exit the App\n"
                << "Enter the number of the parameter you want: "; epcommand5: std::cin>>command;
    if(command == "0") {std::system("CLS"); return true;}
    else if(command == "00") {std::system("CLS"); return false;}
    else {std::cout << "Invalid choice. Please try again.\n"; goto epcommand5;}
    }
    else if(command == "0") {std::system("CLS"); return true;}
    else if(command == "00") {std::system("CLS"); return false;}
    else {std::cout << "Invalid choice. Please try again.\n"; goto dpcommand;}
}

bool EditProduct(Inventory &inventory ,Product &product) {
EditProduct:
  std::system("CLS");
  std::string command;
  std::cout << "Product Editing Section\n\n"
            << "Choose the parameter you want to edit:\n"
            << "1.Name\n"
            << "2.Price\n"
            << "3.Quantity\n"
            << "0.Return to the main menu\n"
            << "00.Exit the App\n"
            << "Enter the number of the parameter you want: "; epcommand: std::cin>>command;
    if(command == "1") {
              std::system("CLS");
              std::cout << "Editing Name\n"
                        << "Current Name: " << product.get_name()
                        << " New Name: "; std::string name; std::cin>>name; product.set_name(name); inventory.update(product);
              std::system("CLS");
              std::cout << "Product's Name succussfully changed to: " << product.get_name()
                        << "\n=================================\n"
                        << "1.Edit\n"
                        << "0.Return to the main menu\n"
                        << "00.Exit the App\n"
                        << "Enter the number of the parameter you want: "; epcommand2: std::cin>>command;
              if(command == "1") {goto EditProduct;}
              else if(command == "0") {std::system("CLS"); return true;}
              else if(command == "00") {std::system("CLS"); return false;}
              else {std::cout << "Invalid choice. Please try again.\n"; goto epcommand2;}
            }
    else if(command == "2") {
              std::system("CLS");
              std::cout << "Editing Price\n"
                        << "Current Price: " << product.get_price()
                        << " New Price: "; int price; std::cin>>price; product.set_price(price); inventory.update(product);
              std::system("CLS");
              std::cout << "Product's Price succussfully changed to: " << product.get_price()
                        << "\n=================================\n"
                        << "1.Edit\n"
                        << "0.Return to the main menu\n"
                        << "00.Exit the App\n"
                        << "Enter the number of the parameter you want: "; epcommand3: std::cin>>command;
              if(command == "1") {goto EditProduct;}
              else if(command == "0") {std::system("CLS"); return true;}
              else if(command == "00") {std::system("CLS"); return false;}
              else {std::cout << "Invalid choice. Please try again.\n"; goto epcommand3;}
            }
    else if(command == "3") {
              std::system("CLS");
              std::cout << "Editing Quantity\n"
                        << "Current Quantity: " << product.get_quantity()
                        << " New Quantity: "; int quantity; std::cin>>quantity; product.set_quantity(quantity); inventory.update(product);
              std::system("CLS");
              std::cout << "Product's Quantity succussfully changed to: " << product.get_quantity()
                        << "\n=================================\n"
                        << "1.Edit\n"
                        << "0.Return to the main menu\n"
                        << "00.Exit the App\n"
                        << "Enter the number of the parameter you want: "; epcommand4: std::cin>>command;
              if(command == "1") {goto EditProduct;}
              else if(command == "0") {std::system("CLS"); return true;}
              else if(command == "00") {std::system("CLS"); return false;}
              else {std::cout << "Invalid choice. Please try again.\n"; goto epcommand4;}
            }
    else if(command == "0") {std::system("CLS"); return true;}
    else if(command == "00") {std::system("CLS"); return false;}
    else {std::cout << "Invalid choice. Please try again.\n"; goto epcommand;}
}

bool DisplayCustomer(CustomerList &cList, Inventory &inventory, SalesManagement &notebook) {
  std::system("CLS");
  std::cout << "Here you can search for or review Customers' data.\n"
            << "Please choose an action:\n"
            << "1.Search\n"
            << "2.Display Customers List\n"
            << "0.Return to the main menu\n"
            << "00.Exit the App\n"
            << "Enter the number of the action you want: "; dpcommand: std::string command; std::cin>>command;
    if(command == "1") {
    Search:
      std::system("CLS");
      std::cout << "Search section\n\n"
                << "Please choose a search method:\n"
                << "1.Search by ID\n"
                << "2.Search by Name and family\n"
                << "0.Return to the main menu\n"
                << "00.Exit the App\n"
                << "Enter the number of the action you want: "; dpcommand2: std::cin>>command;
        if(command == "1") {
          std::system("CLS");
          std::cout << "Please Enter an ID: "; int id; std::cin>>id;
          Customer customer; bool flag;
          flag = cList.SearchByID(id, customer);
          if(!flag) {
            std::cout << "No Customer found!\n"
                      << "Type anything to return.";
            std::cin >> command; goto Search;
          }
          else {
            std::system("CLS");
            std::cout << "Customer found! \n"
                      << std::left << std::setw(4) << "ID" << std::setw(15) << "Name" << std::setw(15) << "family" << std::setw(10) << "Total" << std::endl
                      << std::setw(4) << customer.get_ID() << std::setw(15) << customer.get_name() << std::setw(15) << customer.get_family() << std::setw(10) << customer.get_total() << '\n'
                      << "=================================\n"
                      << "1.Edit this Customer\n"
                      << "2.Display this Customer's cart\n"
                      << "3.Add Purchases for this customer\n"
                      << "4.Search again\n"
                      << "0.Return to the main menu\n"
                      << "00.Exit the App\n"
                      << "Enter the number of the action you want: "; dpcommand3: std::cin>>command;
            if(command == "1") {
              if(EditCustomer(cList, customer)) {return true;}
              else {return false;}
            }
            else if(command == "2") {
              std::cout << "Customer's Cart preview Section.\n\n";
              customer.displayCart(inventory);
              std::cout << "=================================\n"
                        << "0.Return to the main menu\n"
                        << "00.Exit the App\n"
                        << "Enter the number of the action you want: "; dpcommand6: std::cin>>command;
              if(command == "0") {std::system("CLS"); return true;}
              else if(command == "00") {std::system("CLS"); return false;}
              else {std::cout << "Invalid choice. Please try again.\n"; goto dpcommand6;}
            }
            else if(command == "3") {
              if(addPurchase(customer, inventory, notebook, cList)) {return true;}
              else {return false;}
            }
            else if(command == "4") {goto Search;}
            else if(command == "0") {std::system("CLS"); return true;}
            else if(command == "00") {std::system("CLS"); return false;}
            else {std::cout << "Invalid choice. Please try again.\n"; goto dpcommand6;}
          }
        }
        else if(command == "2") {
          std::system("CLS");
          std::cout << "Please Enter a full name(name family\\e.g. Mahdi Ahmadi): "; std::string name, family; std::cin>>name>>family;
          Customer customer; bool flag;
          flag = cList.SearchByName(name, family, customer);
          if(!flag) {
            std::cout << "No Customer found!\n"
                      << "Type anything to return.";
            std::cin >> command; goto Search;
          }
          else {
            std::cout << "Customer found! \n"
                      << std::left << std::setw(4) << "ID" << std::setw(15) << "Name" << std::setw(15) << "family" << std::setw(10) << "Total" << std::endl
                      << std::setw(4) << customer.get_ID() << std::setw(15) << customer.get_name() << std::setw(15) << customer.get_family() << std::setw(10) << customer.get_total() << '\n'
                      << "=================================\n"
                      << "1.Edit this Customer\n"
                      << "2.Display this Customer's cart\n"
                      << "3.Add Purchases for this customer\n"
                      << "4.Search again\n"
                      << "0.Return to the main menu\n"
                      << "00.Exit the App\n"
                      << "Enter the number of the action you want: "; dpcommand4: std::cin>>command;
            if(command == "1") {
              if(EditCustomer(cList, customer)) {return true;}
              else {return false;}
            }
            else if(command == "2") {
              std::cout << "Customer's Cart preview Section.\n\n";
              customer.displayCart(inventory);
              std::cout << "=================================\n"
                        << "0.Return to the main menu\n"
                        << "00.Exit the App\n"
                        << "Enter the number of the action you want: "; dpcommand7: std::cin>>command;
              if(command == "0") {std::system("CLS"); return true;}
              else if(command == "00") {std::system("CLS"); return false;}
              else {std::cout << "Invalid choice. Please try again.\n"; goto dpcommand7;}
            }
            else if(command == "3") {
              if(addPurchase(customer, inventory, notebook, cList)) {return true;}
              else {return false;}
            }
            else if(command == "4") {goto Search;}
            else if(command == "0") {std::system("CLS"); return true;}
            else if(command == "00") {std::system("CLS"); return false;}
            else {std::cout << "Invalid choice. Please try again.\n"; goto dpcommand4;}
          }
        }
        else if(command == "0") {std::system("CLS"); return true;}
        else if(command == "00") {std::system("CLS"); return false;}
        else {std::cout << "Invalid choice. Please try again.\n"; goto dpcommand2;}
    }
    else if(command == "2") {
      std::system("CLS");
      std::cout << "Customer List section\n\n";
      cList.Display();
      std::cout << "=================================\n"
                << "0.Return to the main menu\n"
                << "00.Exit the App\n"
                << "Enter the number of the action you want: "; epcommand5: std::cin>>command;
    if(command == "0") {std::system("CLS"); return true;}
    else if(command == "00") {std::system("CLS"); return false;}
    else {std::cout << "Invalid choice. Please try again.\n"; goto epcommand5;}
    }
    else if(command == "0") {std::system("CLS"); return true;}
    else if(command == "00") {std::system("CLS"); return false;}
    else {std::cout << "Invalid choice. Please try again.\n"; goto dpcommand;}
}

bool EditCustomer(CustomerList cList, Customer customer) {
EditCustomer:
  std::system("CLS");
  std::string command;
  std::cout << "Customer Editing Section\n\n"
            << "Choose the parameter you want to edit:\n"
            << "1.Name\n"
            << "2.Family\n"
            << "3.Total\n"
            << "0.Return to the main menu\n"
            << "00.Exit the App\n"
            << "Enter the number of the parameter you want: "; eccommand: std::cin>>command;
    if(command == "1") {
              std::system("CLS");
              std::cout << "Editing Name\n"
                        << "Current Name: " << customer.get_name()
                        << " New Name: "; std::string name; std::cin>>name; customer.set_name(name); CustomerData cd = customer.get_cData(); cList.update(cd);
              std::system("CLS");
              std::cout << "Customer's Name succussfully changed to: " << customer.get_name()
                        << "\n=================================\n"
                        << "1.Edit\n"
                        << "0.Return to the main menu\n"
                        << "00.Exit the App\n"
                        << "Enter the number of the parameter you want: "; eccommand2: std::cin>>command;
              if(command == "1") {goto EditCustomer;}
              else if(command == "0") {std::system("CLS"); return true;}
              else if(command == "00") {std::system("CLS"); return false;}
              else {std::cout << "Invalid choice. Please try again.\n"; goto eccommand2;}
            }
    else if(command == "2") {
              std::system("CLS");
              std::cout << "Editing Family name\n"
                        << "Current Family name: " << customer.get_family()
                        << " New Family name: "; std::string family; std::cin>>family; customer.set_family(family); CustomerData cd = customer.get_cData(); cList.update(cd);
              std::system("CLS");
              std::cout << "Customer's Family name succussfully changed to: " << customer.get_family()
                        << "\n=================================\n"
                        << "1.Edit\n"
                        << "0.Return to the main menu\n"
                        << "00.Exit the App\n"
                        << "Enter the number of the parameter you want: "; eccommand3: std::cin>>command;
              if(command == "1") {goto EditCustomer;}
              else if(command == "0") {std::system("CLS"); return true;}
              else if(command == "00") {std::system("CLS"); return false;}
              else {std::cout << "Invalid choice. Please try again.\n"; goto eccommand3;}
            }
    else if(command == "3") {
              std::system("CLS");
              std::cout << "Editing Total\n"
                        << "Current Total: " << customer.get_total()
                        << " New Total: "; int total; std::cin>>total; customer.set_total(total); CustomerData cd = customer.get_cData(); cList.update(cd);
              std::system("CLS");
              std::cout << "Customer's Total succussfully changed to: " << customer.get_total()
                        << "\n=================================\n"
                        << "1.Edit\n"
                        << "0.Return to the main menu\n"
                        << "00.Exit the App\n"
                        << "Enter the number of the parameter you want: "; eccommand4: std::cin>>command;
              if(command == "1") {goto EditCustomer;}
              else if(command == "0") {std::system("CLS"); return true;}
              else if(command == "00") {std::system("CLS"); return false;}
              else {std::cout << "Invalid choice. Please try again.\n"; goto eccommand4;}
            }
    else if(command == "0") {std::system("CLS"); return true;}
    else if(command == "00") {std::system("CLS"); return false;}
    else {std::cout << "Invalid choice. Please try again.\n"; goto eccommand;}
}

bool calTurnover(SalesManagement &notebook) {
  cal:
    std::string command;
    std::system("CLS");
    std::cout << "Here you can calculate the Turnover of your bussiness.\n"
              << "Please choose a time frame:\n"
              << "1.Last week\n"
              << "2.Last fortnight\n"
              << "3.Last month\n"
              << "4.Last season\n"
              << "5.Last year\n"
              << "6.Custom\n"
              << "0.Return to the main menu.\n"
              << "00.Exit the App.\n"
              << "Enter the Number of the action you wish to do: "; ctcommand: std::cin >> command;
    std::string startDate;
    std:: string endDate;
    if(command == "1") {
      dateManip(startDate, endDate, command);
      std::cout << "Your Turnover for Last week is: " << notebook.turnover(startDate, endDate) << '\n';
    }
    else if(command == "2") {
      dateManip(startDate, endDate, command);
      std::cout << "Your Turnover for Last fortnight is: " << notebook.turnover(startDate, endDate) << '\n';
    }
    else if(command == "3") {
      dateManip(startDate, endDate, command);
      std::cout << "Your Turnover for Last month is: " << notebook.turnover(startDate, endDate) << '\n';
    }
    else if(command == "4") {
      dateManip(startDate, endDate, command);
      std::cout << "Your Turnover for Last season is: " << notebook.turnover(startDate, endDate) << '\n';
    }
    else if(command == "5") {
      dateManip(startDate, endDate, command);
      std::cout << "Your Turnover for Last year is: " << notebook.turnover(startDate, endDate) << '\n';
    }
    else if(command == "6") {
      std::cout << "Please enter a date as the starting point of calculation(y-m-d\\e.g. 2024-04-05): "; std::cin >> startDate;
      std::cout << "now enter a date as the ending point of calculation(y-m-d\\e.g. 2024-04-05): "; std::cin >> endDate;
      std::cout << "Your Turnover from " << startDate << " to " << endDate << " is: " << notebook.turnover(startDate, endDate) << '\n';
    }
    else if(command == "0") {std::system("CLS"); return true;}
    else if(command == "00") {std::system("CLS"); return false;}
    else {std::cout << "Invalid choice. Please try again.\n"; goto cal;}

    std::cout << "=================================\n"
              << "1.Calculate Turnover for a different time frame.\n"
              << "0.Return to the main menu.\n"
              << "00.Exit the App.\n"
              << "Enter the Number of the action you wish to do: ";
    command4:
      std::cin >> command;
      if(command == "1") {goto cal;}
      else if(command == "0") {std::system("CLS"); return true;}
      else if(command == "00") {std::system("CLS"); return false;}
      else {std::system("CLS");
            std::cout << "Invalid choice. Please try again.\n"
                      << "=================================\n"
                      << "1.Calculate Turnover for a different time frame.\n"
                      << "2.Return to the main menu.\n"
                      << "0.Exit the App.\n"
                      << "Enter the Number of the action you wish to do: ";
            goto command4;}

}

bool displaySalesData(SalesManagement &notebook, Inventory &inventory) {
  std::system("CLS");
  std::string command;
  std::cout << "Sale Data section\n\n";
  notebook.display(inventory);
  std::cout << "=================================\n"
            << "0.Return to the main menu\n"
            << "00.Exit the App\n"
            << "Enter the number of the parameter you want: "; dscommand: std::cin>>command;
            if(command == "0") {std::system("CLS"); return true;}
            else if(command == "00") {std::system("CLS"); return false;}
            else {std::cout << "Invalid choice. Please try again.\n"; goto dscommand;}
}

void dateManip(std::string &startDate, std::string &endDate,const std::string &frame) {
  std::system("CLS");
  std::cout << "Please enter today's date(y-m-d\\e.g. 2024-04-05): "; std::cin >> endDate;
  int day, month, year; sscanf(endDate.c_str(), "%d-%d-%d", &year, &month, &day);
  if(frame == "1") {day -= 7;}
  else if(frame == "2") {day -= 14;}
  else if(frame == "3") {month -= 1;}
  else if(frame == "4") {month -= 3;}
  else if(frame == "5") {year -= 1;}

  if(day < 1) {
    int m;
    if (m == 1 || m == 3 || m == 5 || m == 7 || m == 8 || m == 10 || m == 12) {m = 31;}
    else if (m == 4 || m == 6 || m == 9 || m == 11) {m = 30;}
    else if (m == 2) {
      if(year % 4 == 0) {m == 29;}
      else {m == 28;}
    }
    day = m - day;
    month -= 1;
  }
  if(month < 1) {
    month = 12 - month;
    year -= 1;
  }
  if(day < 10) {
    startDate = std::to_string(year) + "-" + std::to_string(month) + "-0" + std::to_string(day);
  }
  else if(month < 10) {
    startDate = std::to_string(year) + "-0" + std::to_string(month) + "-" + std::to_string(day);
  }
  else if(month < 10 && day < 10) {
    startDate = std::to_string(year) + "-0" + std::to_string(month) + "-0" + std::to_string(day);
  }
}