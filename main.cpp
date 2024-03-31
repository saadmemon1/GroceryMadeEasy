#include <iostream>
#include <string>
#include <vector>
using namespace std;

class Item;

class Category {
protected:
    string name;
    vector<Item*> itemsOfCategory;
public:
    Category(string n) : name(n) {}
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
    Item(string n, string bN, int q, float p, bool iS, Category& c, int ID) : Category(c) , name(n), brandName(bN), quantity(q), price(p), inStock(iS), productID(ID) {
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
    Item i1("Laptop", "Dell", 10, 1000, true, Electronics, 1);
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
