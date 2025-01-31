// salesManager.h - Function declarations that deal with sales
#pragma once
#include <string>
#include <iomanip>


std::string* sellProducts(std::string* stock, int& ID, int& clientNumber, int& productCounter);
void printReceipt(std::string* addedProducts, std::string* stock, int& receiptNumber, int& clientNumber, const char* timePoint, const int productCounter);
void mainMenu(std::string*& stock, int& totalProducts, int& stockSize, int& ID, int& clientNumber, int&receiptNumber);
