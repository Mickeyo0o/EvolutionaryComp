#ifndef LNSGENERATOR_H
#define LNSGENERATOR_H
#include <vector>
#include <chrono>
#include <iostream>
#include <random>
#include "Generator.h"
#include "LocalSearchGenerator.h"
#include "RandomHamiltonianCycleGenerator.h"
#include "KRegretGreedyCycleCombinationGenerator.h"

class LNSGenerator: public Generator
{
    public:
        LNSGenerator(const CostDistanceInfo* costDistanceInfo, int nodesInCycle, double masRuntime, double destroyNodeRatio); // time in ms
        virtual ~LNSGenerator();
        std::vector<int> generateCycle(int start_pos);
        inline void setLocalSearchFlag(bool lsFlag)
        {
            useLocalSearchAfterEveryStep = lsFlag;
        }
        int lastIterCount;

    private:
        void destroy(std::vector<int>& cycle);
        void repair(std::vector<int>& cycle);
        double maxRuntime;
        bool useLocalSearchAfterEveryStep = false;
        double destroyNodeRatio;
};

#endif // LNSGENERATOR_H
