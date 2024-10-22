#ifndef INTRANODECHANGEEDGENEIGHBOURHOODMOVE_H
#define INTRANODECHANGEEDGENEIGHBOURHOODMOVE_H
#include <algorithm>
#include "NeighbourhoodMove.h"

class IntraNodeChangeEdgeNeighbourhoodMove: public NeighbourhoodMove
{
    public:
        IntraNodeChangeEdgeNeighbourhoodMove(unsigned int cyclePos1EdgeStart, unsigned int cyclePos2EdgeStart);
        virtual ~IntraNodeChangeEdgeNeighbourhoodMove();
        int calculateFunctionDelta(const CostDistanceInfo* costDistanceInfo, const std::vector<int>& currentCycle);
        void performMove(std::vector<int>& currentCycle);
        std::string debugInfo()
        {
            return "Intra Node Change Edge " + std::to_string(cyclePos1EdgeStart) + " " + std::to_string(cyclePos2EdgeStart);
        }
        static std::vector<NeighbourhoodMove*> generateAllMoves(const std::vector<int>& currentCycle, const CostDistanceInfo* costDistanceInfo);
    private:
        unsigned int cyclePos1EdgeStart, cyclePos2EdgeStart;
};

#endif // INTRANODECHANGEEDGENEIGHBOURHOODMOVE_H
