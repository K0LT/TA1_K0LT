#include <iostream>
#include <cctype>
#include "salesManager.h"
#include "product.h"
#include "stock.h"
#include "enums.h"

constexpr double TAX_RATE = 0.27; // Define tax as a constant

void upperCase(std::string &str) {
    for (char &c : str) {
        c = std::toupper(static_cast<unsigned char>(c));
    }
}

// Collects user input to determine what products to sell
std::string** sellProducts(std::string **stock, int &ID, int &clientNumber) {

    std::string** addedProducts = new std::string*[ID]; // Dynamically allocate based on ID
    int productCounter = 0;

    std::cout << "\nPlease enter the client's number: ";
    while (!(std::cin >> clientNumber)) {
        std::cin.clear();
        std::cin.ignore(1000, '\n');
        std::cout << "Invalid input. Enter a valid client number: ";
    }

    bool selling = true;
    while (selling) {
        std::string inputStr;
        int inputQT = 0, inputID = 0;

        while (true) {
            std::cout << "\nWould you like a list of available products? [YES] [NO]\n";
            std::cin >> inputStr;
            upperCase(inputStr);
            if (inputStr == "YES" || inputStr == "NO") break;
        }

        if (inputStr == "YES") {
            listStock(stock);
        }

        while (true) {
            std::cout << "\nPlease enter a valid product ID: ";
            if (!(std::cin >> inputID) || inputID < 0 || inputID >= ID || stock[inputID] == nullptr) {
                std::cin.clear();
                std::cin.ignore(1000, '\n');
                std::cout << "\nInvalid ID. Try again.";
            } else break;
        }

        listProduct(inputID, stock);

        while (true) {
            std::cout << "\nHow many of [" << stock[inputID][E_NAME] << "] would you like to add? ";
            if (!(std::cin >> inputQT) || inputQT <= 0 || inputQT > std::stoi(stock[inputID][E_QT])) {
                std::cin.clear();
                std::cin.ignore(1000, '\n');
                std::cout << "\nInvalid quantity. Try again.";
            } else break;
        }
        removeQuantity(inputID, stock, inputQT);

        addedProducts[productCounter] = new std::string[2];
        addedProducts[productCounter][E_ID] = std::to_string(inputID);
        addedProducts[productCounter][E_QT] = std::to_string(inputQT);
        productCounter++;

        while (true) {
            std::cout << "\nWould you like to add more products? [YES] [NO]\n";
            std::cin >> inputStr;
            upperCase(inputStr);
            if (inputStr == "YES" || inputStr == "NO") break;
        }
        selling = (inputStr == "YES");
    }
    addedProducts[productCounter] = nullptr; // Null terminate array for safety
    return addedProducts;
}

void printReceipt(std::string **addedProducts, std::string **stock, int &receiptNumber, int &clientNumber, const char* timePoint) {
    double totalPrice = 0;
    std::cout << "\nPrinting receipt...";
    std::cout << "\nReceipt number: " << receiptNumber++;
    std::cout << "\nClient number: " << clientNumber << std::endl;

    for (int i = 0; addedProducts[i] != nullptr; i++) {
        int addedProductID = std::stoi(addedProducts[i][E_ID]);
        int addedProductQT = std::stoi(addedProducts[i][E_QT]);
        double beforeTax = addedProductQT * std::stod(stock[addedProductID][E_PRICE]);
        double afterTax = beforeTax * (1 + TAX_RATE);
        totalPrice += afterTax;

        std::cout << "\nProduct: [" << stock[addedProductID][E_NAME] << "]\n";
        std::cout << "Quantity: [" << addedProducts[i][E_QT] << "]";
        std::cout << "\nPrice(before tax): [" << beforeTax << "]";
        std::cout << "\nTax: " << (TAX_RATE * 100) << "%";
        std::cout << "\nPrice(after tax): " << afterTax;
    }

    std::cout << "\nTotal: " << totalPrice;
    std::cout << "\nDate: " << timePoint;
}

// Free dynamically allocated memory
void freeAddedProducts(std::string **addedProducts) {
    for (int i = 0; addedProducts[i] != nullptr; i++) {
        delete[] addedProducts[i];
    }
    delete[] addedProducts;
}
