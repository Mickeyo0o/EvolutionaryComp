#ifndef MULTIPLESTARTLOCALSEARCHGENERATOR_H
#define MULTIPLESTARTLOCALSEARCHGENERATOR_H
#include <iostream>
#include "Generator.h"
#include "CostDistanceInfo.h"
#include "RandomHamiltonianCycleGenerator.h"
#include "LocalSearchGenerator.h"

class MultipleStartLocalSearchGenerator : public Generator
{
    public:
        MultipleStartLocalSearchGenerator(CostDistanceInfo* info, int nodesInCycle, int iters);
        virtual ~MultipleStartLocalSearchGenerator();
        std::vector<int> generateCycle(int start_pos);
    private:
        int iters;
};

#endif // MULTIPLESTARTLOCALSEARCHGENERATOR_H
