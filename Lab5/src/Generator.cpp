#include "Generator.h"

Generator::Generator(const CostDistanceInfo* costDistanceInfo, int nodesInCycle): costDistanceInfo(costDistanceInfo), nodesInCycle(nodesInCycle)
{
    //ctor
}

Generator::~Generator() {

}

int Generator::calculateCycleCost(std::vector<int> cycle) {
    int sum = 0;
    for(size_t i = 0; i < cycle.size()-1; i++) {
        size_t node_id = cycle[i];
        size_t next_node_id = cycle[i + 1];
        sum += costDistanceInfo->getNodeCost(node_id);
        sum += costDistanceInfo->getDistance(node_id, next_node_id);
    }
    sum += costDistanceInfo->getNodeCost(cycle[cycle.size() - 1]);
    sum += costDistanceInfo->getDistance(cycle[0], cycle[cycle.size() - 1]);
    return sum;
}
