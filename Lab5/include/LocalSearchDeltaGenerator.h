#ifndef LOCALSEARCHDELTAGENERATOR_H
#define LOCALSEARCHDELTAGENERATOR_H
#include <set>
#include "Generator.h"
#include "NeighbourhoodMove.h"
#include "InterNodeNeighbourhoodMove.h"
#include "IntraNodeChangeEdgeNeighbourhoodMove.h"

class LocalSearchDeltaGenerator: public Generator
{
    public:
        LocalSearchDeltaGenerator(const CostDistanceInfo* costDistanceInfo, std::vector<int> initialCycle);
        virtual ~LocalSearchDeltaGenerator();
        std::vector<int> generateCycle(int start_pos);

    private:
        std::vector<int> initialCycle;
        void addMoveToList(std::set<NeighbourhoodMove*, bool(*)(NeighbourhoodMove*, NeighbourhoodMove*)>& movesList, NeighbourhoodMove* nMove);
        void generateMoves(std::set<NeighbourhoodMove*, bool(*)(NeighbourhoodMove*, NeighbourhoodMove*)>& movesList, const std::vector<int>& currentCycle, int* cyclePosCache, const std::vector<int>& enteringNodesIds);

};

#endif // LOCALSEARCHDELTAGENERATOR_H
