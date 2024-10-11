#ifndef GREEDYCYCLEGENERATOR_H
#define GREEDYCYCLEGENERATOR_H
#include "Generator.h"

/*  This class is almost the same as NNAllGenerator, the only difference is the formula for calculating
    function value change when adding at the beginning or end - here, the distance to the other end of the cycle
    is also taken into account.
*/

class GreedyCycleGenerator: public Generator
{
    public:
        GreedyCycleGenerator(const CostDistanceInfo* costDistanceInfo, int nodesInCycle);
        virtual ~GreedyCycleGenerator();
        std::vector<int> generateCycle(int start_pos);
    private:
        void addNodeToCycle(std::vector<int>& cycle, bool* visited);
};

#endif // GREEDYCYCLEGENERATOR_H
