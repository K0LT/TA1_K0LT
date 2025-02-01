// salesManager.h - Function declarations that deal with sales
#pragma once

// We use strings, so we include <string>
#include <string>

// We use this for setprecision
#include <iomanip>

// Our functions are declared here, we include the definitions in the .cpp file with the same name
std::string* sellProducts(std::string* stock, int& ID, int& clientNumber, int& productCounter);
void printReceipt(std::string* addedProducts, std::string* stock, int& receiptNumber, int& clientNumber, const char* timePoint, int productCounter);
void mainMenu(std::string*& stock, int& totalProducts, int& stockSize, int& ID, int& clientNumber, int&receiptNumber);
