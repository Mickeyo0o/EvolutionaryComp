#include "MultipleStartLocalSearchGenerator.h"

MultipleStartLocalSearchGenerator::MultipleStartLocalSearchGenerator(CostDistanceInfo* info, int nodesInCycle, int iters):
    Generator(info, nodesInCycle), iters(iters)
{

}

MultipleStartLocalSearchGenerator::~MultipleStartLocalSearchGenerator()
{
    //dtor
}

std::vector<int> MultipleStartLocalSearchGenerator::generateCycle(int start_pos)
{
    std::vector<int> bestCycle;
    int bestFuncValue = -1;
    RandomHamiltonianCycleGenerator initialGenerator(costDistanceInfo, nodesInCycle, start_pos * iters);
    for(int i = 0; i < iters; i++)
    {
        std::vector<int> cycle = initialGenerator.generateCycle(i);
        LocalSearchGenerator localSearchGeneretor(costDistanceInfo, cycle);
        std::vector<int> optimizedCycle = localSearchGeneretor.generateCycle(i);
        int currFuncValue = localSearchGeneretor.calculateCycleCost(optimizedCycle);
        if(currFuncValue < bestFuncValue || bestFuncValue == -1)
        {
            bestFuncValue = currFuncValue;
            bestCycle = optimizedCycle;
        }
    }
    return bestCycle;
}
