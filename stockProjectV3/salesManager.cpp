// salesManager.cpp - Sales processing implementation
#include "salesManager.h"
#include "stock.h"

#include <iostream>
#include <cctype>
#include <iomanip>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <cstring>

// Converts string to uppercase
void upperCase(std::string& str) {
    for (char& c : str) c = std::toupper(c);
}

// Sell products
std::string* sellProducts(std::string* stock, int& ID, int& clientNumber, int& productCounter) {
    constexpr double markUp = 1.3;

    constexpr double taxRate = 0.23;

    auto* addedProducts = new std::string[100 * 2];

    double totalBeforeTax = 0.0;
    double totalWithTax = 0.0;

    clientNumber = usrInput<int>("\nEnter client ID: ", true);

    listStock(stock, ID, nullptr, ID);

    bool selling = true;

    while (selling) {
        
        int inputID = usrInput<int>("\nEnter product ID: ", true);

        if (inputID >= ID || inputID < 0) {
            std::cout << "Please enter an ID smaller than " << ID << "\n";
            continue;
        }
        
        int inputQT = usrInput<int>("Enter quantity: ", true);

        if (inputQT <= 0)
        {
            std::cout << "Please enter a quantity greater than 0.\n";
        }
        
        const int index = inputID * 4;

        int availableStock = std::stoi(stock[index + E_QT]);

        if (inputQT > availableStock) {
            std::cout << "Not enough stock!\n";
            continue;
        }
        availableStock -= inputQT;
        stock[index + E_QT] = std::to_string(availableStock);

        double costPrice = std::stod(stock[index + E_PRICE]);   // Fetch cost price
        double salePricePerUnit = costPrice * markUp;          // Calculate sale price per unit
        double subTotal = salePricePerUnit * inputQT;           // Calculate subtotal before tax
        double tax = subTotal * taxRate;                       // Tax amount for this product

        // Update total prices
        totalBeforeTax += subTotal;
        totalWithTax += subTotal + tax;

        addedProducts[productCounter * 2] = std::to_string(inputID);
        addedProducts[productCounter * 2 + 1] = std::to_string(inputQT);
        productCounter++;

        // Display product details to the user
        std::cout << "\nAdded [" << stock[index + E_NAME] << "] to cart.";
        std::cout << "\nQuantity: " << inputQT
                  << " | Sale Price per unit: $" << salePricePerUnit
                  << " | Subtotal: $" << subTotal
                  << " | Tax: $" << tax << "\n";

        auto inputStr = usrInput<std::string>("\nAdd more products? [YES/NO]: \n",false);
        upperCase(inputStr);
        selling = (inputStr == "YES");
    }

    // Step 7: Perform checkout
    std::cout << "\n=== Checkout ===\n";
    std::cout << "Total Before Tax: $" << totalBeforeTax << "\n";
    std::cout << "Total Tax: $" << (totalWithTax - totalBeforeTax) << "\n";
    std::cout << "Total with Tax: $" << totalWithTax << "\n";

    // Accept payment from the user
    double amountPaid = 0.0;
    while (amountPaid < totalWithTax) {
        amountPaid = usrInput<double>("Enter amount paid by client: $", true);
        if (amountPaid < totalWithTax) {
            std::cout << "Insufficient payment. Please enter at least $" << totalWithTax << "\n";
        }
    }

    // Calculate and display change
    double change = amountPaid - totalWithTax;
    std::cout << "Change: $" << change << "\n";

    addedProducts[productCounter * 2] = "END";
    addedProducts[productCounter * 2 + 1] = "";
    addedProducts[productCounter * 2 + 2] = std::to_string(amountPaid); // Amount Paid
    addedProducts[productCounter * 2 + 3] = std::to_string(change);     // Change

    return addedProducts;
}

void printReceipt(std::string* addedProducts, std::string* stock, int& receiptNumber, int& clientNumber, const char* timePoint, const int productCounter) {
    constexpr double TAX_RATE = 0.23;
    constexpr double markUp = 1.3;
    double subTotal = 0.0;
    double amountPaid = std::stod(addedProducts[(productCounter * 2) + 2]);
    double change = std::stod(addedProducts[(productCounter * 2) + 3]);

    bool lotteryWinner = (std::rand() % 10 == 0);

    // Receipt Header
    std::cout << "\n===== RECEIPT =====\n";
    std::cout << "Receipt: #" << receiptNumber++ << "\nClient: #" << clientNumber;
    std::cout << "\nDate: " << timePoint;

    double totalBeforeTax = 0.0;
    double totalTax = 0.0;

    for (int i = 0; i < productCounter; i++) {
        int id = std::stoi(addedProducts[i * 2]);       // Read product ID
        int qty = std::stoi(addedProducts[i * 2 + 1]);  // Read quantity

        double costPrice = std::stod(stock[id * 4 + E_PRICE]);   // Cost price
        double salePricePerUnit = costPrice * markUp;          // Sale price per unit
        subTotal = salePricePerUnit * qty;               // Subtotal before tax
        double tax = subTotal * TAX_RATE;                       // Tax for this product
        totalBeforeTax += subTotal;
        totalTax += tax;

        // Print product details
        std::cout << "\nProduct: " << stock[id * 4 + E_NAME];
        std::cout << "\nQuantity: " << qty;
        std::cout << "\nPrice Before Tax: $" << subTotal;
        std::cout << "\nTax Rate: " << (TAX_RATE * 100) << "%";
        std::cout << "\nPrice After Tax: $" << (subTotal + tax) << "\n";
    }

    // Lottery Discount
    if (lotteryWinner) {
        subTotal = 0; // Lottery winners pay nothing
        std::cout << "\nCongratulations! You won the lottery!\n";
    }

    // Print Total
    std::cout << "\nTotal: $" << std::fixed << std::setprecision(2) << subTotal << "\n";
}

void mainMenu(std::string*& stock, int& totalProducts, int& stockSize, int& ID, int& clientNumber, int&receiptNumber) {
    bool running = true;

    while (running) {
        std::cout << "\n==== MAIN MENU ====";
        std::cout << "\n1. Sell Products";
        std::cout << "\n2. Manage Stock";
        std::cout << "\n3. View Stock";
        std::cout << "\n4. Exit";

        int choice = usrInput<int>("\nSelect an option: ", true);

        switch (choice) {
        case 1: {
            int productCounter = 0;
            std::string* soldProducts = sellProducts(stock, ID, clientNumber, productCounter);
            if (soldProducts) {
                // Get current date & time
                const auto now = std::chrono::system_clock::now();
                const std::time_t t_c = std::chrono::system_clock::to_time_t(now);

                char timeBuffer[26];


// Checking to see if we're compiling on Windows or not, ctime is used on Linux, ctime_s is used on Windows
#if defined(_MSC_VER)
                ctime_s(timeBuffer, sizeof(timeBuffer), &t_c);
#else
                std::strcpy(timeBuffer, std::ctime(&t_c));
#endif

                printReceipt(soldProducts, stock, receiptNumber, clientNumber, timeBuffer, productCounter);
                delete[] soldProducts;
            }
            break;
        }
        case 2:
            stockManagementMenu(stock, totalProducts, stockSize, ID);
            break;
        case 3:
            listStock(stock, stockSize, nullptr, totalProducts);
            break;
        case 4:
            running = false;
            break;
        default:
            std::cout << "\nInvalid option. Try again.\n";
        }
    }
}