#include "NeighbourhoodMove.h"

NeighbourhoodMove::NeighbourhoodMove()
{
    //ctor
}

NeighbourhoodMove::~NeighbourhoodMove()
{
    //dtor
}

bool NeighbourhoodMove::operator == (const NeighbourhoodMove& other)
{
    return this->equals(other);
}
