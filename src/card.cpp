#include "card.h"

Rectangle Card::GetSourceRec(float slotWidth, float slotHeight) const
{
	int xIndex = 0;
	int yIndex = 0;

    switch (suit)
    {
    case Suit::Diamonds: yIndex = 0; break;
    case Suit::Clubs:    yIndex = 1; break; 
    case Suit::Hearts:   yIndex = 2; break;
    case Suit::Spades:   yIndex = 3; break;
    }


    if (isFaceUp)
    {
        xIndex = static_cast<int>(rank) - 1;
    }
    else
    {
        // default card back
        yIndex = 2;
        xIndex = 15;
    }

    return Rectangle {
        xIndex * slotWidth,
        yIndex * slotHeight,
        slotWidth,
        slotHeight
    };
}