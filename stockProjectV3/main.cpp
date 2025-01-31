#include <chrono>
#include <string>
#include "stock.h"
#include "salesManager.h"

int main() {
    // We hard code our stock size, so we can change it as needed
    int stockSize = 100;

    // Static variables never go out of scope, this is somewhat redundant, but it's a good practice
    static int ID = 0, receiptNumber = 1, clientNumber = 0;

    // We link the ID to totalProducts to ensure the values are coherent
    int totalProducts = ID;

    // Creating our stock dynamically
    std::string* stock = createStock(stockSize);

    // Initializing our stock with some default values
    initializeStock(stock, ID,totalProducts, stockSize);

    // Calling our main menu function
    mainMenu(stock, totalProducts, stockSize, ID, clientNumber, receiptNumber);

    // Releasing the memory we allocated
    deleteStock(stock);
}