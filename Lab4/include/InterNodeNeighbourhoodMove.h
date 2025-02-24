#ifndef INTERNODENEIGHBOURHOODMOVE_H
#define INTERNODENEIGHBOURHOODMOVE_H
#include <unordered_set>
#include <iostream>
#include <algorithm>
#include "NeighbourhoodMove.h"

class InterNodeNeighbourhoodMove: public NeighbourhoodMove
{
    public:
        InterNodeNeighbourhoodMove(unsigned int nodeInCycleId, unsigned int nodeToAddId);
        virtual ~InterNodeNeighbourhoodMove();
        int calculateFunctionDelta(const CostDistanceInfo* costDistanceInfo, const std::vector<int>& currentCycle, int* cyclePosCache);
        void performMove(std::vector<int>& currentCycle, int* cyclePosCache);
        std::string debugInfo()
        {
            return "Inter Node " + std::to_string(nodeInCycleId) + " " + std::to_string(nodeToAddId);
        }
        static std::vector<NeighbourhoodMove*> generateAllMoves(const std::vector<int>& currentCycle, const CostDistanceInfo* costDistanceInfo, int* cyclePosCache, int** nearestNeighbours, int nearestNeighboursCount);
    private:
        unsigned int nodeInCycleId, nodeToAddId;
};

#endif // INTERNODENEIGHBOURHOODMOVE_H
