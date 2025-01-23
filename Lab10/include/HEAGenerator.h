#ifndef HEAGENERATOR_H
#define HEAGENERATOR_H
#include <iostream>
#include <time.h>
#include <string>
#include <chrono>
#include <cmath>
#include "Generator.h"
#include "RandomHamiltonianCycleGenerator.h"
#include "LocalSearchCandidateMovesGenerator.h"
#include "InterNodeNeighbourhoodMoveLocalSearch.h"
#include "IntraNodeChangeEdgeNeighbourhoodMoveLocalSearch.h"
#include "KRegretGreedyCycleCombinationGenerator.h"
#include <algorithm>
#include <set>

class HEAGenerator: public Generator
{
    public:
        HEAGenerator(const CostDistanceInfo* costDistanceInfo, int nodesInCycle, int popSize, float timeForGen);
        virtual ~HEAGenerator();
        std::vector<int> generateCycle(int start_pos);
        int iters = 0;
    private:
        struct SolutionWrapper
        {
            std::vector<int> solution;
            int cost;
            int costNoPenalty;
            int nodesFromParentsCount;
        };
        int popSize;
        float timeForGen;
        SolutionWrapper recombinationSimple(const SolutionWrapper& parentA, const SolutionWrapper& parentB, unsigned seed);
        SolutionWrapper recombinationComplex(const SolutionWrapper& parentA, const SolutionWrapper& parentB, unsigned seed);
        SolutionWrapper* mutate(SolutionWrapper* solution, int mutThresh);
        float penalty(const SolutionWrapper& solution, int thresh);
        void sortPopulation(SolutionWrapper* population);
        int getParentId(SolutionWrapper* pop, int popSize);
        std::vector<int> getParents(SolutionWrapper* pop);
};

#endif // HEAGENERATOR_H
