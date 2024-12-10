#ifndef INTRANODECHANGENODENEIGHBOURHOODMOVE_H
#define INTRANODECHANGENODENEIGHBOURHOODMOVE_H
#include <iostream>
#include "NeighbourhoodMove.h"

class IntraNodeChangeNodeNeighbourhoodMove: public NeighbourhoodMove
{
    public:
        IntraNodeChangeNodeNeighbourhoodMove(unsigned int cyclePos1, unsigned int cyclePos2);
        virtual ~IntraNodeChangeNodeNeighbourhoodMove();
        int calculateFunctionDelta(const CostDistanceInfo* costDistanceInfo, const std::vector<int>& currentCycle);
        void performMove(std::vector<int>& currentCycle);
        std::string debugInfo()
        {
            return "Intra Node Change Node " + std::to_string(cyclePos1) + " " + std::to_string(cyclePos2);
        }
        static std::vector<NeighbourhoodMove*> generateAllMoves(const std::vector<int>& currentCycle, const CostDistanceInfo* costDistanceInfo);
    private:
        unsigned int cyclePos1, cyclePos2;
};

#endif // INTRANODECHANGENODENEIGHBOURHOODMOVE_H
