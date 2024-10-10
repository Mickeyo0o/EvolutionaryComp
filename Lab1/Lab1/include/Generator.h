#ifndef GENERATOR_H
#define GENERATOR_H
#include "CostDistanceInfo.h"

class Generator
{
    public:
        Generator(const CostDistanceInfo* costDistanceInfo, int nodesInCycle);
        virtual ~Generator();
        virtual std::vector<int> generateCycle(int start_pos) = 0;
        int calculateCycleCost(std::vector<int> cycle);

    protected:
        const CostDistanceInfo* costDistanceInfo;
        int nodesInCycle;
};

#endif // GENERATOR_H
