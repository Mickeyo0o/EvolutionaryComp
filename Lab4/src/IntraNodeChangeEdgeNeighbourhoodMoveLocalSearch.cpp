#include "IntraNodeChangeEdgeNeighbourhoodMoveLocalSearch.h"

IntraNodeChangeEdgeNeighbourhoodMoveLocalSearch::IntraNodeChangeEdgeNeighbourhoodMoveLocalSearch(unsigned int cyclePos1EdgeStart, unsigned int cyclePos2EdgeStart):
    cyclePos1EdgeStart(cyclePos1EdgeStart), cyclePos2EdgeStart(cyclePos2EdgeStart)
{
    //ctor
}

IntraNodeChangeEdgeNeighbourhoodMoveLocalSearch::~IntraNodeChangeEdgeNeighbourhoodMoveLocalSearch()
{
    //dtor
}

int IntraNodeChangeEdgeNeighbourhoodMoveLocalSearch::calculateFunctionDelta(const CostDistanceInfo* costDistanceInfo, const std::vector<int>& currentCycle, int* posInCycleCache)
{
    int cyclePos1EdgeEnd = (cyclePos1EdgeStart + 1) % currentCycle.size();
    int cyclePos2EdgeEnd = (cyclePos2EdgeStart + 1) % currentCycle.size();

    int node1StartId = currentCycle[cyclePos1EdgeStart];
    int node2StartId = currentCycle[cyclePos2EdgeStart];
    int node1EndId = currentCycle[cyclePos1EdgeEnd];
    int node2EndId = currentCycle[cyclePos2EdgeEnd];

    int currentValue = costDistanceInfo->getDistance(node1StartId, node1EndId) + costDistanceInfo->getDistance(node2StartId, node2EndId);
    int newPossibleValue = costDistanceInfo->getDistance(node1EndId, node2EndId) + costDistanceInfo->getDistance(node1StartId, node2StartId);
    return newPossibleValue - currentValue;
}

void IntraNodeChangeEdgeNeighbourhoodMoveLocalSearch::performMove(std::vector<int>& currentCycle, int* posInCycleCache)
{
    size_t startReverseOffset = (cyclePos1EdgeStart + 1) % currentCycle.size();
    if(cyclePos2EdgeStart == (currentCycle.size() - 1))
    {
        std::reverse(currentCycle.begin() + startReverseOffset, currentCycle.end());
    }
    else
    {
        size_t endReverseOfsset = (cyclePos2EdgeStart + 1) % currentCycle.size(); // End range is exclusive, start is inclusive
        std::reverse(currentCycle.begin() + startReverseOffset, currentCycle.begin() + endReverseOfsset);
    }
}

std::vector<NeighbourhoodMove*> IntraNodeChangeEdgeNeighbourhoodMoveLocalSearch::generateAllMoves(const std::vector<int>& currentCycle, const CostDistanceInfo* costDistanceInfo)
{
    std::vector<NeighbourhoodMove*> ret;
    size_t sizeOfRet = (currentCycle.size() * (currentCycle.size() - 3)) / 2;
    ret.reserve(sizeOfRet);

    for(int posInCycle1 = 0; posInCycle1 < currentCycle.size() - 3; posInCycle1++)
    {
        for(int posInCycle2 = posInCycle1 + 2; posInCycle2 < currentCycle.size(); posInCycle2++)
        {
            ret.push_back(new IntraNodeChangeEdgeNeighbourhoodMoveLocalSearch(posInCycle1, posInCycle2));
        }
    }

    return ret;
}
