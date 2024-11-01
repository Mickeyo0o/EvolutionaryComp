#ifndef INTRANODECHANGEEDGENEIGHBOURHOODMOVELocalSearch_H
#define INTRANODECHANGEEDGENEIGHBOURHOODMOVELocalSearch_H
#include <algorithm>
#include "NeighbourhoodMove.h"

class IntraNodeChangeEdgeNeighbourhoodMoveLocalSearch: public NeighbourhoodMove
{
    public:
        IntraNodeChangeEdgeNeighbourhoodMoveLocalSearch(unsigned int cyclePos1EdgeStart, unsigned int cyclePos2EdgeStart);
        virtual ~IntraNodeChangeEdgeNeighbourhoodMoveLocalSearch();
        int calculateFunctionDelta(const CostDistanceInfo* costDistanceInfo, const std::vector<int>& currentCycle, int* posInCycleCache);
        void performMove(std::vector<int>& currentCycle, int* posInCycleCache);
        std::string debugInfo()
        {
            return "Intra Node Change Edge " + std::to_string(cyclePos1EdgeStart) + " " + std::to_string(cyclePos2EdgeStart);
        }
        static std::vector<NeighbourhoodMove*> generateAllMoves(const std::vector<int>& currentCycle, const CostDistanceInfo* costDistanceInfo);
    private:
        unsigned int cyclePos1EdgeStart, cyclePos2EdgeStart;
};

#endif // INTRANODECHANGEEDGENEIGHBOURHOODMOVELocalSearch_H
