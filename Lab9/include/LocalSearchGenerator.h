#ifndef LOCALSEARCHGENERATOR_H
#define LOCALSEARCHGENERATOR_H
#include <iterator>
#include <limits>
#include <iostream>
#include <algorithm>
#include <chrono>
#include <random>
#include "Generator.h"
#include "InterNodeNeighbourhoodMove.h"
#include "IntraNodeChangeEdgeNeighbourhoodMove.h"
#include "IntraNodeChangeNodeNeighbourhoodMove.h"

class LocalSearchGenerator: public Generator
{
    public:
        LocalSearchGenerator(const CostDistanceInfo* costDistanceInfo, std::vector<int> initialCycle, bool isGreedy, bool swapEdges, unsigned int seed);
        virtual ~LocalSearchGenerator();
        std::vector<int> generateCycle(int start_pos);

    private:
        bool isGreedy;
        std::vector<int> initialCycle;
        unsigned int seed;
        bool swapEdges;
};

#endif // LOCALSEARCHGENERATOR_H
