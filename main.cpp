#include <iostream>
#include <string>
#include <vector>
#include <raylib.h>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <cctype>
#include <map>
#include <cctype>
#include <algorithm>
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

    vector<Item*> items;
};

int W = 1500, H = 900;

int scrollOffset = 0;

void MainMenu(Font& OpenSans) {
    DrawTextEx(OpenSans, "Welcome to GME: Grocery Made Easy", {(W/2)-20 - MeasureTextEx(OpenSans, "Welcome to GME: Grocery Made Easy", 20, 0).x/2, 20}, 30, 2.0f, BLACK);
    DrawTextEx(OpenSans, "Press 'U' to login", {W/2 - MeasureTextEx(OpenSans, "Press 'U' to login as a user", 20, 0).x/2, static_cast<float>(H/2)}, 20, 2.0f, BLACK);
    DrawTextEx(OpenSans, "Press 'S' to sign up!", {W/2 - MeasureTextEx(OpenSans, "Press 'R' to login as a rider", 20, 0).x/2, static_cast<float>(H/2)+60}, 20, 2.0f, BLACK);
}

// Function to draw the homepage for users
void UserHomePage(const std::vector<Item>& items, Font& OpenSans) {
    DrawTextEx(OpenSans, "Welcome to GME: Grocery Made Easy", {W/2 - MeasureTextEx(OpenSans, "Welcome to GME: Grocery Made Easy", 30, 0).x/2, 20}, 30, 2.0f, BLACK);
    for (size_t i = 0; i < items.size(); i++) {
        DrawTextEx(OpenSans, items[i].name.c_str(), {W/2 - MeasureTextEx(OpenSans, items[i].name.c_str(), 30, 0).x/2, static_cast<float>(100 + i*50 - scrollOffset)}, 30, 2.0f, BLACK);
    }
}

bool LoginPage(const map<string,string>& users, Font &OpenSans) {
    string usernameInput = "";
    string passwordInput = "";
    Rectangle usernameRec = { 200, 200, 300, 50 };
    Rectangle passwordRec = { 200, 300, 300, 50 };

    while (!WindowShouldClose()) {
        // Update
        int key = GetCharPressed();

        // Check if a key has been pressed
        if ((key >= 48 && key <= 57) || (key >= 65 && key <= 90) || (key >= 97 && key <= 122)) {
            if (CheckCollisionPointRec(GetMousePosition(), usernameRec) && usernameInput.length() < 10) {
                usernameInput += (char)key;
            } else if (CheckCollisionPointRec(GetMousePosition(), passwordRec) && passwordInput.length() < 10) {
                passwordInput += (char)key;
            }
        }
        // Check if ESC key has been pressed
        if (IsKeyPressed(KEY_ESCAPE)) {
            return false;
        }
        // Check if Enter key has been pressed
        if (IsKeyPressed(KEY_ENTER) && usernameInput.length() > 0 && passwordInput.length() > 0) {
            auto it = users.find(usernameInput);
            if (it == users.end()) {
                DrawTextEx(OpenSans, "Username does not exist. Please try again.", {200, 500}, 30, 2.0f, RED);
                usernameInput = "";
                passwordInput = "";
                continue;
            }
            if (it->second != passwordInput) {
                DrawTextEx(OpenSans, "Incorrect password. Please try again.", {200, 500}, 30, 2.0f, RED);
                passwordInput = "";
                continue;
            }
            return true;
        }

        // Draw
        BeginDrawing();
        ClearBackground(RAYWHITE);

        // Draw the textboxes
        DrawRectangleLines(usernameRec.x, usernameRec.y, usernameRec.width, usernameRec.height, DARKGRAY);
        DrawRectangleLines(passwordRec.x, passwordRec.y, passwordRec.width, passwordRec.height, DARKGRAY);

        // Draw the input text
        DrawText(usernameInput.c_str(), usernameRec.x + 5, usernameRec.y + 5, 40, BLACK);
        DrawText(passwordInput.c_str(), passwordRec.x + 5, passwordRec.y + 5, 40, BLACK);

        DrawTextEx(OpenSans, "Enter your username:", {usernameRec.x - MeasureTextEx(OpenSans, "Enter your username:", 30, 0).x, usernameRec.y - 40}, 30, 2.0f, BLACK);
        DrawTextEx(OpenSans, "Enter your password:", {passwordRec.x - MeasureTextEx(OpenSans, "Enter your password:", 30, 0).x, passwordRec.y - 40}, 30, 2.0f, BLACK);


//        if (IsKeyPressed(KEY_ENTER)) {
//            auto it = users.find(usernameInput);
//            if (it == users.end()) {
//                DrawTextEx(OpenSans, "Username does not exist. Please try again.", {200, 500}, 30, 2.0f, RED);
//                usernameInput = "";
//                passwordInput = "";
//                return false;
//            }
//            if (it->second != passwordInput) {
//                DrawTextEx(OpenSans, "Incorrect password. Please try again.", {200, 500}, 30, 2.0f, RED);
//                passwordInput = "";
//                return false;
//            }
//            return true;
//        }
        EndDrawing();
    }
    return false;
}

void OrderConfirmationPage(Cart& cart, Font& OpenSans) {
    // Generate a random 5-digit order number
    int orderNumber = rand() % 90000 + 10000;

    // Draw a rectangle in the middle of the screen
    Rectangle rec = { static_cast<float>(W/2 - 300), static_cast<float>(H/2 - 200), 600, 400 };
    DrawRectangleRec(rec, LIGHTGRAY);

    // Draw the text inside the rectangle
    int y = rec.y + 20;
    DrawTextEx(OpenSans, ("Order Confirmation - Order Number: " + std::to_string(orderNumber)).c_str(), {rec.x + rec.width/2 - MeasureTextEx(OpenSans, ("Order Confirmation - Order Number: " + std::to_string(orderNumber)).c_str(), 30, 0).x/2, static_cast<float>(y)}, 30, 2.0f, BLACK);
    y += 40;
    for (const auto& item : cart.items) {
        DrawTextEx(OpenSans, (item->name + ": " + std::to_string(item->price)).c_str(), {rec.x + rec.width/2 - MeasureTextEx(OpenSans, (item->name + ": " + std::to_string(item->price)).c_str(), 30, 0).x/2, static_cast<float>(y)}, 30, 2.0f, BLACK);
        y += 40;
    }
    DrawTextEx(OpenSans, ("Total: " + std::to_string(cart.calculateTotal())).c_str(), {rec.x + rec.width/2 - MeasureTextEx(OpenSans, ("Total: " + std::to_string(cart.calculateTotal())).c_str(), 30, 0).x/2, static_cast<float>(y)}, 30, 2.0f, BLACK);
    y += 40;
    DrawTextEx(OpenSans, "✔", {rec.x + rec.width/2 - MeasureTextEx(OpenSans, "✔", 30, 0).x/2, static_cast<float>(y)}, 30, 2.0f, BLACK);
    y += 40;
    DrawTextEx(OpenSans, "Thank you for your order!", {rec.x + rec.width/2 - MeasureTextEx(OpenSans, "Thank you for your order!", 30, 0).x/2, static_cast<float>(y)}, 30, 2.0f, BLACK);
}

bool isValidPassword(const string & password){
    return password.length() < 5;
}

bool isValidUsername(const string& username) {
    return username.length() >= 5;
}

void createSignUpPage(map<string, string>& users, Font& OpenSans) {
    string usernameInput = "";
    string passwordInput = "";
    Rectangle usernameRec = { static_cast<float>(W/2 - 150), static_cast<float>(H/2 - 100), 300, 50 };
    Rectangle passwordRec = { static_cast<float>(W/2 - 150), static_cast<float>(H/2), 300, 50 };

    while (!WindowShouldClose()) {
        // Update
        int key = GetCharPressed();

        // Check if a key has been pressed
        if ((key >= 48 && key <= 57) || (key >= 65 && key <= 90) || (key >= 97 && key <= 122)) {
            if (CheckCollisionPointRec(GetMousePosition(), usernameRec) && usernameInput.length() < 10) {
                usernameInput += (char) key;
            } else if (CheckCollisionPointRec(GetMousePosition(), passwordRec) && passwordInput.length() < 10) {
                passwordInput += (char) key;
            }
        }

        // Draw
        BeginDrawing();
        ClearBackground(RAYWHITE);

        // Draw the textboxes
        DrawRectangleLines(usernameRec.x, usernameRec.y, usernameRec.width, usernameRec.height, DARKGRAY);
        DrawRectangleLines(passwordRec.x, passwordRec.y, passwordRec.width, passwordRec.height, DARKGRAY);

        // Draw the input text
        DrawTextEx(OpenSans, "Enter a username:",
                   {usernameRec.x - MeasureTextEx(OpenSans, "Enter a username:", 30, 0).x, usernameRec.y - 40}, 30,
                   2.0f, BLACK);
        DrawTextEx(OpenSans, "Enter a password:",
                   {passwordRec.x - MeasureTextEx(OpenSans, "Enter a password:", 30, 0).x, passwordRec.y - 40}, 30,
                   2.0f, BLACK);
        DrawText(usernameInput.c_str(), usernameRec.x + 5, usernameRec.y + 5, 40, BLACK);
        DrawText(passwordInput.c_str(), passwordRec.x + 5, passwordRec.y + 5, 40, BLACK);

        EndDrawing();


        if (IsKeyPressed(KEY_ENTER)) {
            if (users.find(usernameInput) != users.end()) {
                DrawTextEx(OpenSans, "Username already exists. Please try again.", {200, 500}, 30, 2.0f, RED);
                usernameInput = "";
                passwordInput = "";
                continue;
            }

            if (!isValidUsername(usernameInput)) {
                DrawTextEx(OpenSans, "Username must be at least 5 characters long.", {200, 500}, 30, 2.0f, RED);
                usernameInput = "";
                passwordInput = "";
                continue;
            }
            if (!isValidPassword(passwordInput)) {
                DrawTextEx(OpenSans,
                           "Password must be at least 5 characters long and contain digits and letters only.",
                           {200, 500}, 30, 2.0f, RED);
                passwordInput = "";
                continue;
            }
            users.insert({usernameInput, passwordInput});
            DrawTextEx(OpenSans, "Account created successfully!", {200, 500}, 30, 2.0f, GREEN);
            break; // Exit the loop
        }
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
//  RIDER_HOME_PAGE,

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

        map<string,string> users;   //TODO: Update this
        ifstream file2("users.csv");
        string line2;
        while (getline(file2, line2)) {
            vector<string> parts = split(line2, ',');
            users.insert({parts[0], parts[1]});
        }
        Cart cart;

        AppState state = MAIN_MENU;
        Font OpenSans = LoadFont("resources/fonts/OpenSans_Condensed-SemiBold.ttf"); // Replace with your font file
        while (!WindowShouldClose())    // Detect window close button or ESC key
        {
            // Update
            if (IsKeyPressed(KEY_U)) {
                if(LoginPage(users, OpenSans)) {
                    state = USER_HOME_PAGE;
                }
            }
            else if (IsKeyPressed(KEY_S)) {
                if(createSignUpPage(users, OpenSans)) {
                    state = USER_HOME_PAGE;
                }
            }
            if(state == LOGIN_PAGE && IsKeyPressed(KEY_S)) {
                state = SIGNUP_PAGE;
            }
            if(state == SIGNUP_PAGE && IsKeyPressed(KEY_U)) {
                state = LOGIN_PAGE;
            }
            if (state == LOGIN_PAGE && !LoginPage(users, OpenSans)) {
                CloseWindow();  // Close the window when LoginPage returns false
                break;  // Break the loop to end the program
            }

//            } else if (IsKeyPressed(KEY_R)) {
//                state = RIDER_HOME_PAGE;
//                // TODO: Change state to rider homepage
//            }
                if (GetMouseWheelMove() > 0) {
                    if (scrollOffset > 0) scrollOffset -= 20;
                }
                if (GetMouseWheelMove() < 0) {
                    if (scrollOffset < (items.size() * 60 - H)) scrollOffset += 20;
                }
                if (IsKeyDown(KEY_DOWN)) {
                    if (scrollOffset < (items.size() * 60 - H)) scrollOffset += 10;
                }
                if (IsKeyDown(KEY_UP)) {
                    if (scrollOffset > 0) scrollOffset -= 10;
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
                    case ORDER_CONFIRMATION_PAGE:
                        OrderConfirmationPage(cart, OpenSans);
                        break;
                    case LOGIN_PAGE:
                        if(LoginPage(users, OpenSans)) state = USER_HOME_PAGE;
                        else state = MAIN_MENU;
                        // Handle other states as needed
                        break;
                    case SIGNUP_PAGE:
                        createSignUpPage(users, OpenSans);
                }

                EndDrawing();
            }
            UnloadFont(OpenSans);
            CloseWindow();
        }
        catch (const CustomException &e) {
            cout << e.what() << endl;
        }
        catch(const InvalidNumberException &e) {
            cout << e.what() << endl;
        }
        catch (const std::exception &e) {
            cout << "Caught an exception: " << e.what() << endl;
        }
        catch (...) {
            cout << "Caught an unknown exception." << endl;
        }

        return 0;
    }