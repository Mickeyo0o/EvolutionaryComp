#ifndef LOCALSEARCHGENERATOR_H
#define LOCALSEARCHGENERATOR_H
#include <iterator>
#include <limits>
#include <iostream>
#include <algorithm>
#include <chrono>
#include <random>
#include "Generator.h"
#include "InterNodeNeighbourhoodMoveLocalSearch.h"
#include "IntraNodeChangeEdgeNeighbourhoodMoveLocalSearch.h"

class LocalSearchGenerator: public Generator
{
    public:
        LocalSearchGenerator(const CostDistanceInfo* costDistanceInfo, std::vector<int> initialCycle);
        virtual ~LocalSearchGenerator();
        std::vector<int> generateCycle(int start_pos);

    private:
        std::vector<int> initialCycle;
};

#endif // LOCALSEARCHGENERATOR_H
