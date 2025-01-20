//
// Created by k0lt on 12/27/24.
//

#include "Card.h"
Card::Card(Suits passedSuit, int rankValue)
{
    suit = passedSuit;
    switch (rankValue)
    {
    case 0:
        rank = TWO;
        value = 0;
        break;
    case 1:
        rank = THREE;
        value = 0;
        break;
    case 2:
        rank = FOUR;
        value = 0;
        break;
    case 3:
        rank = FIVE;
        value = 0;
        break;
    case 4:
        rank = SIX;
        value = 0;
        break;
    case 5:
        rank = QUEEN;
        value = 2;
        break;
    case 6:
        rank = JACK;
        value = 3;
        break;
    case 7:
        rank = KING;
        value = 4;
        break;
    case 8:
        rank = SEVEN;
        value = 10;
        break;
    case 9:
        rank = ACE;
        value = 11;
        break;
    default:
        rank = NOVALUE;
    }
}
Suits Card::getSuit()
{
    return suit;
}

Ranks Card::getRank()
{
    return rank;
}

int Card::getValue()
{
    return value;
}

void Card::setSuit(Suits passedSuit)
{
    suit = passedSuit;
}

void Card::setRank(Ranks passedRank)
{
    rank = passedRank;
}

