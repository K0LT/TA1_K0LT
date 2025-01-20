//
// Created by k0lt on 12/27/24.
//
#pragma once
#include <string>

enum Suits
{
    HEARTS, DIAMONDS, SPADES, CLUBS, UNDEFINED
};


enum Ranks
{
    ACE = 10, SEVEN = 9, KING = 8, JACK = 7, QUEEN = 6, SIX = 5, FIVE = 4, FOUR = 3, THREE = 2, TWO = 1, NOVALUE = 0
};


class Card {

    Suits suit = UNDEFINED;
    Ranks rank = NOVALUE;
    int value;
public:
    // Constructor & Destructor
    Card() = default;
    Card(Suits passedSuit, int rankValue);
    ~Card() = default;

    // Getters
    Suits getSuit();
    Ranks getRank();
    int getValue();

    // Setters
    void setSuit(Suits passedSuit);
    void setRank(Ranks passedRank);
};

