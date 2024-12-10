#include "LNSGenerator.h"

LNSGenerator::LNSGenerator(const CostDistanceInfo* costDistanceInfo, int nodesInCycle, double maxRuntime, double destroyNodeRatio):
    Generator(costDistanceInfo, nodesInCycle), maxRuntime(maxRuntime), destroyNodeRatio(destroyNodeRatio)
{

}

LNSGenerator::~LNSGenerator()
{

}


std::vector<int> LNSGenerator::generateCycle(int start_pos)
{
    lastIterCount = 0;
    srand(start_pos);
    RandomHamiltonianCycleGenerator randomGen(costDistanceInfo, nodesInCycle, start_pos);
    std::vector<int> bestCycle = randomGen.generateCycle(start_pos);
    LocalSearchGenerator localSearchGenerator(costDistanceInfo, bestCycle);
    bestCycle = localSearchGenerator.generateCycle(start_pos);
    int bestCost = localSearchGenerator.calculateCycleCost(bestCycle);
    auto startTime = std::chrono::high_resolution_clock::now();
    auto currTime = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> timePassed = currTime - startTime;
    while(timePassed.count() < maxRuntime)
    {
        std::vector<int> currCycle = bestCycle;
        destroy(currCycle);
        repair(currCycle);
        if(useLocalSearchAfterEveryStep)
        {
            LocalSearchGenerator lsGen(costDistanceInfo, currCycle);
            currCycle = lsGen.generateCycle(start_pos);
        }
        int currCost = calculateCycleCost(currCycle);
        if(currCost < bestCost)
        {
            bestCost = currCost;
            bestCycle = currCycle;
        }
        currTime = std::chrono::high_resolution_clock::now();
        timePassed = currTime - startTime;
        lastIterCount++;
    }

    return bestCycle;
}

void LNSGenerator::destroy(std::vector<int>& cycle)
{
    for(int it = 0; it < (destroyNodeRatio * cycle.size()); it++) // This is very much not optimized
    {
        std::vector<std::pair<double, int>> costsAndNodes;
        int prevNodeId = cycle.back();

        int costSum = 0;

        for(int id = 0; id < cycle.size(); id++)
        {
            int nextNodeId = cycle[(id + 1) % cycle.size()];
            int currNodeCost = costDistanceInfo->getDistance(prevNodeId, id) + costDistanceInfo->getDistance(id, nextNodeId) + costDistanceInfo->getNodeCost(id);
            costsAndNodes.push_back(std::pair<int, int>(currNodeCost, id));

            costSum += currNodeCost;
            prevNodeId = id;
        }
        std::sort(costsAndNodes.begin(), costsAndNodes.end());

        costsAndNodes[0].first /= costSum;

        for(int i = 1; i < costsAndNodes.size(); i++)
        {
            auto* currPair = &costsAndNodes[i];
            auto* prevPair = &costsAndNodes[i - 1];
            currPair->first /= costSum;
            currPair->first += prevPair->first;
        }
        float currPick = ((float)rand() / RAND_MAX);
        int selectedNodeToRemove = costsAndNodes[0].second;
        for(auto nodeCostPair: costsAndNodes)
        {
            if(nodeCostPair.first > currPick)
            {
                selectedNodeToRemove = nodeCostPair.second;
                break;
            }
        }

        cycle.erase(std::find(cycle.begin(), cycle.end(), selectedNodeToRemove));
    }

}

void LNSGenerator::repair(std::vector<int>& cycle)
{
    KRegretGreedyCycleCombinationGenerator kregretGenerator(costDistanceInfo, nodesInCycle, 0.5f);
    cycle = kregretGenerator.generateCycle(0, cycle);
}
