// salesManager.h - Sales processing functions
#pragma once
#include <string>
#include <iostream>
#include <sstream>
#include <limits>
#include <type_traits>
#include <algorithm>
#include <iomanip>

inline std::string formatPrice(const std::string& priceStr) {
    // Convert the string to a double
    double price = std::stod(priceStr);

    // Format the double to two decimal places
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(3) << price;
    // Return the formatted price as a string
    return oss.str();
}
// Helper function to clear input buffer
inline void clearInputBuffer() {

    //If there is something to clear in the buffer
    if (std::cin.rdbuf()->in_avail() > 0) {

        // Only ignore input if there's something to clear otherwise it will pause execution waiting for a new line char
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    // Reset error flags if any
    std::cin.clear();
}


template <typename T>
T usrInput(const std::string& prompt, bool onlyDigits) {
    std::string input;

    clearInputBuffer();

    while (true) {
        std::cout << prompt;

        // Read the entire line as a string
        std::getline(std::cin, input);

        if (onlyDigits) {

            //Maybe we can use the isDigit here
            if (input.empty() || input.find_first_not_of("0123456789") != std::string::npos) {
                std::cout << "Invalid input. Please try again.\n";
                continue;
            }

            // Special handling for integer types
            if constexpr (std::is_integral_v<T>) {
                try {
                    size_t pos;
                    long long num = std::stoll(input, &pos);  // Convert to long long for range checking

                    if (num > std::numeric_limits<T>::max()) {
                        std::cout << "Invalid input. Number is out of range. You cannot enter a number greater than "
                            << std::numeric_limits<T>::max() << ". Please try again.\n";
                        continue;
                    }
                    if (num < std::numeric_limits<T>::min()) {
                        std::cout << "Invalid input. Number is out of range. You cannot enter a number smaller than "
                            << std::numeric_limits<T>::min() << ". Please try again.\n";
                        continue;
                    }

                    clearInputBuffer(); // Clear before returning
                    return static_cast<T>(num); // Successfully converted input within range
                }
                catch (const std::exception&) {
                    std::cout << "Invalid input. Please enter a valid number within range.\n";
                    continue;
                }
            }
        }
        else {
            // Ensure the first character is not a digit for string input
            if constexpr (std::is_same_v<T, std::string>) {
                if (!input.empty() && std::isdigit(input[0])) {
                    std::cout << "Invalid input. A string cannot start with a digit. Please try again.\n";
                    continue;
                }
                clearInputBuffer(); // Clear before returning
                return input; // Return valid string input
            }
        }

        std::stringstream ss(input);
        T value;
        if (ss >> value && ss.eof()) {  // Ensure full conversion without leftovers
            clearInputBuffer(); // Clear before returning
            return value;  // Successfully converted input
        }
        else {
            std::cout << "Invalid input. Please try again.\n";
        }
    }
}

inline double readDouble(const std::string& prompt) {
    std::string input;

    while (true) {
        std::cout << prompt;
        std::getline(std::cin, input);

        // Validate input for a valid decimal number
        if (input.empty() || input.find_first_not_of("0123456789.") != std::string::npos ||
            std::ranges::count(input.begin(), input.end(), '.') > 1) {
            std::cout << "Invalid input. Please enter a valid decimal number (e.g., 0.99).\n";
            continue;
            }

        try {
            return std::stod(input);
        } catch (const std::exception&) {
            std::cout << "Invalid input. Please enter a valid decimal number (e.g., 0.99).\n";
        }
    }
}

void upperCase(std::string& str);
std::string* sellProducts(std::string* stock, int& ID, int& clientNumber, int& productCounter);
void printReceipt(std::string* addedProducts, std::string* stock, int& receiptNumber, int& clientNumber, const char* timePoint, const int productCounter);
void mainMenu(std::string*& stock, int& totalProducts, int& stockSize, int& ID, int& clientNumber, int&receiptNumber);
