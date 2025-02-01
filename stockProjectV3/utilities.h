// Some helper functions to help deal with inputs and parsing values

// Included only once per compilation
#pragma once
#include <string>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <limits>
#include <type_traits>
#include <ranges>
#include <algorithm>

// Formats a string wit hour price to a fixed decimal string with n decimal places
inline std::string formatPrice(const std::string& priceStr) {

    // Convert the string to a double for numerical operations
    double price = std::stod(priceStr);

    // Create a string stream to format our double value
    std::ostringstream oss;

    // We set the stream to fixed-point notation and limit it to n places
    oss << std::fixed << std::setprecision(3) << price;

    // Converts the formatted number back to a string and return it
    return oss.str();
}

// Helper function to clear input buffer and reset it
inline void clearInputBuffer() {

    //If there is something to clear in the buffer
    if (std::cin.rdbuf()->in_avail() > 0) {

        // Only ignore input if there's something to clear otherwise it will pause execution waiting for a new line char ('\n')
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    // Reset error flags if any
    std::cin.clear();
}

// This is a template function, which means it can accept more than one type of variable
template <typename T>
T usrInput(const std::string& prompt, bool onlyDigits) {
    std::string input;

    // Clear the input buffer before reading input
    clearInputBuffer();

    // Read input until valid input is given (captures everything until newline '\n')
    while (true) {
        std::cout << prompt;

        // Read the entire line as a string (captures everything until newline '\n')
        std::getline(std::cin, input);

        // When only digits are allowed, the onlyDigits boolean will be passed as true and we enter here
        if (onlyDigits) {

            // Check if the input is empty or contains non-digit characters
            if (input.empty() || input.find_first_not_of("0123456789") != std::string::npos) {
                std::cout << "Invalid input. Please try again.\n";
                continue;
            }

            // Handle numeric input specifically for integral types
            if constexpr (std::is_integral_v<T>) {
                try {
                    size_t pos;

                    // Convert user input to a long long type for extended range checking
                    long long num = std::stoll(input, &pos);

                    // Validate that the number fits within the range of type T
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

                    // Clear the input buffer before returning the valid result
                    clearInputBuffer();

                    // Convert and return the valid input as type T
                    return static_cast<T>(num);
                }

                catch (const std::exception&) {
                    // Handle failure during conversion (invalid string-to-number input)
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

                // Clear the input buffer before returning the valid result
                clearInputBuffer();

                // Return valid string input
                return input;
            }
        }

        // Handle conversion for other types (float, double, etc.)
        std::stringstream ss(input);
        T value;

        // Confirm the input was fully converted without extra characters left over
        if (ss >> value && ss.eof()) {

            // Clear the input buffer return the valid result
            clearInputBuffer();
            return value;
        }
        else {
            // Handle failure for invalid inputs
            std::cout << "Invalid input. Please try again.\n";
        }
    }
}

// Helper function to read a double value from user input
inline double readDouble(const std::string& prompt) {
    std::string input;

    // Continuously prompt the user until valid input is provided
    while (true) {
        std::cout << prompt;

        // Read the entire line as a string
        std::getline(std::cin, input);

        // Validate input for a valid decimal number
        if (input.empty() || input.find_first_not_of("0123456789.") != std::string::npos ||
            std::ranges::count(input.begin(), input.end(), '.') > 1) {
            std::cout << "Invalid input. Please enter a valid decimal number (e.g., 0.99).\n";
            continue;
            }

        // Try converting the input string to a double
        try {

            // Returns the double value when it's valid
            return std::stod(input);
        } catch (const std::exception&) {

            // We handle conversion errors here (malformed input, for example)
            std::cout << "Invalid input. Please enter a valid decimal number (e.g., 0.99).\n";
        }
    }
}

// Converts all individual characters that make up the string to uppercase
inline void upperCase(std::string& str) {
    // We iterate through each character in the string and convert it to uppercase (we use casts because implicit conversions are implementation defined)
    for (char& c : str) c = static_cast<char>(std::toupper(static_cast<unsigned char>(c)));
}