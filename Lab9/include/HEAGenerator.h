#ifndef HEAGENERATOR_H
#define HEAGENERATOR_H
#include <iostream>
#include <time.h>
#include <string>
#include <chrono>
#include "Generator.h"
#include "RandomHamiltonianCycleGenerator.h"
#include "LocalSearchGenerator.h"
#include "KRegretGreedyCycleCombinationGenerator.h"
#include <algorithm>
#include <set>

class HEAGenerator: public Generator
{
    public:
        HEAGenerator(const CostDistanceInfo* costDistanceInfo, int nodesInCycle, int popSize, bool useSimpleRecombination, bool localSearchAfterRecomb, float timeForGen);
        virtual ~HEAGenerator();
        std::vector<int> generateCycle(int start_pos);
        int iters = 0;
    private:
        struct SolutionWrapper
        {
            std::vector<int> solution;
            int cost;
        };
        int popSize;
        float timeForGen;
        bool useSimpleRecombination;
        bool localSearchAfterRecomb;
        SolutionWrapper recombinationSimple(const SolutionWrapper& parentA, const SolutionWrapper& parentB, unsigned seed);
        SolutionWrapper recombinationComplex(const SolutionWrapper& parentA, const SolutionWrapper& parentB, unsigned seed);
        void sortPopulation(SolutionWrapper* population);
};

#endif // HEAGENERATOR_H
