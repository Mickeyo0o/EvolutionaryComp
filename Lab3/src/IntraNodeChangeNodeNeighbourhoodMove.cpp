#include "IntraNodeChangeNodeNeighbourhoodMove.h"

IntraNodeChangeNodeNeighbourhoodMove::IntraNodeChangeNodeNeighbourhoodMove(unsigned int cyclePos1, unsigned int cyclePos2): cyclePos1(cyclePos1), cyclePos2(cyclePos2)
{
    //ctor
}

IntraNodeChangeNodeNeighbourhoodMove::~IntraNodeChangeNodeNeighbourhoodMove()
{
    //dtor
}

int IntraNodeChangeNodeNeighbourhoodMove::calculateFunctionDelta(const CostDistanceInfo* costDistanceInfo, const std::vector<int>& currentCycle)
{
    int node1Id = currentCycle[cyclePos1];
    int node2Id = currentCycle[cyclePos2];
    int lN1CyclePos = cyclePos1 - 1;
    if(cyclePos1 == 0)
    {
        lN1CyclePos = currentCycle.size() - 1;
    }
    int rN2CyclePos = (cyclePos2 + 1) % currentCycle.size();
    int lN1Id = currentCycle[lN1CyclePos];
    int rN2Id = currentCycle[rN2CyclePos];

    int lN2CyclePos = cyclePos2 - 1;
    if(cyclePos2 == 0)
    {
        lN2CyclePos = currentCycle.size() - 1;
    }
    int rN1CyclePos = (cyclePos1 + 1) % currentCycle.size();

    int lN2Id = currentCycle[lN2CyclePos];
    int rN1Id = currentCycle[rN1CyclePos];

    if(cyclePos1 == lN2CyclePos)
    {
        int currentValue = costDistanceInfo->getDistance(node1Id, lN1Id) + costDistanceInfo->getDistance(node2Id, rN2Id);
        int newPossibleValue = costDistanceInfo->getDistance(node2Id, lN1Id) + costDistanceInfo->getDistance(node1Id, rN2Id);
        return newPossibleValue - currentValue;
    }

    else if(cyclePos1 == rN2CyclePos)
    {
        int currentValue = costDistanceInfo->getDistance(node2Id, lN2Id) + costDistanceInfo->getDistance(node1Id, rN1Id);
        int newPossibleValue = costDistanceInfo->getDistance(node1Id, lN2Id) + costDistanceInfo->getDistance(node2Id, rN1Id);
        return newPossibleValue - currentValue;
    }


    int currentValue = costDistanceInfo->getDistance(node1Id, lN1Id) + costDistanceInfo->getDistance(node1Id, rN1Id) +
                        costDistanceInfo->getDistance(node2Id, lN2Id) + costDistanceInfo->getDistance(node2Id, rN2Id);

    int newPossibleValue = costDistanceInfo->getDistance(node2Id, lN1Id) + costDistanceInfo->getDistance(node2Id, rN1Id) +
                        costDistanceInfo->getDistance(node1Id, lN2Id) + costDistanceInfo->getDistance(node1Id, rN2Id);

    return newPossibleValue - currentValue;
}

void IntraNodeChangeNodeNeighbourhoodMove::performMove(std::vector<int>& currentCycle)
{
    int node1Id = currentCycle[cyclePos1];
    int node2Id = currentCycle[cyclePos2];

    currentCycle[cyclePos1] = node2Id;
    currentCycle[cyclePos2] = node1Id;
}

std::vector<NeighbourhoodMove*> IntraNodeChangeNodeNeighbourhoodMove::generateAllMoves(const std::vector<int>& currentCycle, const CostDistanceInfo* costDistanceInfo)
{
    std::vector<NeighbourhoodMove*> ret;
    size_t sizeOfRet = (currentCycle.size() * (currentCycle.size() - 1)) / 2;
    ret.reserve(sizeOfRet);

    for(int pos1InCycle = 0; pos1InCycle < currentCycle.size(); pos1InCycle++)
    {
        for(int pos2InCycle = pos1InCycle + 1; pos2InCycle < currentCycle.size(); pos2InCycle++)
        {
            ret.push_back(new IntraNodeChangeNodeNeighbourhoodMove(pos1InCycle, pos2InCycle));
        }
    }

    return ret;
}
