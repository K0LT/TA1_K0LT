#pragma once
#include <string>

// Simple enumeration to make the stock more readable, E_ID = 0 (default), E_QT = 1, etc.
enum stockPositions { E_ID, E_QT, E_NAME, E_PRICE };

// Function declarations, pairs with stock.cpp (definitions)
std::string* createStock(int size);
void initializeStock(std::string*& stock, int& ID, int& totalProducts, int& stockSize);
void deleteStock(std::string* stock);
void addNewProduct(int& ID, int& totalProducts, std::string*& stock, int& stockSize);
void addStockToExistingProduct(std::string* stock, int totalProducts);
void removeProduct(std::string*& stock, int& totalProducts);
void listStock(std::string*& stock, int size,std::string* transactions = nullptr, int productCount = 0);
void stockManagementMenu(std::string*& stock, int& totalProducts, int& stockSize, int& ID);
int findSmallestAvailableID(const std::string* stock, int totalProducts);