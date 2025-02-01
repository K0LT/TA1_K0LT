// salesManager.cpp - Sales processing implementation
#include "salesManager.h"
#include "stock.h"
#include "utilities.h"

#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <cstring>


// Handles the sale of products, including checkout, returns a string with the sale information
std::string* sellProducts(std::string* stock, int& ID, int& clientNumber, int& productCounter) {

    // Constant expressions are evaluated at compile time (https://www.learncpp.com/cpp-tutorial/constant-expressions/)
    constexpr double markUp = 1.3;
    constexpr double taxRate = 0.23;

    // Every 'sold' product is added into this temporary raw array
    auto* addedProducts = new std::string[100 * 2];

    // We initialize our variables to be used later
    double totalBeforeTax = 0.0;
    double totalWithTax = 0.0;

    // Get client ID
    clientNumber = usrInput<int>("\nEnter client ID: ", true);

    // We display our products so the user can see what IDs to select
    listStock(stock, ID, nullptr, ID);

    // We initialize a boolean to run our loop with
    bool selling = true;

    while (selling) {

        // Collecting the chosen ID from the user
        int inputID = usrInput<int>("\nEnter product ID: ", true);

        // Validate the ID
        if (inputID >= ID || inputID < 0) {
            std::cout << "Please enter an ID smaller than " << ID << "\n";
            continue;
        }

        // Collecting the chosen quantity from the user
        int inputQT = usrInput<int>("Enter quantity: ", true);

        // Validate the quantity
        if (inputQT <= 0)
        {
            std::cout << "Please enter a quantity greater than 0.\n";
        }

        // We define a variable as constant so we don't accidentally change it
        const int index = inputID * 4;

        // Fetch the current stock quantity for the product
        int availableStock = std::stoi(stock[index + E_QT]);

        // Validate if the chosen quantity doesn't exceed the stock
        if (inputQT > availableStock) {
            std::cout << "Not enough stock! Available stock of [" << stock[index + E_NAME] << "]: " << availableStock << "\n";
            continue;
        }

        // We decrease the available stock by the chosen input and effect the change
        availableStock -= inputQT;
        stock[index + E_QT] = std::to_string(availableStock);

        // We fetch the cost price from the stock
        double costPrice = std::stod(stock[index + E_PRICE]);
        // Calculate sale price per unit and subtotal before tax
        double salePricePerUnit = costPrice * markUp;
        // Calculate subtotal by multiplying price per unit * ammount of units
        double subTotal = salePricePerUnit * inputQT;
        // Calculate tax amount for this product
        double tax = subTotal * taxRate;

        // Update total prices
        totalBeforeTax += subTotal;
        totalWithTax += subTotal + tax;

        // Add the selected product ID to our raw array at position (productCounter * 2)
        addedProducts[productCounter * 2] = std::to_string(inputID);

        // We add the quantity to the temporary array after the ID
        addedProducts[productCounter * 2 + 1] = std::to_string(inputQT);

        // We increment, productCounter serves as the number of products we've added and the index to navigate the array
        productCounter++;

        // Display product details to the user
        std::cout << "\nAdded [" << stock[index + E_NAME] << "] to cart.";

        // Print out information to the user for our receipt
        std::cout << "\nQuantity: " << inputQT
                  << " | Sale Price per unit: $" << salePricePerUnit
                  << " | Subtotal: $" << subTotal
                  << " | Tax: $" << tax << "\n";

        // Get the input from the user
        auto inputStr = usrInput<std::string>("\nAdd more products? [YES/NO]: \n",false);

        // Convert it to uppercase
        upperCase(inputStr);

        // Test the input, if not "YES", loop continues
        selling = (inputStr == "YES");
    }

    // Start the checkout process
    std::cout << "\n=== Checkout ===\n";
    std::cout << "Total Before Tax: $" << totalBeforeTax << "\n";
    std::cout << "Total Tax: $" << (totalWithTax - totalBeforeTax) << "\n";
    std::cout << "Total with Tax: $" << totalWithTax << "\n";

    // Accept payment from the user
    double amountPaid = 0.0;

    // If the amount paid is inferior to the total, prompt again
    while (amountPaid < totalWithTax) {
        amountPaid = usrInput<double>("Enter amount paid by client: $", true);
        if (amountPaid < totalWithTax) {
            std::cout << "Insufficient payment. Please enter at least $" << totalWithTax << "\n";
        }
    }

    // Calculate and display change
    double change = amountPaid - totalWithTax;
    std::cout << "Change: $" << change << "\n";

    // After we've added our products to the array, we add a 'sentinel value' after the last product position, we use this information to know when to stop printing the receipt
    addedProducts[productCounter * 2] = "END";

    // We add a second guard for good measure, since everything in this array exists in pairs (if we only add one, we break the indexing since it's based on increments of 2)
    addedProducts[productCounter * 2 + 1] = "";

    // After the sentinel values, we add another pair of values to the array (amount paid and change)
    addedProducts[productCounter * 2 + 2] = std::to_string(amountPaid);
    addedProducts[productCounter * 2 + 3] = std::to_string(change);

    // After everything is added, we return the array
    return addedProducts;
}

// This function prints out the receipt information and also determines if there's a random free sale or not
void printReceipt(std::string* addedProducts, std::string* stock, int& receiptNumber, int& clientNumber, const char* timePoint, const int productCounter) {

    // We define some constexpr variables (they won't change and the initializer is known at compile time, good practice)
    constexpr double taxRate = 0.23;

    // We define the increase from vendor price to sale price (30%, so we replace the value with 130% of itself)
    constexpr double markUp = 1.3;

    // Initializing the subtotal
    double subTotal = 0.0;

    // Amount paid and change are the value pair after the "END" and ""
    double amountPaid = std::stod(addedProducts[(productCounter * 2) + 2]);
    double change = std::stod(addedProducts[(productCounter * 2) + 3]);

    // 10% chance to win the lottery
    bool lotteryWinner = (std::rand() % 10 == 0);

    // Receipt header
    std::cout << "\n===== RECEIPT =====\n";
    std::cout << "Receipt: #" << receiptNumber++ << "\nClient: #" << clientNumber;
    std::cout << "\nDate: " << timePoint;

    // We initialize variables where they're relevant, good practice, not mandatory
    double totalBeforeTax = 0.0;
    double totalTax = 0.0;

    // We iterate through the array, reading the values and printing them
    for (int i = 0; i < productCounter; i++) {
        int id = std::stoi(addedProducts[i * 2 + E_ID]);
        int qty = std::stoi(addedProducts[i * 2 + E_QT]);

        double costPrice = std::stod(stock[id * 4 + E_PRICE]);
        double salePricePerUnit = costPrice * markUp;
        subTotal = salePricePerUnit * qty;
        double tax = subTotal * taxRate;
        totalBeforeTax += subTotal;
        totalTax += tax;

        // Print product details
        std::cout << "\nProduct: " << stock[id * 4 + E_NAME];
        std::cout << "\nQuantity: " << qty;
        std::cout << "\nPrice Before Tax: $" << subTotal;
        std::cout << "\nTax Rate: " << (taxRate * 100) << "%";
        std::cout << "\nPrice After Tax: $" << (subTotal + tax) << "\n";
    }

    // If this variable is flagged true, we replace the change with the amount the user paid (therefore free)
    if (lotteryWinner) {
        subTotal = 0; // Lottery winners pay nothing
        std::cout << "\nCongratulations! You won the lottery!\n";

        std::cout << "\nTotal Before Tax: $" << std::fixed << std::setprecision(2) << totalBeforeTax;
        std::cout << "\nTotal Tax: $" << totalTax;
        std::cout << "\nTotal After Tax: $" << (totalBeforeTax + totalTax);
        std::cout << "\nAmount Paid: $" << amountPaid;
        std::cout << "\nChange: $" << amountPaid;
    }
    else
    {
        // Otherwise, we print the values normally
        std::cout << "\nTotal Before Tax: $" << std::fixed << std::setprecision(2) << totalBeforeTax;
        std::cout << "\nTotal Tax: $" << totalTax;
        std::cout << "\nTotal After Tax: $" << (totalBeforeTax + totalTax);
        std::cout << "\nAmount Paid: $" << amountPaid;
        std::cout << "\nChange: $" << change;
    }

    // Receipt footer
    std::cout << "\n===================\n";
}

// Our main menu function definition
void mainMenu(std::string*& stock, int& totalProducts, int& stockSize, int& ID, int& clientNumber, int&receiptNumber) {
    bool running = true;

    while (running) {
        std::cout << "\n==== MAIN MENU ====";
        std::cout << "\n1. Sell Products";
        std::cout << "\n2. Manage Stock";
        std::cout << "\n3. View Stock";
        std::cout << "\n4. Exit";

        // Get user input, same method as always
        int choice = usrInput<int>("\nSelect an option: ", true);

        switch (choice) {
        case 1: {
            int productCounter = 0;

            // We grab the return of the sellProducts function and store its address in a pointer
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
            // If the user enters '4', running flag is turned to false, loop doesn't reset
            running = false;
            break;
        default:
            // Any input that isn't covered by the switch cases leads here
            std::cout << "\nInvalid option. Try again.\n";
        }
    }
}