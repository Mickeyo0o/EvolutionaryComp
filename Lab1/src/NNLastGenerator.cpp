#include "NNLastGenerator.h"

NNLastGenerator::NNLastGenerator(const CostDistanceInfo* costDistanceInfo, int nodesInCycle): Generator(costDistanceInfo, nodesInCycle) {

}

std::vector<int> NNLastGenerator::generateCycle(int start_pos) {
    std::vector<int> cycle;
    std::vector<bool> visited;
    visited.resize(costDistanceInfo->getNumOfNodes(), false);
    int currentNode = start_pos;
    visited[currentNode] = true;
    cycle.push_back(currentNode);

    for (int i = 1; i < nodesInCycle; ++i) {
        int nearestIncrease = std::numeric_limits<int>::max();
        int nearestNode = -1;

        for (int j = 0; j < costDistanceInfo->getNumOfNodes(); j++) {
            if (!visited[j]) {
                int totalIncrease = costDistanceInfo->getDistance(currentNode, j) + costDistanceInfo->getNodeCost(j);
                if (totalIncrease < nearestIncrease) {
                    nearestIncrease = totalIncrease;
                    nearestNode = j;
                }
            }
        }

        if (nearestNode != -1) {
            visited[nearestNode] = true;
            cycle.push_back(nearestNode);
            currentNode = nearestNode;
        }
    }

    return cycle;
}

NNLastGenerator::~NNLastGenerator()
{
    //dtor
}
