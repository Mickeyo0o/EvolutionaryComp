#include "LocalSearchGenerator.h"

LocalSearchGenerator::LocalSearchGenerator(const CostDistanceInfo* costDistanceInfo, std::vector<int> initialCycle, bool isGreedy, bool swapEdges, unsigned int seed):
    Generator(costDistanceInfo, initialCycle.size()), initialCycle(initialCycle), isGreedy(isGreedy), seed(seed), swapEdges(swapEdges)
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
    if(swapEdges)
    {
        intraMoves = IntraNodeChangeEdgeNeighbourhoodMove::generateAllMoves(ret, costDistanceInfo);
    }
    else
    {
        intraMoves = IntraNodeChangeNodeNeighbourhoodMove::generateAllMoves(ret, costDistanceInfo);
    }

    do
    {
        improvementAchievedLastIter = false;
        std::vector<NeighbourhoodMove*> possibleMoves;

        std::vector<NeighbourhoodMove*> interMoves = InterNodeNeighbourhoodMove::generateAllMoves(ret, costDistanceInfo);

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

        std::shuffle(possibleMoves.begin(), possibleMoves.end(), std::default_random_engine(seed));
        seed++;

        int bestIncreaseInThisIter = 0; // Negative value means there is improvement
        NeighbourhoodMove* bestMoveinThisIter = nullptr;

        for(NeighbourhoodMove* currentMove: possibleMoves)
        {
            int increaseForMove = currentMove->calculateFunctionDelta(costDistanceInfo, ret);
            if(increaseForMove < bestIncreaseInThisIter)
            {
                improvementAchievedLastIter = true;
                bestIncreaseInThisIter = increaseForMove;
                bestMoveinThisIter = currentMove;
                if(isGreedy) break;
            }
        }

        if(bestMoveinThisIter != nullptr)
        {
            bestMoveinThisIter->performMove(ret);
        }
        for(NeighbourhoodMove* neighbourhoodMove: interMoves)
        {
            delete neighbourhoodMove;
        }

    } while(improvementAchievedLastIter);

    for(NeighbourhoodMove* neighbourhoodMove: intraMoves)
    {
        delete neighbourhoodMove;
    }

    return ret;
}
