// This header will only be included in our compiled program once
#pragma once
#include <string>

// Adds a new std::string array with 4 positions to our 2D string array (stock)
void addProduct(int &ID, int quantity, std::string &name, float price, std::string **stock);

// Lists an individual product
void listProduct(int index, std::string **stock);

// Adds more of an existing product
void addQuantity(int &ID, std::string **stock, int addedQuantity);

void removeQuantity(int &ID, std::string **stock, int removedQuantity);

// Deletes the std::string array at ID index in stock
void deleteProduct(int indexID, std::string **stock);
