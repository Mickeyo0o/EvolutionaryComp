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
    size_t endReverseOffset = cyclePosCache[node2EdgeStart];
    int n = currentCycle.size();
    if (n == 0) return;

    startReverseOffset %= n;
    endReverseOffset %= n;
    while (true)
    {
        std::swap(currentCycle[startReverseOffset], currentCycle[endReverseOffset]);
        startReverseOffset = (startReverseOffset + 1) % n;
        if (startReverseOffset == endReverseOffset) break;

        endReverseOffset = (endReverseOffset - 1 + n) % n;
        if (startReverseOffset == endReverseOffset) break;
    }
    for(int posInCycle = 0; posInCycle < currentCycle.size(); posInCycle++)
    {
        cyclePosCache[currentCycle[posInCycle]] = posInCycle;
    }
}

std::vector<NeighbourhoodMove*> IntraNodeChangeEdgeNeighbourhoodMove::generateAllMoves(const std::vector<int>& currentCycle, const CostDistanceInfo* costDistanceInfo, int* cyclePosCache)
{
    std::vector<NeighbourhoodMove*> ret;
    size_t sizeOfRet = (currentCycle.size() * currentCycle.size());
    ret.reserve(sizeOfRet);

    for(int node1 = 0; node1 < costDistanceInfo->getNumOfNodes(); node1++)
    {
        int node1CyclePos = cyclePosCache[node1];
        if(node1CyclePos == -1) continue; //node1 not in cycle, skip

        for(int node2 = 0; node2 < costDistanceInfo->getNumOfNodes(); node2++)
        {
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


bool IntraNodeChangeEdgeNeighbourhoodMove::isApplicable(const std::vector<int>& currentCycle, int* cyclePosCache)
{
    n1cs = cyclePosCache[node1EdgeStart];
    n2cs = cyclePosCache[node2EdgeStart];
    int edge1StartPos = cyclePosCache[node1EdgeStart];
    int edge1EndPos = cyclePosCache[node1EdgeEnd];
    int edge2StartPos = cyclePosCache[node2EdgeStart];
    int edge2EndPos = cyclePosCache[node2EdgeEnd];

   // std::cout << edge1StartPos << "->" << edge1EndPos << " " << edge2StartPos << "->" << edge2EndPos << std::endl;

    bool areAllNodesInCycle = (edge1StartPos != -1 && edge1EndPos != -1 && edge2StartPos != -1 && edge2EndPos != -1);
    int diffEdge1 = edge1EndPos - edge1StartPos;
    int diffEdge2 = edge2EndPos - edge2StartPos;
    return areAllNodesInCycle && ((diffEdge1 == 1 && diffEdge2 == 1) || (diffEdge1 == -1 && diffEdge2 == -1));
}

bool IntraNodeChangeEdgeNeighbourhoodMove::shouldBeLeftInLM(const std::vector<int>& currentCycle, int* cyclePosCache)
{
    int edge1StartPos = cyclePosCache[node1EdgeStart];
    int edge1EndPos = cyclePosCache[node1EdgeEnd];
    int edge2StartPos = cyclePosCache[node2EdgeStart];
    int edge2EndPos = cyclePosCache[node2EdgeEnd];

    bool areAllNodesInCycle = !(edge1StartPos == -1 || edge1EndPos == -1 || edge2StartPos == -1 || edge2EndPos == -1);
    int diffEdge1 = edge1EndPos - edge1StartPos;
    int diffEdge2 = edge2EndPos - edge2StartPos;

    return areAllNodesInCycle && ((diffEdge1 * diffEdge2) == -1 || (diffEdge1 == -1 && diffEdge2 == -1));
}

std::vector<int> IntraNodeChangeEdgeNeighbourhoodMove::getEnteringIds()
{
    std::vector<int> ret;
    ret.push_back(node1EdgeStart);
    ret.push_back(node2EdgeStart);
    ret.push_back(node1EdgeEnd);
    ret.push_back(node2EdgeEnd);
    return ret;
}


bool IntraNodeChangeEdgeNeighbourhoodMove::equals(const NeighbourhoodMove& other)
{
    const IntraNodeChangeEdgeNeighbourhoodMove* otherN = dynamic_cast<const IntraNodeChangeEdgeNeighbourhoodMove*>(&other);
    if(otherN == NULL) return false;
    return this->node1EdgeEnd == otherN->node1EdgeEnd && this->node2EdgeEnd == otherN->node2EdgeEnd &&
            this->node1EdgeStart == otherN->node1EdgeStart && this->node2EdgeStart == otherN->node2EdgeStart;
}
