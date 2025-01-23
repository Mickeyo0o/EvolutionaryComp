#include "IntraNodeChangeEdgeNeighbourhoodMove.h"

IntraNodeChangeEdgeNeighbourhoodMove::IntraNodeChangeEdgeNeighbourhoodMove(unsigned int node1EdgeStart, unsigned int node2EdgeStart, unsigned int node1EdgeEnd, unsigned int node2EdgeEnd):
    node1EdgeStart(node1EdgeStart), node2EdgeStart(node2EdgeStart), node1EdgeEnd(node1EdgeEnd), node2EdgeEnd(node2EdgeEnd)
{
    //ctor
}

IntraNodeChangeEdgeNeighbourhoodMove::~IntraNodeChangeEdgeNeighbourhoodMove()
{
    //dtor
}

int IntraNodeChangeEdgeNeighbourhoodMove::calculateFunctionDelta(const CostDistanceInfo* costDistanceInfo, const std::vector<int>& currentCycle, int* cyclePosCache)
{
    int currentValue = costDistanceInfo->getDistance(node1EdgeStart, node1EdgeEnd) + costDistanceInfo->getDistance(node2EdgeStart, node2EdgeEnd);
    int newPossibleValue = costDistanceInfo->getDistance(node1EdgeEnd, node2EdgeEnd) + costDistanceInfo->getDistance(node1EdgeStart, node2EdgeStart);
    return newPossibleValue - currentValue;
}

void IntraNodeChangeEdgeNeighbourhoodMove::performMove(std::vector<int>& currentCycle, int* cyclePosCache)
{
    size_t startReverseOffset = cyclePosCache[node1EdgeEnd];
    int cyclePos2EdgeStart = cyclePosCache[node2EdgeStart];
    if(cyclePos2EdgeStart == (currentCycle.size() - 1))
    {
        std::reverse(currentCycle.begin() + startReverseOffset, currentCycle.end());
        for(int posInCycle = startReverseOffset; posInCycle < currentCycle.size(); posInCycle++)
        {
            cyclePosCache[currentCycle[posInCycle]] = posInCycle;
        }
    }
    else
    {
        size_t endReverseOfsset = cyclePosCache[node2EdgeEnd];
        size_t start_o = std::min(startReverseOffset, endReverseOfsset);
        size_t end_o = std::max(endReverseOfsset, startReverseOffset);
        std::reverse(currentCycle.begin() + start_o, currentCycle.begin() + end_o);
        for(int posInCycle = start_o; posInCycle < end_o; posInCycle++)
        {
            cyclePosCache[currentCycle[posInCycle]] = posInCycle;
        }
    }
}

std::vector<NeighbourhoodMove*> IntraNodeChangeEdgeNeighbourhoodMove::generateAllMoves(const std::vector<int>& currentCycle, const CostDistanceInfo* costDistanceInfo, int* cyclePosCache, int** nearestNeighbours, int nearestNeighboursCount)
{
    std::vector<NeighbourhoodMove*> ret;
    size_t sizeOfRet = (currentCycle.size() * nearestNeighboursCount) * 2;
    ret.reserve(sizeOfRet);

    for(int node1 = 0; node1 < costDistanceInfo->getNumOfNodes(); node1++)
    {
        int node1CyclePos = cyclePosCache[node1];
        if(node1CyclePos == -1) continue; //node1 not in cycle, skip

        for(int nnId = 0; nnId < nearestNeighboursCount; nnId++)
        {
            int node2 = nearestNeighbours[node1][nnId];
            int node2CyclePos = cyclePosCache[node2];

            if(node2CyclePos == -1) continue; // nearest neighbour not in cycle, skip

            int node1PrevCyclePos = node1CyclePos == 0 ? currentCycle.size() - 1 : node1CyclePos - 1;
            int node1Prev = currentCycle[node1PrevCyclePos];

            int node1SuccCyclePos = (node1CyclePos + 1) % currentCycle.size();
            int node1Succ = currentCycle[node1SuccCyclePos];

            int node2PrevCyclePos = node2CyclePos == 0 ? currentCycle.size() - 1 : node2CyclePos - 1;
            int node2Prev = currentCycle[node2PrevCyclePos];

            int node2SuccCyclePos = (node2CyclePos + 1) % currentCycle.size();
            int node2Succ = currentCycle[node2SuccCyclePos];

            ret.push_back(new IntraNodeChangeEdgeNeighbourhoodMove(node1, node2, node1Succ, node2Succ));
            ret.push_back(new IntraNodeChangeEdgeNeighbourhoodMove(node1Prev, node2Prev, node1, node2)); // Add both move possibilities
        }
    }

    return ret;
}
