#include <string>
#include <iostream>
#include "product.h"
#include "enums.h"

void addProduct(int &ID, int quantity, std::string &name, float price, std::string **stock)
{
    // Using ID as an index and also as a total product count for now
    // Might use nullptrs to hold empty positions after we've deleted a product
    // Check if nullptr, if nullptr then add product at that ID
    for (int i = 0; i < 100; i++)
    {
        // If there are unused IDs in the stock, we use those instead of the specified ID
        if (stock[i] == nullptr && i < ID)
        {
            std::cout << "\n[ADDPRODUCT-DEBUG]: Found unused ID at " << i << ". Creating product with ID: " << i << " rather than " << ID;
            int tempID = i;
            stock[tempID] = new std::string[4];
            stock[tempID][E_ID] = std::to_string(tempID);
            stock[tempID][E_QT] = std::to_string(quantity);
            stock[tempID][E_NAME] = name;
            stock[tempID][E_PRICE] = std::to_string(price);
            break;
        }
    }

    stock[ID] = new std::string[4];
    stock[ID][E_ID] = std::to_string(ID);
    stock[ID][E_QT] = std::to_string(quantity);
    stock[ID][E_NAME] = name;
    stock[ID][E_PRICE] = std::to_string(price);
    ID++;
}

void listProduct(int index, std::string **stock)
{
    std::cout << "\nProduct ID: "<< stock[index][E_ID] << "\nQuantity: " << stock[index][E_QT] << "\nName: " << stock[index][E_NAME] << "\nPrice(noTax): " << stock[index][E_PRICE] << std::endl;
}

void addQuantity(int &ID, std::string **stock, int addedQuantity)
{
    int temp = std::stoi(stock[ID][E_QT]);
    temp += addedQuantity;
    stock[ID][1] = std::to_string(temp);
}

void removeQuantity(int& ID, std::string** stock, int removedQuantity)
{
    int temp = std::stoi(stock[ID][E_QT]);
    temp -= removedQuantity;
    stock[ID][E_ID] = std::to_string(temp);
}

// We store the address in a temp pointer, we set the stock index to nullptr and then delete the allocated array
void deleteProduct(int indexID, std::string **stock)
{
    std::string* tempPtr = stock[indexID];
    stock[indexID] = nullptr;
    delete[] tempPtr;
}
