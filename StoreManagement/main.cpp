#include "SalesManagement.h"
#include "Product.h"
#include "Inventory.h"
#include "CustomerList.h"
#include "Customer.h"
#include <iostream>

//file Names
std::string InventoryFile  = "ProductsData.txt";
std::string CustomersFile = "CustomersData.txt";
std::string SalesFile     = "SalesData.txt";

void displayMainMenu();
void displayProductMenu();
void displayCustomerMenu();
void displaySalesDataMenu();
bool addCustomer(Inventory &inventory, SalesManagement &notebook, CustomerList &cList);
bool addProduct(Inventory &inventory);
bool addPurchase(Customer &customer, Inventory &inventory, SalesManagement &notebook, CustomerList &cList);
bool DisplayProduct (Inventory &inventory);
bool EditProduct(Inventory &inventory ,Product &product);
bool DisplayCustomer(CustomerList &cList);
bool EditCustomer(CustomerList cList, Customer customer);
bool calTurnover(SalesManagement &notebook);

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
      running = DisplayCustomer(cList);
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
    else if(command == "0") {std::system("CLS"); running = true;}
    else if(command == "00") {std::system("CLS"); running = false;}
    else {std::cout << "Invalid choice. Please try again.\n"; goto Scommand;}
  }
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
          << "0.Restart the App\n"
          << "00.Exit the App\n"
          << "Enter the Number of the action you want: ";
}

bool addCustomer(Inventory &inventory, SalesManagement &notebook, CustomerList &cList) {
  std::string command;
  label2:
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
          << "0.Restart the app.\n"
          << "00.Exit the app.\n";
  command2:
    std::cin >> command;
    if(command == "1") { 
      if(addPurchase(newcustomer, inventory, notebook, cList)) {return true;}
      else {return false;}
    } else if(command == "2") {std::system("CLS"); goto label2;
    } else if(command == "0") {std::system("CLS"); return true;
    } else if(command == "00") {std::system("CLS"); return false;
    } else {std::cout << "Invalid choice. Please try again.\n"; goto command2;}
}

bool addProduct(Inventory &inventory) {
  std::string command;
  label1:                                                                                                         
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
          << "00.Exit the app.\n";
    command1:
      std::cin >> command;
      if(command == "1") {std::system("CLS"); goto label1;}
      else if(command == "0") {std::system("CLS"); return true;}
      else if(command == "00") {std::system("CLS"); return false;}
      else {std::cout << "Invalid choice. Please try again.\n"; goto command1;}
}

bool addPurchase(Customer &customer, Inventory &inventory, SalesManagement &notebook, CustomerList &cList) {
  std::string command;
  purchase1:
    Order order;
std::cout << "Here you can record the purchases of the customer: " << customer.get_name() << ' ' << customer.get_family() << " Customer ID: " << customer.get_ID() << '\n'
          << "please provide the purchase's details:\n"
          << "Date of purchase(y-m-d\\e.g. 2024-4-5): "; std::cin >> order.date;
std::cout << "Product's ID: "; std::cin >> order.ID; 
    Product product = inventory.findProduct(order.ID);
std::cout << "Product's details:\n"<< "ID: " << product.get_ID() << '\t' << "Name: " << product.get_name() << '\t' 
          << "Price: " << product.get_price() << '\t' << "Quantity: " << product.get_quantity() << '\n'
          << "Amount of the order: "; amount: std::cin >> order.quantity; order.price = product.get_price();
    if(customer.buy(product, order)) { 
      cList.update(customer.get_cData());
      notebook.add(order);
      inventory.update(product);
 std::cout << "Purchase recorded successfully.\n"
           << "=================================\n"
           << "1.Add another purchase.\n"
           << "0.Return to the main menu.\n"
           << "00.Exit the App";
      command4:
      std::cin >> command;    
      if(command == "1") {std::system("CLS"); goto purchase1;}
      else if(command == "0") {std::system("CLS"); return true;}
      else if(command == "00") {std::system("CLS"); return false;}
      else {std::cout << "Invalid choice. Please try again.\n"; goto command4;}
    }
    else{
 std::cout << "Amount of the order exceedes the remaining quantity of this product in the inventory.\n"
           << "=================================\n"
           << "1.Edit amount of the order\n"
           << "2.Add another purchase\n"
           << "0.Return to the main menu\n"
           << "00.Exit the App";
    command3:
      std::cin >> command;    
      if(command == "1") {goto amount;}
      else if(command == "2") {std::system("CLS"); goto purchase1;}
      else if(command == "0") {std::system("CLS"); return true;}
      else if(command == "00") {std::system("CLS"); return false;}
      else {std::cout << "Invalid choice. Please try again.\n"; goto command3;}
    }
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
                      << "ID\tName\t\tPrice\tQuantity\n"
                      << product.get_ID() << '\t' << product.get_name() << "\t\t" << product.get_price() << '\t' << product.get_quantity() << '\n'
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
                      << "ID\tName\t\tPrice\tQuantity\n"
                      << product.get_ID() << '\t' << product.get_name() << "\t\t" << product.get_price() << '\t' << product.get_quantity() << '\n'
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
            << "Choose the parameter you want to edit:"
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
                        << "New Name: "; std::string name; std::cin>>name; product.set_name(name); inventory.update(product);
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
                        << "New Price: "; int price; std::cin>>price; product.set_price(price); inventory.update(product);
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
                        << "New Quantity: "; int quantity; std::cin>>quantity; product.set_quantity(quantity); inventory.update(product);
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

bool DisplayCustomer(CustomerList &cList) {
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
                      << "ID\tName\t\tFamily\t\tTotal\n"
                      << customer.get_ID() << '\t' << customer.get_name() << "\t\t" << customer.get_family() << "\t\t" << customer.get_total() << '\n'
                      << "=================================\n"
                      << "1.Edit this Customer\n"
                      << "2.Display this Customer's cart\n"
                      << "3.Search again\n"
                      << "0.Return to the main menu\n"
                      << "00.Exit the App\n"
                      << "Enter the number of the action you want: "; dpcommand3: std::cin>>command;
            if(command == "1") {
              //if(EditCustomer(cList, customer)) {return true;}
              //else {return false;}
            }
            else if(command == "2") {} //new function
            else if(command == "3") {goto Search;}
            else if(command == "0") {std::system("CLS"); return true;}
            else if(command == "00") {std::system("CLS"); return false;}
            else {std::cout << "Invalid choice. Please try again.\n"; goto dpcommand3;}
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
                      << "ID\tName\t\tfamily\t\tTotal\n"
                      << customer.get_ID() << '\t' << customer.get_name() << "\t\t" << customer.get_family() << "\t\t" << customer.get_total() << '\n'
                      << "=================================\n"
                      << "1.Edit this Customer\n"
                      << "2.Display this Customer's cart\n"
                      << "3.Search again\n"
                      << "0.Return to the main menu\n"
                      << "00.Exit the App\n"
                      << "Enter the number of the action you want: "; dpcommand4: std::cin>>command;
            if(command == "1") {
              //if(EditCustomer(cList, customer)) {return true;}
              //else {return false;}
            }
            else if(command == "2") {}
            else if(command == "3") {goto Search;}
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
            << "Choose the parameter you want to edit:"
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
                        << "New Name: "; std::string name; std::cin>>name; customer.set_name(name); CustomerData cd = customer.get_cData(); cList.update(cd); 
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
                        << "New Family name: "; std::string family; std::cin>>family; customer.set_family(family); CustomerData cd = customer.get_cData(); cList.update(cd);
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
                        << "New Total: "; int total; std::cin>>total; customer.set_total(total); CustomerData cd = customer.get_cData(); cList.update(cd);
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
  cal: //I can add a mode selector for this section. like last month or last year or...
    std::system("CLS");
    std::cout << "Here you can calculate the Turnover of your bussiness.\n"
              << "Please enter a date as the starting point of calculation(y-m-d\\e.g. 2024-04-05): "; std::string startDate; std::cin >> startDate;
    std::cout << "now enter a date as the ending point of calculation(y-m-d\\e.g. 2024-04-05): "; std:: string endDate; std::cin >> endDate;
    std::cout << "Your Turnover from " << startDate << " to " << endDate << " is: " << notebook.turnover(startDate, endDate) << '\n'
              << "=================================\n"
              << "1.Calculate Turnover for a different time frame.\n"
              << "0.Return to the main menu.\n"
              << "00.Exit the App.\n"
              << "Enter the Number of the action you wish to do: ";
    command4:
      std::string command;
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