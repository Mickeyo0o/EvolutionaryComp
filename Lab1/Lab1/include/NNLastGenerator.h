#ifndef NNLASTGENERATOR_H
#define NNLASTGENERATOR_H
#include <limits>
#include "Generator.h"

class NNLastGenerator: public Generator
{
    public:
        NNLastGenerator(const CostDistanceInfo* costDistanceInfo, int nodesInCycle);
        virtual ~NNLastGenerator();
        std::vector<int> generateCycle(int start_pos);
};

#endif // NNLASTGENERATOR_H
