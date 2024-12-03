#include "InterNodeNeighbourhoodMoveLocalSearch.h"

InterNodeNeighbourhoodMoveLocalSearch::InterNodeNeighbourhoodMoveLocalSearch(unsigned int cyclePos, unsigned int nodeToAddId):
    cyclePos(cyclePos), nodeToAddId(nodeToAddId)
{
    //ctor
}

InterNodeNeighbourhoodMoveLocalSearch::~InterNodeNeighbourhoodMoveLocalSearch()
{
    //dtor
}

int InterNodeNeighbourhoodMoveLocalSearch::calculateFunctionDelta(const CostDistanceInfo* costDistanceInfo, const std::vector<int>& currentCycle, int* cyclePosCache)
{
    unsigned int nodeInCycleId = currentCycle[cyclePos];
    cacheNodeCycle = nodeInCycleId;
    int lNCyclePos = cyclePos - 1;
    if(cyclePos == 0)
    {
        lNCyclePos = currentCycle.size() - 1;
    }
    int rNCyclePos = (cyclePos + 1) % currentCycle.size();
    unsigned int rNId = currentCycle[rNCyclePos];
    unsigned int lNId = currentCycle[lNCyclePos];

    int currValue = costDistanceInfo->getNodeCost(nodeInCycleId) + costDistanceInfo->getDistance(nodeInCycleId, lNId) + costDistanceInfo->getDistance(nodeInCycleId, rNId);
    int newPossibleValue = costDistanceInfo->getNodeCost(nodeToAddId) + costDistanceInfo->getDistance(nodeToAddId, lNId) + costDistanceInfo->getDistance(nodeToAddId, rNId);
    return newPossibleValue - currValue;
}

void InterNodeNeighbourhoodMoveLocalSearch::performMove(std::vector<int>& currentCycle, int* cyclePosCache)
{
    currentCycle[cyclePos] = nodeToAddId;
}

std::vector<NeighbourhoodMove*> InterNodeNeighbourhoodMoveLocalSearch::generateAllMoves(const std::vector<int>& currentCycle, const CostDistanceInfo* costDistanceInfo)
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
    size_t nodesRemaining = costDistanceInfo->getNumOfNodes() -  nodesInCycle;
    ret.reserve(nodesInCycle * nodesRemaining);
    for(int posInCycle = 0; posInCycle < nodesInCycle; posInCycle++)
    {
        for(int nodeIdToAdd: nodesNotInCycle)
        {
            ret.push_back(new InterNodeNeighbourhoodMoveLocalSearch(posInCycle, nodeIdToAdd));
        }
    }
    return ret;
}


bool InterNodeNeighbourhoodMoveLocalSearch::isApplicable(const std::vector<int>& currentCycle, int* cyclePosCache)
{
    return true;
}

bool InterNodeNeighbourhoodMoveLocalSearch::shouldBeLeftInLM(const std::vector<int>& currentCycle, int* cyclePosCache)
{
    return isApplicable(currentCycle, cyclePosCache);
}

std::vector<int> InterNodeNeighbourhoodMoveLocalSearch::getEnteringIds()
{
    return std::vector<int>{};
}


bool InterNodeNeighbourhoodMoveLocalSearch::equals(const NeighbourhoodMove& other)
{
    const InterNodeNeighbourhoodMoveLocalSearch* otherN = dynamic_cast<const InterNodeNeighbourhoodMoveLocalSearch*>(&other);
    if(otherN == NULL) return false;
    return this->nodeToAddId == otherN->nodeToAddId && this->cyclePos == otherN->cyclePos;
}

