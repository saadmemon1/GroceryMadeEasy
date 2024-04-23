#include <iostream>
#include <string>
#include <vector>
#include <raylib.h>
#include <fstream>
#include <sstream>
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
                if (name[i] != ' ' && name[i] != '|' && (name[i] < 65 || name[i] > 122) && name[i] != '.' && (name[i] < 48 || name[i] > 57)) {
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

int scrollOffset = 0;

void MainMenu(Font& OpenSans) {
    DrawTextEx(OpenSans, "Welcome to GME: Grocery Made Easy", {300 - MeasureTextEx(OpenSans, "Welcome to GME: Grocery Made Easy", 20, 0).x/2, 20}, 30, 2.0f, BLACK);
    DrawTextEx(OpenSans, "Press 'U' to login as a user", {W/2 - MeasureTextEx(OpenSans, "Press 'U' to login as a user", 20, 0).x/2, H/2}, 20, 2.0f, BLACK);
    DrawTextEx(OpenSans, "Press 'R' to login as a rider", {W/2 - MeasureTextEx(OpenSans, "Press 'R' to login as a rider", 20, 0).x/2, (H/2)+60}, 20, 2.0f, BLACK);
}

// Function to draw the homepage for users
void UserHomePage(const std::vector<Item>& items, Font& OpenSans) {
    DrawTextEx(OpenSans, "Welcome to GME: Grocery Made Easy", {W/2 - MeasureTextEx(OpenSans, "Welcome to GME: Grocery Made Easy", 30, 0).x/2, 20}, 30, 2.0f, BLACK);
    for (size_t i = 0; i < items.size(); i++) {
        DrawTextEx(OpenSans, items[i].name.c_str(), {W/2 - MeasureTextEx(OpenSans, items[i].name.c_str(), 30, 0).x/2, static_cast<float>(100 + i*50 - scrollOffset)}, 30, 2.0f, BLACK);
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

vector<string> split(const string &s, char delimiter) {
    vector<string> tokens;
    string token;
    istringstream tokenStream(s);
    while (getline(tokenStream, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}

string trim(const string &s) {
    auto start = s.begin();
    while (start != s.end() && isspace(*start)) {
        start++;
    }
    auto end = s.end();
    do {
        end--;
    } while (end != start && isspace(*end));
    return string(start, end + 1);
}


int main() {
    // TODO: Pictures of 189x189 icons for items
    // TODO: 147x147 for the category card, 80x80 img for the category icon png


    try {
    InitWindow(W, H, "GME: Grocery Made Easy");
    // This manual entry of items will be replaced by using fstream library and a .txt file.

        Category Electronics("Electronics");
        Category Dairy("Dairy");
        Category Healthcare("Healthcare");
        Category PetNeeds("Pet Needs");
        Category Pharmacy("Pharmacy");
        Category Skincare("Skincare");
        Category Household("Household and Cleaning Supplies");
        Category EverydayEssentials("Everyday Essentials");
        Category Beverages("Beverages");
        Category FrozenItems("Frozen Items");

        vector<Item> items;
        ifstream file("items.txt");
        string line;
        while (getline(file, line)) {
            vector<string> parts = split(line, ',');
            for (string &part : parts) {
                part = trim(part);
            }
            string name = parts[0];
            string brandName = parts[1];
            int quantity = stoi(parts[2]);
            int price = stoi(parts[3]);
            bool inStock = parts[4] == "true" ? true : false;
            Category category(parts[5]);
            int productID = stoi(parts[6]);
            Item item(name, brandName, quantity, price, inStock, category, productID);
            items.push_back(item);
        }

//        Item i1("Inspiron 3530 | Core i5 13th Gen 8GB 512GB SSD 15.6 FHD Windows 11 |1 Year Warranty|", "Dell", 2, 200000, true, Electronics, 1); // TODO: Check the price thing, if the price entered is invalid, it asks for price twice.
//        Item i2("Full Cream Milk | 250ml", "Olpers", 5, 73, true, Dairy, 2);
//        Item i3("Antiseptic Hand Sanitizer | 250ml", "Hemani", 10, 450, true, Healthcare, 3);
//        Item i4("Cat Food Sea Food | 450g", "Morsus", 3, 700, true, PetNeeds, 4);
//        Item i5("Paracetamol | 500mg", "GSK", 7, 32, true, Pharmacy, 5);
//        Item i6("Nivea Men Acne Oil Control Face Wash | 100g", "Nivea", 4, 1100, true, Skincare, 6);
//        Item i7("Dishwash Liquid With Real Lemon Juice  | Pouch | 450ml", "Finish", 6, 275, true, Household, 7);
//        Item i8("Maxob Toilet Tissue Roll | Single", "Rose Petal", 8, 120, true, EverydayEssentials, 8);
//        Item i9("Pepsi | 1.5L", "Pepsi", 9, 150, true, Beverages, 9);
//        Item i10("Frozen Chicken Nuggets | 500g", "Sabroso", 10, 1500, true, FrozenItems, 10);
//        Item i11("Soap | 100g", "Dove", 11, 250, true, Household, 11);
//        Item i12("Toothpaste | 100g", "Sensodyne", 12, 400, true, EverydayEssentials, 12);
//        Item i13("Milk Chocolate | 100g", "Cadbury", 13, 150, true, EverydayEssentials, 13);
//        Item i14("Biscuits | 200g", "Bake Parlor", 14, 100, true, EverydayEssentials, 14);
//        Item i15("Shampoo | 200ml", "Dove", 15, 300, true, Skincare, 15);
//        Item i16("Conditioner | 200ml", "Sun Silk", 16, 300, true, Skincare, 16);
//        Item i17("Toilet Cleaner | 500ml", "Harpic", 17, 500, true, Household, 17);
//        Item i18("Nutella Chocolate Spread | 200g", "Nutella", 18, 1000, true, EverydayEssentials, 18);
//        Item i19("Coca Cola | 1.5L", "Coca Cola", 19, 150, true, Beverages, 19);
//        Item i20("Frozen Chicken Fries | 500g", "Sabroso", 20, 1500, true, FrozenItems, 20);
//        Item i21("Handwash | 250ml", "Dettol", 21, 200, true, Healthcare, 21);
//        Item i22("Braun Bread | Medium", "Dawn", 22, 150, true, EverydayEssentials, 22);
//        Item i23("Mozarella Cheese | 200g", "Day Fresh", 23, 300, true, Dairy, 23);
//        Item i24("Peanut Butter | 200g", "American Garden", 24, 700, true, EverydayEssentials, 24);
//        Item i25("Lays | 200g", "Lays", 25, 200, true, EverydayEssentials, 25);
//        Item i26("Butter | 200g", "Blue Band", 26, 300, true, Dairy, 26);
//        Item i27("Eggs | 1 Dozen", "Farm Fresh", 27, 300, true, Dairy, 27);
//        Item i28("X3s True Wireless Earbuds | Bluetooth 5.0 | 20H Playtime | IPX5 |", "Edifier", 28, 5000, true, Electronics, 28);
//        Item i29("S24 Ultra Phone | 6GB 128GB | PTA Approved", "Samsung", 29, 500000, true, Electronics, 29);
//        Item i30("iPhone 15 Pro Max | 128GB | PTA Approved", "Apple", 30, 592000, true, Electronics, 30);
//        Item i31("iPhone 14 Pro Max | 256GB | PTA Approved", "Apple", 30, 392000, true, Electronics, 31);
//        Item i32("MacBook M1 | 256GB | Space Grey", "Apple", 32, 230000, true, Electronics, 32);
//        Item i33("iPad Pro 2022 | 128GB", "Apple", 33, 150000, true, Electronics, 33);
//        Item i34("AirPods Pro 2024 | Active Noise Cancellation | Wireless Charging Case", "Apple", 34, 50000, true, Electronics, 34);
//        Item i35("Apple Watch Series 7 | 45mm | GPS", "Apple", 35, 130000, true, Electronics, 35);
//        Item i36("Apple Watch SE | 40mm | GPS", "Apple", 36, 80000, true, Electronics, 36);
//        Item i37("Myteka Tablet | 1 Strip | 10mg", "Hilton Pharma", 37, 264, true, Pharmacy, 37);
//        Item i38("GSK Calpol 6 Plus Syrup | 90ml", "GSK", 38, 110, true, Pharmacy, 38);
//        Item i39("GSK Calpol 6 Plus Tablets | 10 Tablets", "GSK", 0, 50, false, Pharmacy, 39);
//        Item i40("Cornflakes | 500g", "Kelloggs", 40, 300, true, EverydayEssentials, 40);





        Cart cart;
        items[0].display();
//        cart.addItem(&i1);
//        cart.addItem(&i2);
//        cart.addItem(&i2);
//        cart.addItem(&i2); // Shows Item out of stock.
//        i2.display();   // Shows the item details, with quantity as 0 and inStock as false.
//        cart.display();
//        cart.removeItem(&i1);
//        cart.removeItem(&i2);
//        cart.display();
//        i1.display();
//        i2.display();

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
            if(IsKeyDown(KEY_DOWN)) {
                if(scrollOffset < (items.size() * 60 - H)) scrollOffset += 10;
            }
            if(IsKeyDown(KEY_UP)) {
                if(scrollOffset > 0) scrollOffset -= 10;
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
                    UserHomePage(items, OpenSans);
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
