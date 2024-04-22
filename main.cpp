#include <iostream>
#include <string>
#include <vector>
#include <raylib.h>
#include <fstream>
#include <stdexcept>
#include <cctype>
using namespace std;

class InvalidNumberException : public exception {
public:
    string message;
    InvalidNumberException(const string& m) : message(m) {}

    const char* what() const noexcept override {
        return message.c_str();
    }
};

class CustomException : public exception {
public:
    string message;
    CustomException(string m) : message(m) {}

    const char* what() const noexcept override {
        return message.c_str();
    }
};

void validateNumbers(int& n, bool isQuantity = false, bool isPrice = false);

// Forward declaration for friend function
void validateName(string& name, bool isCategoryName = false, bool isBrandName = false);


class Item;

//void validateProductName(string& n) {
//    for (size_t i = 0; i < n.length(); ++i) {
//        if (n[i] != ' ' && (n[i] < 65 || n[i] > 122)) {
//            throw CustomException("Oops! Invalid Product! The product name is incorrect. Please enter alphabets only.");
//        }
//    }
//}

//void validateBrandName(string& n) {
//        for (size_t i = 0; i < n.length(); ++i) {
//            if (n[i] != ' ' && (n[i] < 65 || n[i] > 122)) {
//                throw CustomException("Oops! Invalid Product! The brand name is incorrect. Please enter alphabets only.");
//            }
//        }
//}


//void validateCategoryName(string& n) {
//        for (size_t i = 0; i < n.length(); ++i) {
//            if (n[i] != ' ' && (n[i] < 65 || n[i] > 122)) {
//                throw CustomException("Oops! Invalid Category! The category name is incorrect. Please enter alphabets only.");
//            }
//        }
//}



class Category {
protected:
    vector<Item*> itemsOfCategory;
public:
    string name;
    Category(const string& n) {
        string x = n;
        validateName(x, true); // Call friend function for category name validation
        name = x;

    }
    void CategoryPush(Item& i) {
        itemsOfCategory.push_back(&i);
    }
};

class Item : public Category {
public:
    string name;
    string brandName;
    int productID;
    int quantity;
    int quantityCart = 0;
    int price;
    bool inStock;
    string categoryName;
    Item(string n, string bN, int q, int p, bool iS, Category& c, int ID) : Category(c) {
        validateName(n); // Call friend function for item name validation
        name = n;
        validateName(bN, false, true); // Call friend function for brand name validation
        brandName = bN;
        validateNumbers(ID); // Call friend function for product ID validation
        productID = ID;
        validateNumbers(q, true); // Call friend function for quantity validation
        quantity = q;
        validateNumbers(p, false, true); // Call friend function for price validation
        price = p;
        inStock = iS;
        if(quantity == 0) inStock = false;
        c.CategoryPush(*this);
        categoryName = c.name;
    }
    void display() {
        cout << "Product ID: " << productID << "\nItem: " << name << "\nBrand: " << brandName << "\nCateogry: " << categoryName << "\nQuantity: " << quantity << "\nPrice: " << price << "\nIn stock: " << boolalpha << inStock  << endl << endl;
    }
    void cartDisplay() {
        cout << "Product ID: " << productID << "\nItem: " << name << "\nBrand: " << brandName << "\nQuantity: " << quantityCart << "\nPrice: " << price << endl << endl;
    }
};

void validateNumbers(int& n, bool isQuantity, bool isPrice) {
    while (true) {
        try {
            if (isQuantity) {
                if(n < 0) {
                    throw InvalidNumberException("Invalid Product! You have entered invalid quantity. Please enter a non-negative value.");
                }
            } else if (isPrice) {
                if(n <= 0) {
                    throw InvalidNumberException("Invalid Product! You have entered invalid price. Please enter a positive value.");
                }
            } else {
                if (n <= 0 || n >= 100) {
                    throw InvalidNumberException("Invalid Product! You have entered invalid product ID. Please enter a value between 1 and 99.");
                }
            }
            break; // If input is valid, break the loop
        }
        catch (const InvalidNumberException& e) {
            cout << e.what() << endl;
            cout << "Please enter the number again: ";
            cin.clear();
            while (cin.get() != '\n'); // ignore the rest of the line
            cin >> n;
            if (cin.fail()) {
                cin.clear();
                while (cin.get() != '\n'); // ignore the rest of the line
                cout << "Invalid input. Please enter a number: ";
            }
        }
    }
}


void validateName(string& name, bool isCategoryName, bool isBrandName) {
    while (true) {
        try {
            for (size_t i = 0; i < name.length(); ++i) {
                if (name[i] != ' ' && (name[i] < 65 || name[i] > 122)) {
                    if (isCategoryName) {
                        throw CustomException("Invalid Product! You have entered invalid category name. Please enter alphabets only.");
                    } else if (isBrandName) {
                        throw CustomException("Invalid Product! You have entered invalid brand name. Please enter alphabets only.");
                    } else {
                        throw CustomException("Invalid Product! You have entered invalid item name. Please enter alphabets only.");
                    }
                }
            }
            break; // If input is valid, break the loop
        } catch (const CustomException& e) {
            cout << e.what() << endl;
            cout << "Please enter the name again: ";
            cin.clear();
            cin >> name;
        }
    }
}


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
const int W = 1300, H = 900;

void MainMenu(Font& OpenSans) {
    DrawTextEx(OpenSans, "Welcome to GME: Grocery Made Easy", {300 - MeasureTextEx(OpenSans, "Welcome to GME: Grocery Made Easy", 20, 0).x/2, 20}, 30, 2.0f, BLACK);
    DrawTextEx(OpenSans, "Press 'U' to login as a user", {W/2 - MeasureTextEx(OpenSans, "Press 'U' to login as a user", 20, 0).x/2, H/2}, 20, 2.0f, BLACK);
    DrawTextEx(OpenSans, "Press 'R' to login as a rider", {W/2 - MeasureTextEx(OpenSans, "Press 'R' to login as a rider", 20, 0).x/2, (H/2)+60}, 20, 2.0f, BLACK);
}

// Function to draw the homepage for users
void UserHomePage(const std::vector<Item*>& items, Font& OpenSans) {
    DrawTextEx(OpenSans, "Welcome to GME: Grocery Made Easy", {W/2 - MeasureTextEx(OpenSans, "Welcome to GME: Grocery Made Easy", 30, 0).x/2, 20}, 30, 2.0f, BLACK);
    for (size_t i = 0; i < items.size(); i++) {
        DrawTextEx(OpenSans, items[i]->name.c_str(), {W/2 - MeasureTextEx(OpenSans, items[i]->name.c_str(), 30, 0).x/2, static_cast<float>(100 + i*50)}, 30, 2.0f, BLACK);
    }
}

enum AppState {
    MAIN_MENU,
    USER_HOME_PAGE,
    SIGNUP_PAGE,
    LOGIN_PAGE,
    ITEM_PAGE,
    CART_PAGE,
    CHECKOUT_PAGE,
    ORDER_CONFIRMATION_PAGE,
    RIDER_HOME_PAGE,

    // Add other states as needed
};


int main() {
    // TODO: Pictures of 189x189 icons for items
    // TODO: 147x147 for the category card, 80x80 img for the category icon png


    try {
    InitWindow(W, H, "GME: Grocery Made Easy");
    // This manual entry of items will be replaced by using fstream library and a .txt file.

        Category Electronics("Electronics");
        Category Drinks("Drinks");
        Item i1("Inspiron Laptop", "Dell", 2, 1000, true, Electronics, 50); // TODO: Check the price thing, if the price entered is invalid, it asks for price twice.
        Item i2("Soda", "Coca Cola", 2, 1, true, Drinks, 2);
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
        AppState state = MAIN_MENU;
        Font OpenSans = LoadFont("resources/fonts/OpenSans_Condensed-SemiBold.ttf"); // Replace with your font file
        while (!WindowShouldClose())    // Detect window close button or ESC key
        {
            // Update
            if (IsKeyPressed(KEY_U)) {
                state = USER_HOME_PAGE;
            } else if (IsKeyPressed(KEY_R)) {
                state = RIDER_HOME_PAGE;
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
                    UserHomePage({&i1, &i2}, OpenSans);
                    break;
                    // Handle other states as needed
            }

            EndDrawing();
        }
        UnloadFont(OpenSans);
        CloseWindow();
    }
    catch (const CustomException& e) {
        cout  << e.what() << endl;
    }
    catch(const InvalidNumberException& e) {
        cout << e.what() << endl;
    }
    catch (const std::exception& e) {
        cout << "Caught an exception: " << e.what() << endl;
    }
    catch (...) {
        cout << "Caught an unknown exception." << endl;
    }

    return 0;
}
