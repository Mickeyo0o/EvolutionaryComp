#include "LocalSearchGenerator.h"

LocalSearchGenerator::LocalSearchGenerator(const CostDistanceInfo* costDistanceInfo, std::vector<int> initialCycle):
    Generator(costDistanceInfo, initialCycle.size()), initialCycle(initialCycle)
{
    //ctor
}

LocalSearchGenerator::~LocalSearchGenerator()
{
    //dtor
}

std::vector<int> LocalSearchGenerator::generateCycle(int start_pos)
{
    std::vector<int> ret;
    for(int nodeId: initialCycle)
    {
        ret.push_back(nodeId);
    }
    bool improvementAchievedLastIter = false;

    std::vector<NeighbourhoodMove*> intraMoves; // These don't change with this implementation
    intraMoves = IntraNodeChangeEdgeNeighbourhoodMoveLocalSearch::generateAllMoves(ret, costDistanceInfo);
    do
    {
        auto start_time = std::chrono::high_resolution_clock::now();
        improvementAchievedLastIter = false;
        std::vector<NeighbourhoodMove*> possibleMoves;

        std::vector<NeighbourhoodMove*> interMoves = InterNodeNeighbourhoodMoveLocalSearch::generateAllMoves(ret, costDistanceInfo);

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
            int increaseForMove = currentMove->calculateFunctionDelta(costDistanceInfo, ret, nullptr);
            if(increaseForMove < bestIncreaseInThisIter)
            {
                improvementAchievedLastIter = true;
                bestIncreaseInThisIter = increaseForMove;
                bestMoveinThisIter = currentMove;
            }
        }

        if(bestMoveinThisIter != nullptr)
        {
            bestMoveinThisIter->performMove(ret, nullptr);
        }
        for(NeighbourhoodMove* neighbourhoodMove: interMoves)
        {
            delete neighbourhoodMove;
        }

        auto end_time = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> timePeriod = end_time - start_time;
       // std::cout << "Step took " << timePeriod.count() << "ms" << std::endl;

    } while(improvementAchievedLastIter);
    for(NeighbourhoodMove* neighbourhoodMove: intraMoves)
    {
        delete neighbourhoodMove;
    }

    return ret;
}
