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
std::string** sellProducts(std::string **stock, int &ID, int &clientNumber)
{
    std::string** addedProducts = new std::string*[100];

    int productCounter{0};

    std::cout << "\nPlease enter the client's number: ";
    std::cin >> clientNumber;

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
            std::cout << "\nWould you like a list of available products?\n[YES] [N0]\n";
            std::cin >> inputStr;
            upperCase(inputStr);
            (inputStr == "YES" || inputStr == "NO") ? correctInput = true : correctInput = false;
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
            std::cout << "\nHow many of [" << stock[inputID][2] << "] would you like to add to your order?\n";
            std::cin >> inputQT;

            if (inputQT > stoi(stock[inputID][1]))
            {
                std::cout << "\nThere are only [" << stock[inputID][1] << "] of [" << stock[inputID][2] << "] remaining. You entered [" << inputQT << "].";
            }
            else correctInput = true;
            removeQuantity(inputID,stock,inputQT);
        }

        std::cout << "\nAdded [" << inputQT << "] [" << stock[inputID][2] << "] to your order!\n";

        addedProducts[productCounter] = new std::string[2];
        addedProducts[productCounter][0] = std::to_string(inputID);
        addedProducts[productCounter][1] = std::to_string(inputQT);
        productCounter++;

        correctInput = false;
        while (!correctInput)
        {
            std::cout << "\nWould you like to add more products to your order? [YES] [NO]\n";
            std::cin >> inputStr;
            upperCase(inputStr);
            (inputStr == "YES" || inputStr == "NO") ? correctInput = true : correctInput = false;
        }
        (inputStr == "YES") ? selling = true : selling = false;
    }while (selling);
    return addedProducts;
}

void printReceipt(std::string **addedProducts, std::string **stock, int &receiptNumber, int &clientNumber)
{
    int iterator{0};
    std::cout << "\nPrinting receipt...\n";
    std::cout << "\nReceipt number: " << receiptNumber++;
    std::cout << "\nClient number: " << clientNumber << std::endl;
    do
    {
        std::cout << "\nProduct: [" << stock[std::stoi(addedProducts[iterator][0])][2] << "].\nQuantity: [" << addedProducts[iterator][1] << "].\n" << "\nPrice: [" << std::stoi(stock[std::stoi(addedProducts[iterator][0])][3]);
        iterator++;
    }while (addedProducts[iterator] != nullptr);

}
