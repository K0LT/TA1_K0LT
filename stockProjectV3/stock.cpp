#include "stock.h"
#include "salesManager.h"
#include <iostream>
#include <limits>

std::string* createStock(int size) {
    return new std::string[size * 4]();
}

void initializeStock(std::string*& stock, int& ID, int& totalProducts, int& stockSize) {
    // Predefined product data: Name, Quantity, Cost Price
    std::string productNames[] = {"Product1", "Product2", "Product3"};
    int productQuantities[] = {10, 20, 15};
    double productCostPrices[] = {5.00, 3.50, 7.25};
    double markUp = 1.3;

    // Calculate the number of predefined products
    int numPredefinedProducts = sizeof(productNames) / sizeof(productNames[0]);

    // Ensure stockSize can fit the predefined products
    if (numPredefinedProducts > stockSize) {
        std::cout << "\nNot enough space in stock to initialize predefined products.\n";
        return;
    }

    // Manually populate the stock array for each product
    for (int i = 0; i < numPredefinedProducts; i++) {
        int index = ID * 4; // Calculate index based on ID

        stock[index + E_ID] = std::to_string(ID);                         // Store ID
        stock[index + E_NAME] = productNames[i];                         // Store Name
        stock[index + E_QT] = std::to_string(productQuantities[i]);      // Store Quantity
        stock[index + E_PRICE] = std::to_string(productCostPrices[i] * markUp); // Calculate and store Selling Price

        ID++;
        totalProducts++;// Increment ID for the next product
    }

    std::cout << "\nStock initialized with predefined products.\n";
}

void deleteStock(std::string* stock) {
    delete[] stock;
}

void listStock(std::string*& stock, int size, std::string* transactions, int productCount) {

    constexpr double markUp = 1.3;
    std::cout << "\n=== Stock List ===\n";
    // Iterating through the stock array
    for (int i = 0; i < size; i++) {

        // Our index is the position we want multiplied by 4
        int index = i * 4;

        // If the string at index IS NOT empty
        if (!stock[index].empty())
        {
            // Gets the quantity of the product at index
            int currentStock = std::stoi(stock[index + E_QT]);

            for (int j = 0; j < productCount; j++) {

                if (transactions != nullptr){
                    // If the ID (i) exists in our transactions, enter
                    if (std::stoi(transactions[j * 2]) == i) {

                        // Temporarily storing the quantity of found product
                        int soldQty = std::stoi(transactions[j * 2 + E_QT]);

                        // Adjusting the quantity of found product
                        currentStock -= soldQty;
                    }
                }
            }

            double costPrice = std::stod(stock[index + E_PRICE]);
            double salePrice = costPrice * markUp;
            // Print the product details, including the formatted price
            std::cout << "\nID: " << stock[index]
                      << "\nQuantity: " << currentStock
                      << "\nName: " << stock[index + E_NAME]
                      << "\nVendor Cost: $" << formatPrice(std::to_string(costPrice))
                      << "\nSale Price: $" << formatPrice(stock[index + E_PRICE]) << "\n";
        }
    }
}

void addStockToExistingProduct(std::string* stock, int totalProducts) {
    int productID, addedQuantity;
    std::cout << "\nEnter product ID to add stock: ";
    std::cin >> productID;
    std::cout << "Enter quantity to add: ";
    std::cin >> addedQuantity;

    int index = productID * 4;
    int currentQuantity = std::stoi(stock[index + E_QT]);
    stock[index + E_QT] = std::to_string(currentQuantity + addedQuantity);

    std::cout << "\nStock updated!";
}

void stockManagementMenu(std::string*& stock, int& totalProducts, int& stockSize, int& ID) {
    bool managingStock = true;

    while (managingStock) {
        std::cout << "\n==== STOCK MANAGEMENT MENU ====";
        std::cout << "\n1. Add New Product";
        std::cout << "\n2. Add Stock to Existing Product";
        std::cout << "\n3. Remove Product";
        std::cout << "\n4. List Stock";
        std::cout << "\n5. Return to Main Menu";

        int choice = usrInput<int>("\nSelect an option: ",true);

        switch (choice) {
        case 1:
            addNewProduct(ID,totalProducts, stock, stockSize);
            break;
        case 2:
            addStockToExistingProduct(stock, totalProducts);
            break;
        case 3:
            removeProduct(stock, totalProducts);
            break;
        case 4:
            listStock(stock, stockSize);
            break;
        case 5:
            managingStock = false;
            break;
        default:
            std::cout << "\nInvalid option. Try again.\n";
        }
    }
}

void addNewProduct(int& ID, int& totalProducts, std::string*& stock, int& stockSize) {

    int confirmedID = findSmallestAvailableID(stock, totalProducts);

    if (totalProducts >= stockSize) {
        std::cout << "\nStock limit reached!";
        return;
    }

    auto name = usrInput<std::string>("\nEnter product name: ", false);
    int quantity = usrInput<int>("Enter stock quantity: ", true);
    double costPrice = readDouble("Enter cost price: $");


    int index = confirmedID * 4;

    stock[index + E_ID] = std::to_string(confirmedID);
    stock[index + E_QT] = std::to_string(quantity);
    stock[index + E_NAME] = name;
    stock[index + E_PRICE] = std::to_string(costPrice);

    if (confirmedID >= ID) {
        ID = confirmedID + 1;
    }

    std::cout << "\nAdded [" << name << "] at $" << costPrice;
    totalProducts++;
}

void removeProduct(std::string*& stock, int& totalProducts) {

    int productID = usrInput<int>("\nEnter product ID to remove: ", true);

    // Validate if the product ID is within bounds
    if (productID < 0 || productID >= totalProducts) {
        std::cout << "Invalid product ID." << std::endl;
        return;
    }

    // Locate the product in the stock array
    for (int i = 0; i < totalProducts; i++) {
        int index = i * 4;
        if (stock[index + E_ID] == std::to_string(productID)) {
            // Clear the strings in the identified slot
            stock[index + E_ID].clear();
            stock[index + E_QT].clear();
            stock[index + E_NAME].clear();
            stock[index + E_PRICE].clear();

            std::cout << "\nProduct removed.";
            return; // Exit the function after removing the product
        }
    }

    std::cout << "Product ID not found." << std::endl;
}

int findSmallestAvailableID(const std::string* stock, int totalProducts) {
    for (int i = 0; i < totalProducts; i++) {
        bool idExists = false;

        // Check if this ID exists in the stock array
        for (int j = 0; j < totalProducts; j++) {
            if (std::to_string(i) == stock[j * 4 + E_ID]) {
                idExists = true;
                break;
            }
        }

        // If ID `i` does not exist, it's the smallest unused ID
        if (!idExists) {
            return i;
        }
    }

    // If no gaps are found, return the next sequential ID
    return totalProducts;
}