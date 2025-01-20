#include "product.h"
#include <string>
#include <iostream>

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
            stock[tempID][0] = std::to_string(tempID);
            stock[tempID][1] = std::to_string(quantity);
            stock[tempID][2] = name;
            stock[tempID][3] = std::to_string(price);
            break;
        }
    }
    stock[ID] = new std::string[4];
    stock[ID][0] = std::to_string(ID);
    stock[ID][1] = std::to_string(quantity);
    stock[ID][2] = name;
    stock[ID][3] = std::to_string(price);
    ID++;
}

void listProduct(int index, std::string **stock)
{
    std::cout << "\nProduct ID: "<< stock[index][0] << "\nQuantity: " << stock[index][1] << "\nName: " << stock[index][2] << "\nPrice(noTax): " << stock[index][3] << std::endl;
}

void addQuantity(int &ID, std::string **stock, int addedQuantity)
{
    int temp = std::stoi(stock[ID][1]);
    temp += addedQuantity;
    stock[ID][1] = std::to_string(temp);
}

void removeQuantity(int& ID, std::string** stock, int removedQuantity)
{
    int temp = std::stoi(stock[ID][1]);
    temp -= removedQuantity;
    stock[ID][1] = std::to_string(temp);
}

// We store the address in a temp pointer, we set the stock index to nullptr and then delete the allocated array
void deleteProduct(int indexID, std::string **stock)
{
    std::string* tempPtr = stock[indexID];
    stock[indexID] = nullptr;
    delete[] tempPtr;
}
