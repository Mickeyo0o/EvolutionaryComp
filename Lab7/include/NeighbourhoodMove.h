#ifndef NEIGHBOURHOODMOVE_H
#define NEIGHBOURHOODMOVE_H
#include <string>
#include "CostDistanceInfo.h"

class NeighbourhoodMove
{
    public:
        NeighbourhoodMove();
        virtual ~NeighbourhoodMove();
        virtual int calculateFunctionDelta(const CostDistanceInfo* costDistanceInfo, const std::vector<int>& currentCycle, int* cyclePosCache) = 0;
        virtual void performMove(std::vector<int>& currentCycle, int* cyclePosCache) = 0;
        virtual std::string debugInfo() = 0;
        virtual bool isApplicable(const std::vector<int>& currentCycle, int* cyclePosCache) = 0;
        virtual bool shouldBeLeftInLM(const std::vector<int>& currentCycle, int* cyclePosCache) = 0; // For inter moves, this is the same as isApplicable, for intra moves, thess differ in one edge case
        virtual bool equals(const NeighbourhoodMove& other) = 0;
        virtual std::vector<int> getEnteringIds() = 0;
        bool operator == (const NeighbourhoodMove& other);
        int functionDelta;
};

#endif // NEIGHBOURHOODMOVE_H
