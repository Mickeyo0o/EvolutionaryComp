#ifndef RANDOMHAMILTONIANCYCLEGENERATOR_H
#define RANDOMHAMILTONIANCYCLEGENERATOR_H
#include "Generator.h"
#include <random>
#include <algorithm>

class RandomHamiltonianCycleGenerator: public Generator
{
    public:
        RandomHamiltonianCycleGenerator(const CostDistanceInfo* costDistanceInfo, int nodesInCycle, unsigned int seed);
        ~RandomHamiltonianCycleGenerator();
        std::vector<int> generateCycle(int start_pos);

    private:
        unsigned int seed;
};

#endif // RANDOMHAMILTONIANCYCLEGENERATOR_H
