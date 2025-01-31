#include <chrono>
#include <iostream>
#include <string>
#include "stock.h"
#include "salesManager.h"

int main() {

    int stockSize = 100;
    static int ID = 0, receiptNumber = 1, clientNumber = 0;
    int totalProducts = ID;

    // Creating our stock dynamically
    std::string* stock = createStock(stockSize);
    initializeStock(stock, ID,totalProducts, stockSize);

    // Hard coding some products into our stock
    // hardCodedExamples(stock, ID);
    // addNewProduct(ID,stock, STOCK_SIZE);
    // Calling our main menu function
    mainMenu(stock, totalProducts, stockSize, ID, clientNumber, receiptNumber);

    // Releasing the memory we allocated
    deleteStock(stock);
}