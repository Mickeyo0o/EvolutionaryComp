#ifndef LOCALSEARCHCANDIDATEMOVESGENERATOR_H
#define LOCALSEARCHCANDIDATEMOVESGENERATOR_H
#include <vector>
#include <algorithm>
#include <iostream>
#include <chrono>
#include "Generator.h"
#include "Node.h"
#include "NeighbourhoodMove.h"
#include "IntraNodeChangeEdgeNeighbourhoodMove.h"
#include "InterNodeNeighbourhoodMove.h"

class LocalSearchCandidateMovesGenerator: public Generator
{
    public:
        LocalSearchCandidateMovesGenerator(const CostDistanceInfo* costDistanceInfo, int nodesInCycle, std::vector<int> startingCycle);
        LocalSearchCandidateMovesGenerator(const CostDistanceInfo* costDistanceInfo, int nodesInCycle, std::vector<int> startingCycle, unsigned int numOfNearestNeighbours);
        virtual ~LocalSearchCandidateMovesGenerator();
        std::vector<int> generateCycle(int start_pos);

    private:
        int** nearestNeighboursPerNode;
        std::vector<int> startingCycle;
        int* positionInCycleCache;
        unsigned int numOfNearestNeighbours = 10;

        void init();
};

#endif // LOCALSEARCHCANDIDATEMOVESGENERATOR_H
