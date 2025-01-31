#pragma once

#include <iostream>
#include <cctype>
#include "stock.h"
#include "product.h"

void upperCase(std::string &str);

std::string** sellProducts(std::string **stock, int &ID, int &clientNumber);

void printReceipt(std::string **addedProducts, std::string **stock, int &receiptNumber, int &clientNumber, const char* timePoint);
