#include <iostream>
#include <string>
#include <vector>
#include <fstream>
using namespace std;


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
        cout << "Product ID: " << productID << "Item: " << name << " Brand: " << brandName << " Quantity: " << quantity << " Price: " << price << " In Stock: " << inStock << "\n";
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
        cout << "\nCart Details: " << endl;
        for(int i = 0; i< items.size(); i++) {
            items[i]->cartDisplay();
        }
        cout << "\nTotal: " << this->calculateTotal() << endl;
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
    Category c1("Electronics");
    Category c2("Drinks");
    Item i1("Laptop", "Dell", 10, 1000, true, c1, 1);
    Item i2("Soda", "Coca Cola", 2, 1, true, c2, 2);
    Cart cart;
    cart.addItem(&i1);
    cart.addItem(&i2);
    cart.addItem(&i2);
    cart.addItem(&i2);
    cart.display();
//    cout << "Total: " << cart.calculateTotal() << endl;

    return 0;
}
