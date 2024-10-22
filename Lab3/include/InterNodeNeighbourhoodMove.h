#ifndef INTERNODENEIGHBOURHOODMOVE_H
#define INTERNODENEIGHBOURHOODMOVE_H
#include <unordered_set>
#include <iostream>
#include <algorithm>
#include "NeighbourhoodMove.h"

class InterNodeNeighbourhoodMove: public NeighbourhoodMove
{
    public:
        InterNodeNeighbourhoodMove(unsigned int cyclePos, unsigned int nodeToAddId);
        virtual ~InterNodeNeighbourhoodMove();
        int calculateFunctionDelta(const CostDistanceInfo* costDistanceInfo, const std::vector<int>& currentCycle);
        void performMove(std::vector<int>& currentCycle);
        std::string debugInfo()
        {
            return "Inter Node " + std::to_string(cyclePos) + " " + std::to_string(nodeToAddId);
        }
        static std::vector<NeighbourhoodMove*> generateAllMoves(const std::vector<int>& currentCycle, const CostDistanceInfo* costDistanceInfo);
    private:
        unsigned int cyclePos, nodeToAddId;
};

#endif // INTERNODENEIGHBOURHOODMOVE_H
