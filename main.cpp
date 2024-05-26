#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <cctype>
#include <map>
#include <cctype>
#include <algorithm>
#include <memory>
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
        cout << "\n\033[1mCart Details:\033[0m" << endl;
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

//int W = 1500, H = 900;
//
//int scrollOffset = 0;

//void MainMenu(Font& OpenSans) {
//    DrawTextEx(OpenSans, "Welcome to GME: Grocery Made Easy", {(W/2)-20 - MeasureTextEx(OpenSans, "Welcome to GME: Grocery Made Easy", 20, 0).x/2, 20}, 30, 2.0f, BLACK);
//    DrawTextEx(OpenSans, "Press 'U' to login", {W/2 - MeasureTextEx(OpenSans, "Press 'U' to login as a user", 20, 0).x/2, static_cast<float>(H/2)}, 20, 2.0f, BLACK);
//    DrawTextEx(OpenSans, "Press 'S' to sign up", {W/2 - MeasureTextEx(OpenSans, "Press 'R' to login as a rider", 20, 0).x/2, static_cast<float>(H/2)+60}, 20, 2.0f, BLACK);
//}

void UserHomePage(const std::vector<Item*>& items) {
//    DrawTextEx(OpenSans, "Welcome to GME: Grocery Made Easy", {W/2 - MeasureTextEx(OpenSans, "Welcome to GME: Grocery Made Easy", 30, 0).x/2, 20}, 30, 2.0f, BLACK);

    int width = 250;
    int height = 250; // Adjust as needed
    int spacing = 100;
    int itemsperRow = 3;
    int rows = (items.size() + itemsperRow - 1) / itemsperRow;
}
//bool LoginPage(const map<string,string>& users, Font &OpenSans) {
//    string usernameInput = "";
//    string passwordInput = "";
//    bool error = false;
//    string errorMessage = "";
//    Rectangle usernameRec = { 200, 200, 300, 50 };
//    Rectangle passwordRec = { 200, 300, 300, 50 };
//
//    while (!WindowShouldClose()) {
//        // Update
//        int key = GetCharPressed();
//
//        // Check if a key has been pressed
//        if ((key >= 48 && key <= 57) || (key >= 65 && key <= 90) || (key >= 97 && key <= 122)) {
//            if (CheckCollisionPointRec(GetMousePosition(), usernameRec) && usernameInput.length() < 10) {
//                usernameInput += (char)key;
//            } else if (CheckCollisionPointRec(GetMousePosition(), passwordRec) && passwordInput.length() < 10) {
//                passwordInput += (char)key;
//            }
//        }
//        // Check if ESC key has been pressed
//        if (IsKeyPressed(KEY_ESCAPE)) {
//            return false;
//        }
//
//
//        // Draw
//        BeginDrawing();
//        ClearBackground(WHITE);
//
//        // Draw the textboxes
//        DrawRectangleLines(usernameRec.x, usernameRec.y, usernameRec.width, usernameRec.height, DARKGRAY);
//        DrawRectangleLines(passwordRec.x, passwordRec.y, passwordRec.width, passwordRec.height, DARKGRAY);
//
//        // Draw the input text
//        DrawTextEx(OpenSans, usernameInput.c_str(), {usernameRec.x + 5, usernameRec.y + 5}, 35, 2.0f, BLACK);
//        string passwordHidden = "";
//        for (size_t i = 0; i < passwordInput.length(); i++) {
//            passwordHidden += "*";
//        }
//        DrawTextEx(OpenSans, passwordHidden.c_str(), {passwordRec.x + 5, passwordRec.y + 5}, 35, 2.0f, BLACK);
//
//        DrawTextEx(OpenSans, "Login", {W/2 - MeasureTextEx(OpenSans, "Login", 50, 0).x/2, 100}, 50, 2.0f, BLACK);
//        DrawTextEx(OpenSans, "GME: Grocery Made Easy", {W/2 - MeasureTextEx(OpenSans, "GME: Grocery Made Easy", 30, 0).x/2, 20}, 30, 2.0f, BLACK);
//        DrawTextEx(OpenSans, "Enter your username:", {usernameRec.x - MeasureTextEx(OpenSans, "Enter your username:", 30, 0).x, usernameRec.y - 40}, 30, 2.0f, BLACK);
//        DrawTextEx(OpenSans, "Enter your password:", {passwordRec.x - MeasureTextEx(OpenSans, "Enter your password:", 30, 0).x, passwordRec.y - 40}, 30, 2.0f, BLACK);
//
//        if(error) {
//            DrawTextEx(OpenSans, errorMessage.c_str(), {200, 500}, 30, 2.0f, RED);
//        }
//        // Check if Enter key has been pressed
//        if (IsKeyPressed(KEY_ENTER) && usernameInput.length() > 0 && passwordInput.length() > 0) {
//            auto it = users.find(usernameInput);
//            if (it == users.end()) {
////                DrawTextEx(OpenSans, "Username does not exist. Please try again.", {200, 500}, 30, 2.0f, RED);
//                usernameInput = "";
//                passwordInput = "";
//                error = true;
//                errorMessage = "Username does not exist. Please try again.";
//                continue;
//            }
//            if (it->second != passwordInput) {
////                DrawTextEx(OpenSans, "Incorrect password. Please try again.", {200, 500}, 30, 2.0f, RED);
//                passwordInput = "";
//                error = true;
//                errorMessage = "Incorrect password. Please try again.";
//                continue;
//            }
//            error = false;
//            EndDrawing();
//            return true;
//        }
//
//        EndDrawing();
//        if(IsKeyPressed(KEY_BACKSPACE)){
//            if(CheckCollisionPointRec(GetMousePosition(), usernameRec)) {
//                if (usernameInput.length() > 0) {
//                    usernameInput.pop_back();
//                }
//            }
//            if(CheckCollisionPointRec(GetMousePosition(), passwordRec)) {
//                if (passwordInput.length() > 0) {
//                    passwordInput.pop_back();
//                }
//            }
//        }
//    }
//    return false;
//}

void OrderConfirmationPage(Cart& cart) {

    int orderNumber = rand() % 90000 + 10000;
    cout<<"Order Confirmation - Order Number: " <<orderNumber<<endl;
    for (const auto& item : cart.items) {
        cout << item->name << ": " << item->price << endl;
    }
    cout << "Total: " << cart.calculateTotal() << endl;
    cout << "\n\033[1m✔\033[0m" << endl;
    cout << "Thank you for placing your order!" << endl;
}

bool isValidPassword(const string & password){
    return password.length() > 5;
}

bool isValidUsername(const string& username) {
    return username.length() >= 5;
}

//bool createSignUpPage(map<string, string>& users, Font& OpenSans) {
//    string usernameInput = "";
//    string passwordInput = "";
//    bool error = false;
//    string message = "";    // For errors
//    int framesDelay = 0;
//    Rectangle usernameRec = { static_cast<float>(W/2 - 150), static_cast<float>(H/2 - 100), 300, 50 };
//    Rectangle passwordRec = { static_cast<float>(W/2 - 150), static_cast<float>(H/2), 300, 50 };
//
//    while (!WindowShouldClose()) {
//        // Update
//        int key = GetCharPressed();
//
//        // Check if a key has been pressed
//        if ((key >= 48 && key <= 57) || (key >= 65 && key <= 90) || (key >= 97 && key <= 122)) {
//            if (CheckCollisionPointRec(GetMousePosition(), usernameRec) && usernameInput.length() < 10) {
//                usernameInput += (char) key;
//            } else if (CheckCollisionPointRec(GetMousePosition(), passwordRec) && passwordInput.length() < 10) {
//                passwordInput += (char) key;
//            }
//        }
//        if (IsKeyPressed(KEY_ESCAPE)) {
//            return false;
//        }
//        // Draw
//        BeginDrawing();
//        ClearBackground(RAYWHITE);
//
//        // Draw the textboxes
//        DrawRectangleLines(usernameRec.x, usernameRec.y, usernameRec.width, usernameRec.height, DARKGRAY);
//        DrawRectangleLines(passwordRec.x, passwordRec.y, passwordRec.width, passwordRec.height, DARKGRAY);
//
//        // Draw the input text
//        DrawTextEx(OpenSans, "Enter a username:",
//                   {usernameRec.x - MeasureTextEx(OpenSans, "Enter a username:", 30, 0).x, usernameRec.y - 40}, 30,
//                   2.0f, BLACK);
//        DrawTextEx(OpenSans, "Enter a password:",
//                   {passwordRec.x - MeasureTextEx(OpenSans, "Enter a password:", 30, 0).x, passwordRec.y - 40}, 30,
//                   2.0f, BLACK);
//        DrawTextEx(OpenSans, usernameInput.c_str(), {usernameRec.x + 5, usernameRec.y + 5}, 35, 2.0f, BLACK);
//        DrawTextEx(OpenSans, passwordInput.c_str(), {passwordRec.x + 5, passwordRec.y + 5}, 35, 2.0f, BLACK);
//        DrawTextEx(OpenSans, "Press Enter to create an account.", {W/2 - MeasureTextEx(OpenSans, "Press Enter to create an account.", 30, 0).x/2, 700}, 30, 2.0f, BLACK);
//        DrawTextEx(OpenSans, "Sign Up", {W/2 - MeasureTextEx(OpenSans, "Sign Up", 50, 0).x/2, 100}, 50, 2.0f, BLACK);
//        DrawTextEx(OpenSans, "GME: Grocery Made Easy", {W/2 - MeasureTextEx(OpenSans, "GME: Grocery Made Easy", 30, 0).x/2, 20}, 30, 2.0f, BLACK);
//        if(error) {
//            DrawTextEx(OpenSans, message.c_str(), {200, 500}, 30, 2.0f, RED);
//        }
//        DrawFPS(10,10);
//        if(framesDelay > 0) {
//            DrawTextEx(OpenSans, "Account created successfully!", {200, 500}, 30, 2.0f, GREEN);
//            framesDelay++;
//            if(framesDelay > 90) {
//                framesDelay = 0;
////                    EndDrawing();
//                return true; // Exit the loop
//            }
//        }
//
//        if (IsKeyPressed(KEY_ENTER) && usernameInput.length() > 0 && passwordInput.length() > 0) {
//            auto it = users.find(usernameInput);
//            if (it != users.end()) {
////                DrawTextEx(OpenSans, "Username already exists. Please try again.", {200, 500}, 30, 2.0f, RED);
//                usernameInput = "";
//                passwordInput = "";
//                message = "Username already exists. Please try again.";
//                error = true;
//                continue;
//            }
//
//            if (!isValidUsername(usernameInput)) {
////                DrawTextEx(OpenSans, "Username must be at least 5 characters long.", {200, 500}, 30, 2.0f, RED);
//                usernameInput = "";
//                passwordInput = "";
//                error = true;
//                message = "Username must be at least 5 characters long.";
//                continue;
//            }
//            if (!isValidPassword(passwordInput)) {
////                DrawTextEx(OpenSans,"Password must be at least 5 characters long and contain digits and letters only.",{200, 500}, 30, 2.0f, RED);
//                passwordInput = "";
//                error = true;
//                message = "Password must be at least 5 characters long and contain digits and letters only.";
//                continue;
//            }
//            error = false;
//            framesDelay = 1;
//            ofstream file2("usersdb.csv", ios_base::app);
//            file2 << "\n" << usernameInput << "," << passwordInput;
//            users.insert({usernameInput, passwordInput});
//        }
//
//
//        EndDrawing();
//
//        if(IsKeyPressed(KEY_BACKSPACE)){
//            if(CheckCollisionPointRec(GetMousePosition(), usernameRec)) {
//                if (usernameInput.length() > 0) {
//                    usernameInput.pop_back();
//                }
//            }
//            if(CheckCollisionPointRec(GetMousePosition(), passwordRec)) {
//                if (passwordInput.length() > 0) {
//                    passwordInput.pop_back();
//                }
//            }
//        }
//
//    }
//    return false;
//}

bool ItemPage(int productID, const vector<Item*>& items, Cart& c) {
    bool x = false;
    Item* item = nullptr;
    for(size_t i = 0; i < items.size(); i++) {
        auto s = items[i];
        if(s->productID == productID) {
            s->display();
            x = true;
            item = new Item(*s);
            break;
        }
    }
    if(item != nullptr) {
        cout << "\nDo you want to purchase this product? (Y/N)" << endl;
        char choice;
        cin >> choice;
        if (choice == 'Y' || choice == 'y') {
            cout << "Please enter the quantity you want to purchase: (Enter 0 to cancel)" << endl;
            int quantity;
            cin >> quantity;
            while(quantity < 0 || quantity > item->quantity) {
                cout << "Invalid quantity. Please enter a positive value and lower than quantity that we have in stock: ";
                cin >> quantity;
            }
                if (quantity == 0) {
                    cout << "Product not added to cart." << endl;
                    return false;
                } else if (quantity > 0 && quantity <= item->quantity) {
                    cout << "Product added to cart." << endl;
                    for (size_t i = 0; i < quantity; i++) {
                        c.addItem(item);
                    }
                    return true;
                } else {
                    cout << "Invalid quantity. Please enter a value less than or equal to the available quantity of "
                         << item->quantity << endl;
                    return false;
                }
        } else {
            cout << "Product not added to cart." << endl;
            return false;
        }
    }
    else {
        cout << "Product not found." << endl;
        return false;
    }

}

bool cart_display(Cart c) {
    c.display();
    cout << "Do you want to modify any product's quantity? (Y/N)" << endl;
    char choice;
    cin >> choice;
    while (choice == 'Y' || choice == 'y') {
        cout << "Please enter the product ID of the product you want to modify: ";
        int productID;
        cin >> productID;
        if(productID < 0) {
            cout << "Invalid product ID." << endl;
            return false;
        }
        for(auto it : c.items) {
            if(it->productID == productID) {
                int maximum = it->quantity + it->quantityCart;
                cout << "The quantity of " << it->name << " will be modified." << endl;
                cout << "We have " << it->quantity << " available in stock." << endl;
                cout << "Current quantity in cart: " << it->quantityCart << endl;
                cout << "The maximum quantity you can add is " << maximum << endl << endl;
                cout << "Please enter the new quantity: ";
                int quantity;
                cin >> quantity;
                while(quantity < 0) {
                    cout << "Invalid quantity. Please enter a positive value: ";
                    cin >> quantity;
                }
                if(quantity == 0) {
                    cout << "Product removed from cart." << endl;
                    c.removeItem(it);
                }
                if (quantity > 0 && quantity <= (it->quantityCart + it->quantity)) {
                    cout << "Product quantity updated." << endl;
                    it->quantityCart = quantity;
                    it->quantity = maximum - quantity;
                }
                else {
                    cout << "Invalid quantity. Please enter a value less than or equal to the available quantity of " << maximum << endl;
                }
            }
//            cout << "Product not found." << endl;
        }
        cout << "Would you like to modify any other product's quantity? (Y/N)" << endl;
        cin >> choice;
    }
    cout << "Would you like to proceed to checkout or continue shopping? (C/S)" << endl;
    cin >> choice;
    if(choice == 'C' || choice == 'c') {
        cout << "Proceeding to checkout." << endl;
        return true;
    }
    else {
        cout << "Continuing shopping." << endl;
        return false;
    }
}


//bool CheckoutPage(Cart& c) {
//    string addressInput = "";
//
//    while(true){
//        cout << "Please enter your address: ";
//        cin >> addressInput;
//        if(addressInput.length() < 20) {
//            cout << "Invalid address. Please enter a valid address." << endl;
//            continue;
//        }
//        cout << "Address saved successfully." << endl;
//        break;
//
//    }
//}
bool CheckoutPage(Cart& c) {
    string addressInput = "";
    c.display();


    while (true) {
        cout << "Enter your address (or type 'X' to cancel): ";
        cin.ignore();
        getline(cin, addressInput);


        if (addressInput == "x" || addressInput == "X") {
            return false;
        }
        if (addressInput.length() > 0) {
            cout << "Your order will be delivered in 3-5 working days." << endl;
            break;
        } else {
            cout << "Address cannot be empty. Please try again." << endl;
        }
    }
    while (true) {
        cout << "\nEnter mode of payment <COD> for Cash on Delivery, <CC> for Credit Card): ";
        string  paymentMode;
        cin >> paymentMode;

        if (paymentMode == "COD") {
            cout << "You have selected Cash on Delivery." << endl;
            cout<<"Please keep the exact amount ready for the delivery person."<<endl;
            break;
        } else if (paymentMode == "CC") {
            cout << "You have selected Credit Card." << endl;
            cout << "Enter card holder name: ";
            string cardHolderName;
            cin.ignore();
            getline(cin, cardHolderName);


            cout << "Enter month of card expiration date: (MM) ";
            int month;
            cin >> month;
            cout << "\nEnter year of card expiration date: (YYYY)";
            int year;
            cin >> year;
            if (month<01 || month>12) {
                cout << "Invalid month. Please try again." << endl;
                continue;
            }
            if (year<2024 || year>2030) {
                cout << "Invalid year. Please try again." << endl;
                continue;
            }
            if(year==2024 && month <=06) {
                cout<<"Card is expired or will expire soon. Please try again."<<endl;
                continue;
            }

            cout << "Enter CVV: ";
            int cvv;
            cin >> cvv;
            if (cvv >= 100 && cvv <= 999) {
                cout << "Payment successful." << endl;
                break;
            } else {
                cout << "Invalid CVV. Please try again." << endl;
            }
        } else {
            cout << "Invalid option. Please try again." << endl;
        }
    }
    return true;
}

enum AppState {
    MAIN_MENU,
    USER_HOME_PAGE, // TODO: Buttons for each item to view its details
    SIGNUP_PAGE,
    LOGIN_PAGE,
    ITEM_PAGE,
    CART_PAGE,  // TODO: Buttons for inc/dec quantity, image etc, total price
    // TODO: Save cart for each user (IMPORTANT!)
    CHECKOUT_PAGE,
    ORDER_CONFIRMATION_PAGE,

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

void loadCart(const string& username, const vector<Item*>& items, Cart& cart) {
    ifstream cartdb("cartdb.csv");
    if (!cartdb) {
        // No cart file for this user, so just return
        return;
    }
    string line;
    while (getline(cartdb, line)) {
        vector<string> parts = split(line, ',');
        if (parts[0] == username) {
            for (size_t i = 1; i < parts.size(); i += 2) {
                int productID = stoi(parts[i]);
                int quantity = stoi(parts[i + 1]);
                for (const auto& item : items) {
                    if (item->productID == productID) {
                        for (size_t j = 0; j < quantity; j++) {
                            cart.addItem(new Item(*item));
                        }
                        break;
                    }
                }
            }
            break;
        }
    }
    cartdb.close();
}

void saveCart(const Cart& cart, const string& username) {
    ifstream file("cartdb.csv", ios_base::app);
    string line;
    while(getline(file, line)) {
        vector<string> parts = split(line, ',');
        if(parts[0] == username) {
            file.close();
            ofstream file2("cartdb.csv",ios_base::app);
            for(const auto& item: cart.items) {
                file2 << item->productID << "," << item->quantityCart;
            }
            file2 << "\n";
            file.close();
            return;
        }
    }
    file.close();
    ofstream file1("cartdb.csv", ios_base::app);
    file1 << username << ",";
    for (const auto& item : cart.items) {
        file1 << item->productID << "," << item->quantityCart;
    }
    file1 << "\n";
    file.close();
}

void DeleteCart(const string& username) {
    ifstream file("cartdb.csv");
    ofstream temp("temp.csv");
    string line;
    while (getline(file, line)) {
        vector<string> parts = split(line, ',');
        if (parts[0] != username) {
            temp << line << "\n";
        }
    }
    file.close();
    temp.close();
    remove("cartdb.csv");
    rename("temp.csv", "cartdb.csv");
}



int main() {
    map<string, string> users;
    map<string, Cart> userCarts;

    ifstream file2("usersdb.csv");
    string line2;
    while (getline(file2, line2)) {
        vector<string> parts = split(line2, ',');
        users.insert({parts[0], parts[1]});
    }
    file2.close();


    char choice;
   string username, password;
    cout << "\n\033[1mWelcome to GME: Grocery Made Easy\033[0m\n" << endl;
    cout << "Please login or sign up to continue." << endl;

    while(true) {
        cout << "Enter L to login or S to sign up: ";
        cin >> choice;
        choice = tolower(choice);
        if (choice == 'l' || choice == 'L' || choice == 's' || choice == 'S') {
            break;
        }
    }
    while (true) {
        if (choice == 'l') {
            cout << "Enter your username: ";
            cin >> username;
            cout << "Enter your password: ";
            cin >> password;

            auto it = users.find(username);
            if (it != users.end() && it->second == password) {
                cout << "Login successful!\n";
                break;
            } else {
                cout << "Invalid username or password. Please try again.\n";
            }
        } else if (choice == 's') {
            cout << "Enter a username: ";
            cin >> username;
            if (!isValidUsername(username)) {
                cout << "Invalid username. It must be at least 5 characters long.\n";
                continue;
            }

            if (users.find(username) != users.end()) {
                cout << "Username already exists. Please enter a unique username.\n";
                continue;
            }

            cout << "Enter a password: ";
            cin >> password;
            if (!isValidPassword(password)) {
                cout << "Invalid password. It must be at least 5 characters long.\n";
                continue;
            }

            users.insert({username,password});
            cout << "\n\033[1mAccount created successfully!\033[0m\n";
            break;
        } else {
            cout << "Invalid choice. Please enter L or S.\n";
        }
    }



        vector<Category> categories;
        ifstream cat("categories.txt");
        string line1;
        while (getline(cat, line1)) {
            Category c(line1);
            categories.push_back(c);
        }

        vector<Item*> items;
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
            Item* i = new Item(name, brandName, quantity, price, inStock, category, productID);
            items.push_back(i);
        }
//        map<int, Texture2D> textures;
//        for (const auto& item : items) {
//            Texture2D texture = LoadTexture(("resources/images/" + std::to_string(item.productID) + ".png").c_str());
//            textures[item.productID] = texture;
//        }
//        map<string,string> users;

        Cart cart;
    loadCart(username, items, userCarts[username]);


    UserHomePage(items);
    int productID = 3;
    ItemPage(productID, items, cart);
    if(cart_display(cart)) {
        if(CheckoutPage(cart)) {
            OrderConfirmationPage(cart);
        }
        else {
            cout << "Order cancelled." << endl;
        }
    }
    else {
        cout << "Order cancelled." << endl;
    }

//        AppState state = MAIN_MENU;

//    }
//    catch (const CustomException &e) {
//        cout << e.what() << endl;
//    }
//    catch(const InvalidNumberException &e) {
//        cout << e.what() << endl;
//    }
//    catch (const std::exception &e) {
//        cout << "Caught an exception: " << e.what() << endl;
//    }
//    catch (...) {
//        cout << "Caught an unknown exception." << endl;
//    }

    return 0;
}