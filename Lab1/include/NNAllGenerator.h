#ifndef NNALLGENERATOR_H
#define NNALLGENERATOR_H
#include <limits>
#include <iostream>
#include "Generator.h"

class NNAllGenerator: public Generator
{
    public:
        NNAllGenerator(const CostDistanceInfo* costDistanceInfo, int nodesInCycle);
        virtual ~NNAllGenerator();
        std::vector<int> generateCycle(int start_pos);
    private:
        void addNodeToCycle(std::vector<int>& cycle, bool* visited);
};

#endif // NNALLGENERATOR_H
