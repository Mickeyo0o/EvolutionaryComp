#include "IteratedLocalSearchGenerator.h"

IteratedLocalSearchGenerator::IteratedLocalSearchGenerator(CostDistanceInfo* info, int nodesInCycle, std::chrono::duration<double, std::milli> maxRuntime, int perturbationMoves):
    Generator(info, nodesInCycle), maxRuntime(maxRuntime), perturbationMoves(perturbationMoves), iterationsDone(0)
{
    //ctor
}

IteratedLocalSearchGenerator::~IteratedLocalSearchGenerator()
{
    //dtor
}

std::vector<int> IteratedLocalSearchGenerator::generateCycle(int start_pos)
{
    RandomHamiltonianCycleGenerator randomGenerator(costDistanceInfo, nodesInCycle, start_pos);
    std::vector<int> currentCycle = randomGenerator.generateCycle(start_pos);
    LocalSearchGenerator localSearchGenerator(costDistanceInfo, currentCycle);
    currentCycle = localSearchGenerator.generateCycle(start_pos);
    int bestFuncValue = localSearchGenerator.calculateCycleCost(currentCycle);
    std::vector<int> bestCycle = currentCycle;
    auto startTime = std::chrono::high_resolution_clock::now();
    auto currTime = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> timePassed = currTime - startTime;
    while(timePassed.count() < maxRuntime.count())
    {
        perturbCycle(currentCycle);
        LocalSearchGenerator localSearchGeneratorCur(costDistanceInfo, currentCycle);
        currentCycle = localSearchGeneratorCur.generateCycle(start_pos);
        int currFuncValue = localSearchGeneratorCur.calculateCycleCost(currentCycle);
        if(currFuncValue < bestFuncValue)
        {
            bestCycle = currentCycle;
            bestFuncValue = currFuncValue;
        }
        else
        {
            currentCycle = bestCycle;
        }
        currTime = std::chrono::high_resolution_clock::now();
        timePassed = currTime - startTime;
        iterationsDone++;
    }
    return bestCycle;
}

void IteratedLocalSearchGenerator::perturbCycle(std::vector<int>& currentCycle)
{
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> moveType(0,1);

    for(int i = 0; i < perturbationMoves; i++)
    {
        bool isInterMove = moveType(rng) == 0;
        if(isInterMove)
        {
            std::vector<int> nodesNotInCycle;
            for(int i = 0; i < costDistanceInfo->getNumOfNodes(); i++)
            {
                if(std::find(currentCycle.begin(), currentCycle.end(), i) == currentCycle.end())
                {
                    nodesNotInCycle.push_back(i);
                }
            }
            std::uniform_int_distribution<std::mt19937::result_type> nodeInCycleIdIndex(0, currentCycle.size() - 1);
            std::uniform_int_distribution<std::mt19937::result_type> nodeNotInCycleIdIndex(0, nodesNotInCycle.size() - 1);
            InterNodeNeighbourhoodMoveLocalSearch localMove(nodeInCycleIdIndex(rng), nodesNotInCycle[nodeNotInCycleIdIndex(rng)]);
            localMove.performMove(currentCycle, NULL); // cache not needed here
        }
        else
        {
            std::uniform_int_distribution<std::mt19937::result_type> nodeCycleIdIndex(0, currentCycle.size() - 1);
            int node1Index = nodeCycleIdIndex(rng);
            int node2Index = nodeCycleIdIndex(rng);
            if(abs(node1Index - node2Index) < 2)
            {
                node2Index = (node2Index + 2) % currentCycle.size();
            }
            IntraNodeChangeEdgeNeighbourhoodMoveLocalSearch localMove(node1Index, node2Index);
            localMove.performMove(currentCycle, NULL); // cache not needed here
        }
    }
}
