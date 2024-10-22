#ifndef KREGRETGREEDYCYCLECOMBINATIONGENERATOR_H
#define KREGRETGREEDYCYCLECOMBINATIONGENERATOR_H
#include <algorithm>
#include <iostream>
#include "Generator.h"


class KRegretGreedyCycleCombinationGenerator: public Generator
{
    public:
        KRegretGreedyCycleCombinationGenerator(const CostDistanceInfo* costDistanceInfo, int nodesInCycle, float regretWeight);
        virtual ~KRegretGreedyCycleCombinationGenerator();
        std::vector<int> generateCycle(int start_pos);

    private:
        int calculateRegret(std::vector<int>& differences, int minDiff);
        float regretWeight;
};

#endif // KREGRETGREEDYCYCLECOMBINATIONGENERATOR_H
