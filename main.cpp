#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <stdexcept>
using namespace std;

class Item;

class Category {
protected:
    string name;
    vector<Item*> itemsOfCategory;
public:
    Category(string n) {
        bool validName;
        do {    // Invalid name check
            validName = true;
            for(int size_t = 0; size_t < n.length(); size_t++) {
                if(n[size_t] != ' ') {
                    if(n[size_t] < 65 || n[size_t] > 122) {
                        cout << "Invalid name. Please enter the name of "<< n << " again: " << endl;
                        cin >> n;
                        validName = false;
                        break;
                    }
                }
            }
        } while (!validName);
        name = n;
    }
    void CategoryPush(Item* i) {
        itemsOfCategory.push_back(i);
    }
};

class Item : public Category {
public:
    string name;
    string brandName;
    int productID;
    int quantity;
    int quantityCart = 0;
    float price;
    bool inStock;
    Item(string n, string bN, int q, float p, bool iS, Category& c, int ID) : Category(name) {
        while (ID <= 0 || ID >= 100) { // Invalid Product ID check
            cout << "Invalid ID. Please enter the product ID of "<< n << " again: " << endl;
            cin >> ID;
        }
        productID = ID;
        bool validName; // Invalid name check
        do {
            validName = true;
            for(int size_t = 0; size_t < n.length(); size_t++) {
                if(n[size_t] != ' ') {
                    if(n[size_t] < 65 || n[size_t] > 122) {
                        cout << "Invalid name. Please enter the name of "<< n << " again: " << endl;
                        cin >> n;
                        validName = false;
                        break;
                    }
                }
            }
        } while (!validName);
        name = n;

        bool validBrandName; // Invalid brand name check
        do {
            validBrandName = true;
            for(int size_t = 0; size_t < bN.length(); size_t++) {
                if(bN[size_t] != ' ') {
                    if(bN[size_t] < 65 || bN[size_t] > 122) {
                        cout << "Invalid brand name. Please enter the name of "<< n << " again: " << endl;
                        getline(cin, bN);
                        validBrandName = false;
                        break;
                    }
                }
            }
        } while (!validBrandName);
        brandName = bN;
        while (q < 0) { // Invalid quantity check
            cout << "Invalid quantity. Please enter the quantity of "<< n << " again: " << endl;
            cin >> q;
        }
        quantity = q;
        while (p <= 0) {    // Invalid price check
            cout << "Invalid price. Please enter the price of "<< n << " again: " << endl;
            cin >> p;
        }
        price = p;
        inStock = iS;

        c.CategoryPush(this);
    }
    void display() {
        cout << "Product ID: " << productID << "\nItem: " << name << "\nBrand: " << brandName << "\nQuantity: " << quantity << "\nPrice: " << price << "\nIn stock: " << boolalpha << inStock  << endl << endl;
    }
    void cartDisplay() {
        cout << "Product ID: " << productID << "\nItem: " << name << "\nBrand: " << brandName << "\nQuantity: " << quantityCart << "\nPrice: " << price << endl << endl;
    }
};

class Cart {
    vector<Item*> items;
    public:
    float calculateTotal() {
        float total = 0;
        for(int i = 0; i< items.size(); i++) {
            if(items[i]->quantityCart > 1) {
                total += items[i]->price * items[i]->quantityCart;
            }
            else {
                total += items[i]->price;
            }
        }
        return total;
    }

    void addItem(Item* i) {
        if(i->inStock) {
            if(items.size() == 0) {
                items.push_back(i);
                i->quantityCart++;
                i->quantity--;
                if (i->quantity == 0) {
                    i->inStock = false;
                }
                return;
            }
            for (int p = 0; p < items.size(); p++) {
                if (items[p]->productID == i->productID) {
                    items[p]->quantityCart++;
                    i->quantity--;
                    if (i->quantity == 0) {
                        i->inStock = false;
                    }
                    return;
                }
            }
            items.push_back(i);
                    i->quantity--;
                    i->quantityCart++;
                    if (i->quantity == 0) {
                        i->inStock = false;
                    }
        }
        else {
            cout << "Item out of stock." << endl;
        }
    }
    void display() {
        cout << "\n\033[1mCart Details:\033[0m" << endl; // \033[1m starts bold text and \033[0m ends it
        for(int i = 0; i< items.size(); i++) {
            items[i]->cartDisplay();
        }
        cout << "Total: " << this->calculateTotal() << endl;
    }
    void removeItem(Item* item) {
        if(items.size() == 0) {
            cout << "Cart is empty." << endl;
            return;
        }
            for (int i = 0; i < items.size(); i++) {
                if (items[i]->productID == item->productID) {
                    if(items[i]->quantityCart > 1) {
                        items[i]->quantityCart--;
                        items[i]->quantity++;
                        item->inStock = true;
                    }
                    else {
                        item->quantity++;
                        item->quantityCart--;
                        item->inStock = true;
                        items.erase(items.begin() + i);
                        return;
                    }
                }
            }
    }
};


int main() {
    Category Electronics("Electronics");
    Category Drinks("Drinks");
    Item i1("Laptop For Alesh!!&", "Dell", 10, 1000, true, Electronics, -50); // Invalid ID check, Invalid name check
    Item i2("Soda", "Coca Cola", 2, 1, true, Drinks, 2);
    // This manual entry of items will be replaced by using fstream library and a .txt file.
    Cart cart;
    i1.display(); // When you click on an item.
    cart.addItem(&i1);
    cart.addItem(&i2);
    cart.addItem(&i2);
    cart.addItem(&i2); // Shows Item out of stock.
    i2.display();   // Shows the item details, with quantity as 0 and inStock as false.
    cart.display();
    cart.removeItem(&i1);
    cart.removeItem(&i2);
    cart.display();
    i1.display();
    i2.display();
    cout << "Total: " << cart.calculateTotal() << endl;

    return 0;
}
