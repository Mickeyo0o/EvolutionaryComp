#include "InterNodeNeighbourhoodMove.h"

InterNodeNeighbourhoodMove::InterNodeNeighbourhoodMove(unsigned int nodeInCycleId, unsigned int nodeToAddId):
    nodeInCycleId(nodeInCycleId), nodeToAddId(nodeToAddId)
{

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

    nodeInCyclePrevAtCalcTime = prevId;
    nodeInCycleSuccAtCalcTime = succId;

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

std::vector<NeighbourhoodMove*> InterNodeNeighbourhoodMove::generateAllMoves(const std::vector<int>& currentCycle, const CostDistanceInfo* costDistanceInfo, int* cyclePosCache)
{
    std::vector<int> nodesNotInCycle;
    for(int n = 0; n < costDistanceInfo->getNumOfNodes(); n++)
    {
        if(cyclePosCache[n] == -1)
        {
            nodesNotInCycle.push_back(n);
        }
    }
    std::vector<NeighbourhoodMove*> ret;
    size_t nodesInCycle = currentCycle.size();
    size_t nodesRemaining = costDistanceInfo->getNumOfNodes() -  nodesInCycle;
    ret.reserve(nodesInCycle * nodesRemaining);
    for(int posInCycle: currentCycle)
    {
        for(int nodeIdToAdd: nodesNotInCycle)
        {
            ret.push_back(new InterNodeNeighbourhoodMove(posInCycle, nodeIdToAdd));
        }
    }
    return ret;
}

bool InterNodeNeighbourhoodMove::isApplicable(const std::vector<int>& currentCycle, int* cyclePosCache)
{
    int cyclePos = cyclePosCache[nodeInCycleId];
    if(cyclePos == -1)
    {
        return false;
    }
    int prevCyclePos = cyclePos - 1;
    if(cyclePos == 0)
    {
        prevCyclePos = currentCycle.size() - 1;
    }
    int succCyclePos = (cyclePos + 1) % currentCycle.size();
    //std::cout << cyclePos << " " << prevCyclePos << " " << succCyclePos << std::endl;
    unsigned int succId = currentCycle[succCyclePos];
    unsigned int prevId = currentCycle[prevCyclePos];
    bool isPrevTheSame = prevId == nodeInCyclePrevAtCalcTime;
    bool isSuccTheSame = succId == nodeInCycleSuccAtCalcTime;
    return cyclePosCache[this->nodeInCycleId] != -1 && cyclePosCache[this->nodeToAddId] == -1 && isPrevTheSame && isSuccTheSame;
}

bool InterNodeNeighbourhoodMove::shouldBeLeftInLM(const std::vector<int>& currentCycle, int* cyclePosCache)
{
    return isApplicable(currentCycle, cyclePosCache);
}

std::vector<int> InterNodeNeighbourhoodMove::getEnteringIds()
{
    std::vector<int> ret;
    ret.push_back(nodeToAddId);
    ret.push_back(nodeInCyclePrevAtCalcTime);
    ret.push_back(nodeInCycleSuccAtCalcTime);
    return ret;
}



bool InterNodeNeighbourhoodMove::equals(const NeighbourhoodMove& other)
{
    const InterNodeNeighbourhoodMove* otherN = dynamic_cast<const InterNodeNeighbourhoodMove*>(&other);
    if(otherN == NULL) return false;
    return this->nodeInCycleId == otherN->nodeInCycleId && this->nodeToAddId == otherN->nodeToAddId;
}

