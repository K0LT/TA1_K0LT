#include "stock.h"
#include "salesManager.h"
#include "utilities.h"
#include <iostream>

// We receive the desired size of the array, then multiply by 4 (every product is stored in a sequential series of 4 strings)
std::string* createStock(int size) {
    // The function creates and returns the array to the caller
    return new std::string[size * 4]();
}
// We use this function so the program has some products in stock when started, this is the very first function call we make
void initializeStock(std::string*& stock, int& ID, int& totalProducts, int& stockSize) {

    // Predefined product data: Name, Quantity, Cost Price
    std::string productNames[] = {"bananas", "pasta", "cheese"};
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

        // Index is calculated based on the ID
        int index = ID * 4;

        // Storing the entered values in the respective positions
        stock[index + E_ID] = std::to_string(ID);
        stock[index + E_NAME] = productNames[i];
        stock[index + E_QT] = std::to_string(productQuantities[i]);
        stock[index + E_PRICE] = std::to_string(productCostPrices[i] * markUp);

        // Increment the ID for the next product; Increment the total product count (this doesn't necessarily have to match the ID, we can delete products, but we never decrease the ID)
        ID++;
        totalProducts++;
    }

    std::cout << "\nStock initialized with predefined products.\n";
}

// Our very last function call, we release the memory allocated to our raw array (stock)
void deleteStock(std::string* stock) {
    delete[] stock;
}

// This function lists the stock where there are valid positions to list
void listStock(std::string*& stock, int size, std::string* transactions, int productCount) {

    // Same as before, constexpr because the value is both a constant and the initializer is known at compile time
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

            // We iterate up to our total product count, there is no need to check the entire array every time
            for (int j = 0; j < productCount; j++) {

                // When listStock is called with an array of transactions, we list the proper values (we remove the quantity the user has added in their purchase)
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
                      << "\nSale Price: $" << formatPrice(std::to_string(salePrice)) << "\n";
        }
    }
}

// This function adds stock to an existing product in our stock
void addStockToExistingProduct(std::string* stock, int totalProducts) {

    listStock(stock, totalProducts, nullptr, totalProducts);

    int productID = usrInput<int>("\nEnter product ID to add stock: ",true);

    // If the string at the specified position is empty, the product does not exist
    if (stock[productID * 4].empty())
    {
        std::cout << "\nInvalid product ID. Try again.\n";
        return;
    }

    int addedQuantity = usrInput<int>("Enter quantity to add: ",true);

    int index = productID * 4;

    int currentQuantity = std::stoi(stock[index + E_QT]);

    // We update the chosen quantity directly in the stock
    stock[index + E_QT] = std::to_string(currentQuantity + addedQuantity);

    std::cout << "\nStock updated!";
}

// This function is our second menu, where we manage our stock related actions
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

// This function adds a new product in the first unused position found inside the array
void addNewProduct(int& ID, int& totalProducts, std::string*& stock, int& stockSize) {

    // ID is passed with the value it's been incremented to, we call findSmallestAvailableID to check whether there is a smaller ID that is unused (after a possible deletion)
    int confirmedID = findSmallestAvailableID(stock, totalProducts);

    // If the array is full we don't add any new products
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

// This function handles the removal of products by resetting the four strings at the chosen position to an empty state
void removeProduct(std::string*& stock, int& totalProducts) {

    listStock(stock, totalProducts, nullptr, totalProducts);

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
            return;
        }
    }

    std::cout << "Product ID not found." << std::endl;
}

// This function searches the stock up to the total product count to find any unused IDs
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