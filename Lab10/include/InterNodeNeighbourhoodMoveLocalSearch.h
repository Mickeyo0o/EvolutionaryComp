#ifndef INTERNODENEIGHBOURHOODMOVELocalSearch_H
#define INTERNODENEIGHBOURHOODMOVELocalSearch_H
#include <unordered_set>
#include <iostream>
#include <algorithm>
#include "NeighbourhoodMove.h"

class InterNodeNeighbourhoodMoveLocalSearch: public NeighbourhoodMove
{
    public:
        InterNodeNeighbourhoodMoveLocalSearch(unsigned int cyclePos, unsigned int nodeToAddId);
        virtual ~InterNodeNeighbourhoodMoveLocalSearch();
        int calculateFunctionDelta(const CostDistanceInfo* costDistanceInfo, const std::vector<int>& currentCycle, int* cyclePosCache);
        void performMove(std::vector<int>& currentCycle, int* cyclePosCache);
        std::string debugInfo()
        {
            return "Inter Node " + std::to_string(cyclePos) + " " + std::to_string(nodeToAddId);
        }
        static std::vector<NeighbourhoodMove*> generateAllMoves(const std::vector<int>& currentCycle, const CostDistanceInfo* costDistanceInfo);
    private:
        unsigned int cyclePos, nodeToAddId;
};

#endif // INTERNODENEIGHBOURHOODMOVELocalSearch_H
