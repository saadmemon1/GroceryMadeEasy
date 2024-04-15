#include <iostream>
#include <string>
#include <vector>
#include <raylib.h>
#include <fstream>
#include <stdexcept>
using namespace std;

class Item;

class Category {
protected:
    string name;
    vector<Item*> itemsOfCategory;
public:
    Category(const string& n) {
        validateName(n);
        name = n;
    }
    void validateName(const string& n) {
        for (size_t i = 0; i < n.length(); ++i) {
            if (n[i] != ' ' && (n[i] < 65 || n[i] > 122)) {
                throw runtime_error("Oops! Invalid name. Please enter alphabets only.");
            }
        }
    }

    void CategoryPush(Item& i) {
        itemsOfCategory.push_back(&i);
    }
};

class Item : public Category {
protected:
    void validateProductID(int ID) {
        if (ID <= 0 || ID >= 100) {
            throw runtime_error("Invalid product ID. Please enter a value between 1 and 99.");
        }
        productID = ID;
    }
public:
    string name;
    string brandName;
    int productID;
    int quantity;
    int quantityCart = 0;
    float price;
    bool inStock;
    Item(const string& n, const string& bN, int q, float p, bool iS, Category& c, int ID) : Category(n) {
        validateName(bN); // Reusing base class validation for brand name (inheriting)
        brandName = bN;
        validateName(n);
        name = n;
        validateProductID(ID);
        productID = ID;

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
        if(quantity == 0) inStock = false;
        c.CategoryPush(*this);
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
        for(size_t i = 0; i< items.size(); i++) {
            items[i]->cartDisplay();
        }
        cout << "Total: " << this->calculateTotal() << endl;
    }
    void removeItem(Item* item) {
        if(items.size() == 0) {
            cout << "Cart is empty." << endl;
            return;
        }
            for (size_t i = 0; i < items.size(); i++) {
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
const int W = 800, H = 800;

void MainMenu(Font& OpenSans) {
    DrawTextEx(OpenSans, "Welcome to GME: Grocery Made Easy", {W/3 - MeasureTextEx(OpenSans, "Welcome to GME: Grocery Made Easy", 20, 0).x/2, 20}, 30, 2.0f, BLACK);
    DrawTextEx(OpenSans, "Press 'U' to login as a user", {W/2 - MeasureTextEx(OpenSans, "Press 'U' to login as a user", 20, 0).x/2, H/2}, 20, 2.0f, BLACK);
    DrawTextEx(OpenSans, "Press 'R' to login as a rider", {W/2 - MeasureTextEx(OpenSans, "Press 'R' to login as a rider", 20, 0).x/2, (H/2)+60}, 20, 2.0f, BLACK);
}

// Function to draw the homepage for users
void UserHomePage(const std::vector<Item*>& items) {
    DrawText("Welcome to the Grocery Store!", W/2 - MeasureText("Welcome to the Grocery Store!", 20)/2, 20, 20, BLACK);
    for (size_t i = 0; i < items.size(); i++) {
        DrawText(items[i]->name.c_str(), 20, 60 + i * 40, 20, BLACK);
    }
}

enum AppState {
    MAIN_MENU,
    USER_HOME_PAGE,
    // Add other states as needed
};


int main() {    // TODO: ADD TRY AND CATCH FOR EXCEPTIONS!
    InitWindow(W, H, "GME: Grocery Made Easy");
    Category Electronics("Electronics");
    Category Drinks("Drinks");
    Item i1("Laptop For Alesh", "Dell", 10, 1000, true, Electronics, 50); // Invalid ID check, Invalid name check
    Item i2("Soda", "Coca Cola", 2, 1, true, Drinks, 2);
    // This manual entry of items will be replaced by using fstream library and a .txt file.
    Cart cart;
    AppState state = MAIN_MENU;
    Font OpenSans = LoadFont("resources/fonts/OpenSans_Regular.ttf"); // Replace with your font file
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        if (IsKeyPressed(KEY_U)) {
            state = USER_HOME_PAGE;
        } else if (IsKeyPressed(KEY_R)) {
            // TODO: Change state to rider homepage
        }

        // Draw
        BeginDrawing();

        ClearBackground(LIGHTGRAY);

        // Draw the appropriate screen based on the current state
        switch (state) {
            case MAIN_MENU:
                MainMenu(OpenSans);
                break;
            case USER_HOME_PAGE:
                UserHomePage({&i1, &i2});
                break;
                // Handle other states as needed
        }

        EndDrawing();
    }
    UnloadFont(OpenSans);
    CloseWindow();
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
