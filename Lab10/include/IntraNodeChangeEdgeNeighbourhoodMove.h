#ifndef INTRANODECHANGEEDGENEIGHBOURHOODMOVE_H
#define INTRANODECHANGEEDGENEIGHBOURHOODMOVE_H
#include <algorithm>
#include "NeighbourhoodMove.h"

class IntraNodeChangeEdgeNeighbourhoodMove: public NeighbourhoodMove
{
    public:
        IntraNodeChangeEdgeNeighbourhoodMove(unsigned int node1EdgeStart, unsigned int node2EdgeStart, unsigned int node1EdgeEnd, unsigned int node2EdgeEnd);
        virtual ~IntraNodeChangeEdgeNeighbourhoodMove();
        int calculateFunctionDelta(const CostDistanceInfo* costDistanceInfo, const std::vector<int>& currentCycle, int* cyclePosCache);
        void performMove(std::vector<int>& currentCycle, int* cyclePosCache);
        std::string debugInfo()
        {
            return "Intra Node Change Edge " + std::to_string(node1EdgeStart) + "->" + std::to_string(node1EdgeEnd) + " " +
                    std::to_string(node2EdgeStart) + "->" + std::to_string(node2EdgeEnd);
        }
        static std::vector<NeighbourhoodMove*> generateAllMoves(const std::vector<int>& currentCycle, const CostDistanceInfo* costDistanceInfo, int* cyclePosCache, int** nearestNeighbours, int nearestNeighboursCount);
    private:
        unsigned int node1EdgeStart, node2EdgeStart, node1EdgeEnd, node2EdgeEnd;
};

#endif // INTRANODECHANGEEDGENEIGHBOURHOODMOVE_H
