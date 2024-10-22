#ifndef NEIGHBOURHOODMOVE_H
#define NEIGHBOURHOODMOVE_H
#include <string>
#include "CostDistanceInfo.h"

class NeighbourhoodMove
{
    public:
        NeighbourhoodMove();
        virtual ~NeighbourhoodMove();
        virtual int calculateFunctionDelta(const CostDistanceInfo* costDistanceInfo, const std::vector<int>& currentCycle) = 0;
        virtual void performMove(std::vector<int>& currentCycle) = 0;
        virtual std::string debugInfo() = 0;
};

#endif // NEIGHBOURHOODMOVE_H
