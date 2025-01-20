#include <iostream>
#include <cctype>
#include "salesManager.h"
#include "product.h"
#include "stock.h"

// Iterates through the string and swaps lowercase chars for uppercase
void upperCase(std::string &str)
{
    for (int i = 0; i < str.size(); i++)
    {
        str[i] = std::toupper(str[i]);
    }
}

// Collects user input to determine what products to sell
void sellProducts(std::string** stock, int &ID)
{
    bool selling = true;
    do
    {
        bool correctInput = false;
        std::string inputStr;
        int inputQT{0};
        int inputID{0};

        // Keeps the user in a loop until they enter a valid answer
        while (!correctInput)
        {
            std::cout << "\nWould you like a list of available products?\n[YES] [N0]";
            std::cin >> inputStr;
            upperCase(inputStr);
            (inputStr == "YES" && inputStr == "NO") ? correctInput = true : correctInput = false;
        }

        if (inputStr == "YES")
        {
            listStock(stock);
        }

        correctInput = false;
        while (!correctInput)
        {
            std::cout << "\nPlease enter a valid product ID: ";
            std::cin >> inputID;

            if (inputID > ID)
            {
                std::cout << "\nInvalid ID. Please enter a valid ID.";
                continue;
            }

            (stock[inputID] == nullptr) ? correctInput = false : correctInput = true;
        }

        listProduct(inputID,stock);

        // Reset our boolean, use a function for this in the future
        correctInput = false;
        while (!correctInput)
        {
            std::cout << "\nHow many of [" << stock[inputID][2] << "] would you like to add to your order?: ";
            std::cin >> inputQT;

            (inputQT > stoi(stock[inputID][1])) ? correctInput = false : correctInput = true;
        }

        std::cout << "\nAdded [" << inputQT << "] [" << stock[inputID][2] << "] to your order!\n";
        correctInput = false;
        while (!correctInput)
        {
            std::cout << "\nWould you like to add more products to your order? [YES] [NO]";
            std::cin >> inputStr;
            upperCase(inputStr);
            (inputStr == "YES" && inputStr == "NO") ? correctInput = true : correctInput = false;
        }
        (inputStr == "YES") ? selling = true : selling = false;
    }while (selling);
}
