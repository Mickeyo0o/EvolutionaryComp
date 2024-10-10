#include "NNAllGenerator.h"

NNAllGenerator::NNAllGenerator(const CostDistanceInfo* costDistanceInfo, int nodes_in_cycle): Generator(costDistanceInfo, nodes_in_cycle)
{
    //ctor
}

NNAllGenerator::~NNAllGenerator()
{
    //dtor
}

std::vector<int> NNAllGenerator::generateCycle(int start_pos) {
    int n = costDistanceInfo->getNumOfNodes();
    std::vector<int> cycle;
    bool* visited = new bool[n];
    for(size_t i = 0; i < n; i++) {
        visited[i] = false;
    }
    cycle.push_back(start_pos);
    visited[start_pos] = true;
    for(size_t i = 1; i < nodesInCycle; ++i) {
        addNodeToCycle(cycle, visited);
    }
    delete [] visited;
    return cycle;
}

void NNAllGenerator::addNodeToCycle(std::vector<int>& cycle, bool* visited) {
    size_t n = costDistanceInfo->getNumOfNodes();
    int* bestPositionToPutNode = new int[n];
    int* bestIncreaseInFunction = new int[n];

    for(int i = 0; i < n; i++) {
        bestPositionToPutNode[i] = -1;
        bestIncreaseInFunction[i] = std::numeric_limits<int>::max();
    }

    int lastNodeIndex = cycle[cycle.size() - 1];
    for(int i = 0; i < n; i++) { // Try adding at the end
        if(!visited[i]) {
            int increaseInFunction = this->costDistanceInfo->getDistance(i, lastNodeIndex) + this->costDistanceInfo->getNodeCost(i);
            bestIncreaseInFunction[i] = increaseInFunction;
            bestPositionToPutNode[i] = cycle.size();
        }
    }

    int firstNodeIndex = cycle[0];
    for(int i = 0; i < n; i++) { // Try adding at the beginning
        if(!visited[i]) {
            int increaseInFunction = this->costDistanceInfo->getDistance(i, firstNodeIndex) + this->costDistanceInfo->getNodeCost(i);
            if(increaseInFunction < bestIncreaseInFunction[i]) {
                bestIncreaseInFunction[i] = increaseInFunction;
                bestPositionToPutNode[i] = 0;
            }
        }
    }

    if(cycle.size() > 1) {
        for(int i = 0; i < n; i++) { // Try all other positions
            if(!visited[i]) {
                for(int posToAddTo = 1; posToAddTo < cycle.size() - 1; posToAddTo++) {
                    int leftNeighbour = cycle[posToAddTo - 1];
                    int rightNeigbour = cycle[posToAddTo];
                    int currFunctionValue = this->costDistanceInfo->getDistance(leftNeighbour, rightNeigbour);
                    int newPossibleFunctionValue = this->costDistanceInfo->getDistance(leftNeighbour, i) + this->costDistanceInfo->getDistance(rightNeigbour, i) +
                                                    this->costDistanceInfo->getNodeCost(i);
                    int increaseInFunction = newPossibleFunctionValue - currFunctionValue;
                    if(increaseInFunction < bestIncreaseInFunction[i]) {
                        bestIncreaseInFunction[i] = increaseInFunction;
                        bestPositionToPutNode[i] = posToAddTo;
                    }
                }
            }
        }
    }
    int bestIncrease = std::numeric_limits<int>::max();
    int bestNodeToAdd = -1;
    for(int i = 0; i < n; i++) {
        if(!visited[i]) {
            if(bestIncreaseInFunction[i] < bestIncrease) {
                bestIncrease = bestIncreaseInFunction[i];
                bestNodeToAdd = i;
            }
        }
    }
    int bestPositionToAdd = bestPositionToPutNode[bestNodeToAdd];
    cycle.insert(cycle.begin() + bestPositionToAdd, bestNodeToAdd);
    visited[bestNodeToAdd] = true;

    delete [] bestPositionToPutNode;
    delete [] bestIncreaseInFunction;
}
