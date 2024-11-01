#include "InterNodeNeighbourhoodMove.h"

InterNodeNeighbourhoodMove::InterNodeNeighbourhoodMove(unsigned int nodeInCycleId, unsigned int nodeToAddId):
    nodeInCycleId(nodeInCycleId), nodeToAddId(nodeToAddId)
{
    //ctor
}

InterNodeNeighbourhoodMove::~InterNodeNeighbourhoodMove()
{
    //dtor
}

int InterNodeNeighbourhoodMove::calculateFunctionDelta(const CostDistanceInfo* costDistanceInfo, const std::vector<int>& currentCycle, int* cyclePosCache)
{
    int cyclePos = cyclePosCache[nodeInCycleId];
    int prevCyclePos = cyclePos - 1;
    if(cyclePos == 0)
    {
        prevCyclePos = currentCycle.size() - 1;
    }
    int succCyclePos = (cyclePos + 1) % currentCycle.size();
    unsigned int succId = currentCycle[succCyclePos];
    unsigned int prevId = currentCycle[prevCyclePos];

    int currValue = costDistanceInfo->getNodeCost(nodeInCycleId) + costDistanceInfo->getDistance(nodeInCycleId, prevId) + costDistanceInfo->getDistance(nodeInCycleId, succId);
    int newPossibleValue = costDistanceInfo->getNodeCost(nodeToAddId) + costDistanceInfo->getDistance(nodeToAddId, prevId) + costDistanceInfo->getDistance(nodeToAddId, succId);
    return newPossibleValue - currValue;
}

void InterNodeNeighbourhoodMove::performMove(std::vector<int>& currentCycle, int* cyclePosCache)
{
    int cyclePos = cyclePosCache[nodeInCycleId];
    currentCycle[cyclePos] = nodeToAddId;
    cyclePosCache[nodeInCycleId] = -1;
    cyclePosCache[nodeToAddId] = cyclePos;
}

std::vector<NeighbourhoodMove*> InterNodeNeighbourhoodMove::generateAllMoves(const std::vector<int>& currentCycle, const CostDistanceInfo* costDistanceInfo, int* cyclePosCache, int** nearestNeighbours, int nearestNeighboursCount)
{
    std::vector<int> nodesNotInCycle;
    for(int n = 0; n < costDistanceInfo->getNumOfNodes(); n++)
    {
        if(std::find(currentCycle.begin(), currentCycle.end(), n) == currentCycle.end())
        {
            nodesNotInCycle.push_back(n);
        }
    }
    std::vector<NeighbourhoodMove*> ret;
    size_t nodesInCycle = currentCycle.size();
    ret.reserve((currentCycle.size() - nodesInCycle) * nearestNeighboursCount);
    for(int nodeToAddId: nodesNotInCycle)
    {
        for(int nearestNeighbourId = 0; nearestNeighbourId < nearestNeighboursCount; nearestNeighbourId++)
        {
            int nearestNeighbour = nearestNeighbours[nodeToAddId][nearestNeighbourId];
            int nearestNeighbourCyclePos = cyclePosCache[nearestNeighbour];
            if(nearestNeighbourCyclePos != -1) // NN is in cycle, we can add nodeToAdd either before or after it
            {
                int nearestNeighbourPrevPos = nearestNeighbourCyclePos - 1;
                if(nearestNeighbourCyclePos == 0)
                {
                    nearestNeighbourPrevPos = currentCycle.size() - 1;
                }
                int nearestNeighbourPrev = currentCycle[nearestNeighbourPrevPos];

                int nearestNeighbourSuccPos = (nearestNeighbourCyclePos + 1) % currentCycle.size();
                int nearestNeighbourSucc = currentCycle[nearestNeighbourSuccPos];

                ret.push_back(new InterNodeNeighbourhoodMove(nearestNeighbourPrev, nodeToAddId));
                ret.push_back(new InterNodeNeighbourhoodMove(nearestNeighbourSucc, nodeToAddId));
            }
        }
    }

    return ret;
}
