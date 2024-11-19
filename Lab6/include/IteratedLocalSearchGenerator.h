#ifndef ITERATEDLOCALSEARCHGENERATOR_H
#define ITERATEDLOCALSEARCHGENERATOR_H
#include <chrono>
#include <random>
#include <iostream>
#include "Generator.h"
#include "LocalSearchGenerator.h"
#include "RandomHamiltonianCycleGenerator.h"
#include "InterNodeNeighbourhoodMoveLocalSearch.h"
#include "IntraNodeChangeEdgeNeighbourhoodMoveLocalSearch.h"


class IteratedLocalSearchGenerator : public Generator
{
    public:
        IteratedLocalSearchGenerator(CostDistanceInfo* info, int nodesInCycle, std::chrono::duration<double, std::milli> maxRuntime, int perturbationMoves);
        virtual ~IteratedLocalSearchGenerator();
        std::vector<int> generateCycle(int start_pos);
        int iterationsDone;

    private:
         std::chrono::duration<double, std::milli> maxRuntime;
         int perturbationMoves;
         void perturbCycle(std::vector<int>& currentCycle);
};

#endif // ITERATEDLOCALSEARCHGENERATOR_H
