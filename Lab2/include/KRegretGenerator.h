#ifndef KREGRETGENERATOR_H
#define KREGRETGENERATOR_H
#include <algorithm>
#include <iostream>
#include "Generator.h"


class KRegretGenerator : public Generator
{
    public:
        KRegretGenerator(const CostDistanceInfo* costDistanceInfo, int nodesInCycle);
        virtual ~KRegretGenerator();
        std::vector<int> generateCycle(int start_pos);

    private:
        int calculateRegret(std::vector<int>& differences, int minDiff);
};

#endif // KREGRETGENERATOR_H
