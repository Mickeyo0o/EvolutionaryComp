#include "LocalSearchCandidateMovesGenerator.h"

LocalSearchCandidateMovesGenerator::LocalSearchCandidateMovesGenerator(const CostDistanceInfo* costDistanceInfo, int nodesInCycle, std::vector<int> startingCycle):
    Generator(costDistanceInfo, nodesInCycle), startingCycle(startingCycle)
{
    init();
}

LocalSearchCandidateMovesGenerator::LocalSearchCandidateMovesGenerator(const CostDistanceInfo* costDistanceInfo, int nodesInCycle,  std::vector<int> startingCycle, unsigned int numOfNearestNeighbours):
    Generator(costDistanceInfo, nodesInCycle), startingCycle(startingCycle), numOfNearestNeighbours(numOfNearestNeighbours)
{
    init();
}

LocalSearchCandidateMovesGenerator::~LocalSearchCandidateMovesGenerator()
{
    for(size_t i = 0; i < costDistanceInfo->getNumOfNodes(); i++)
    {
        delete [] nearestNeighboursPerNode[i];
    }
    delete [] nearestNeighboursPerNode;
}

void LocalSearchCandidateMovesGenerator::init()
{
    nearestNeighboursPerNode = new int*[costDistanceInfo->getNumOfNodes()];
    for(unsigned int i = 0; i < costDistanceInfo->getNumOfNodes(); i++)
    {
        nearestNeighboursPerNode[i] = new int[numOfNearestNeighbours];
    }

    for(size_t n = 0; n < costDistanceInfo->getNumOfNodes(); n++)
    {
        std::pair<int, int>* bestDistances = new std::pair<int, int>[costDistanceInfo->getNumOfNodes()];
        for(size_t nNeighbour = 0; nNeighbour < costDistanceInfo->getNumOfNodes(); nNeighbour++)
        {
            bestDistances[nNeighbour] = std::pair<int, int>(nNeighbour, costDistanceInfo->getDistance(n, nNeighbour) + costDistanceInfo->getNodeCost(nNeighbour));
        }
        std::sort(bestDistances, bestDistances + costDistanceInfo->getNumOfNodes(), [](std::pair<int, int> p1, std::pair<int, int> p2){
            return p1.second < p2.second;
        });
        int addedNodes = 0;
        int i = 0;
        while(addedNodes < numOfNearestNeighbours)
        {
            if(bestDistances[i].first != n)
            {
                nearestNeighboursPerNode[n][addedNodes] = bestDistances[i].first;
                addedNodes++;
            }
            i++;
        }
        delete [] bestDistances;
    }
    positionInCycleCache = new int[costDistanceInfo->getNumOfNodes()];
    int posInCycle = 0;
    for(size_t i = 0; i < costDistanceInfo->getNumOfNodes(); i++)
    {
        positionInCycleCache[i] = -1; //-1 means element at i is not in the cycle
    }
    for(int elInCycle: startingCycle)
    {
        positionInCycleCache[elInCycle] = posInCycle;
        posInCycle++;
    }
}

std::vector<int> LocalSearchCandidateMovesGenerator::generateCycle(int start_pos)
{
    std::vector<int> ret;
    for(int n: startingCycle)
    {
        ret.push_back(n);
    }
    bool improvementAchievedLastIter = false;

    do
    {
        auto start_time = std::chrono::high_resolution_clock::now();
        improvementAchievedLastIter = false;
        std::vector<NeighbourhoodMove*> possibleMoves;

        std::vector<NeighbourhoodMove*> interMoves = InterNodeNeighbourhoodMove::generateAllMoves(ret, costDistanceInfo, positionInCycleCache, nearestNeighboursPerNode, numOfNearestNeighbours);
        std::vector<NeighbourhoodMove*> intraMoves = IntraNodeChangeEdgeNeighbourhoodMove::generateAllMoves(ret, costDistanceInfo, positionInCycleCache, nearestNeighboursPerNode, numOfNearestNeighbours);

        possibleMoves.reserve(interMoves.size() + intraMoves.size());

        possibleMoves.insert(
            possibleMoves.begin(),
            interMoves.begin(),
            interMoves.end()
        );

        possibleMoves.insert(
            possibleMoves.end(),
            intraMoves.begin(),
            intraMoves.end()
        );

        int bestIncreaseInThisIter = 0; // Negative value means there is improvement
        NeighbourhoodMove* bestMoveinThisIter = nullptr;

        for(NeighbourhoodMove* currentMove: possibleMoves)
        {
            int increaseForMove = currentMove->calculateFunctionDelta(costDistanceInfo, ret, positionInCycleCache);
            if(increaseForMove < bestIncreaseInThisIter)
            {
                improvementAchievedLastIter = true;
                bestIncreaseInThisIter = increaseForMove;
                bestMoveinThisIter = currentMove;
            }
        }

        if(bestMoveinThisIter != nullptr)
        {
            bestMoveinThisIter->performMove(ret, positionInCycleCache);
        }
        for(NeighbourhoodMove* neighbourhoodMove: possibleMoves)
        {
            delete neighbourhoodMove;
        }

        auto end_time = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> timePeriod = end_time - start_time;

       // std::cout << "Step took " << timePeriod.count() << "ms" << std::endl;

    } while(improvementAchievedLastIter);

    return ret;
}
