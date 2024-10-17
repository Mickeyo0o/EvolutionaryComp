#include "KRegretGenerator.h"

KRegretGenerator::KRegretGenerator(const CostDistanceInfo* costDistanceInfo, int nodesInCycle) : Generator(costDistanceInfo, nodesInCycle) {
    //ctor
}

KRegretGenerator::~KRegretGenerator() {
    //dtor
}

std::vector<int> KRegretGenerator::generateCycle(int start_pos) {
    int n = costDistanceInfo->getNumOfNodes();
    std::vector<int> cycle;
    bool* visited = new bool[n];
    for(int i = 0; i < n; i++) {
        visited[i] = false;
    }
    cycle.push_back(start_pos);
    visited[start_pos] = true;
    int minCost = std::numeric_limits<int>::max();
    int nodeToAdd = -1;
    for(int i = 0; i < n; i++) {
        if(start_pos == i) continue;
        int currCost = costDistanceInfo->getNodeCost(i) + costDistanceInfo->getDistance(cycle[0], i);
        if(currCost < minCost) {
            minCost = currCost;
            nodeToAdd = i;
        }
    }
    cycle.push_back(nodeToAdd);
    visited[nodeToAdd] = true;
    for(int elInCycle = 2; elInCycle < nodesInCycle; elInCycle++) {
        int* bestPosition = new int[n];
        int* regrets = new int[n];
        for(int i = 0; i < n; i++) {
            bestPosition[i] = -1;
            regrets[i] = -1;
        }
        for(int i = 0; i < n; i++) {
            if(visited[i]) continue;
            std::vector<int> diffs;
            int minDiff = std::numeric_limits<int>::max();
            int bestPos = -1;
            for(int posInCycle = 0; posInCycle < cycle.size(); posInCycle++) {
                int lNeighbourId = -1;
                int rNeighbourId = cycle[posInCycle];
                if(posInCycle == 0) {
                    lNeighbourId = cycle[cycle.size() - 1];
                }
                else {
                    lNeighbourId = cycle[posInCycle - 1];
                }
                int functionDiff = costDistanceInfo->getDistance(i, lNeighbourId) + costDistanceInfo->getDistance(i, rNeighbourId) +
                                    costDistanceInfo->getNodeCost(i) - costDistanceInfo->getDistance(lNeighbourId, rNeighbourId);
                diffs.push_back(functionDiff);
                if(functionDiff < minDiff) {
                    minDiff = functionDiff;
                    bestPos = posInCycle;
                }
            }
            bestPosition[i] = bestPos;
            regrets[i] = this->calculateRegret(diffs, minDiff);
        }
        int maxRegret = -1;
        int bestElToAddId = -1;
        for(int i = 0; i < n; i++) {
            int currRegret = regrets[i];
            if(currRegret > maxRegret) {
                maxRegret = currRegret;
                bestElToAddId = i;
            }
        }
        visited[bestElToAddId] = true;
        cycle.insert(cycle.begin() + bestPosition[bestElToAddId], bestElToAddId);
        delete [] bestPosition;
        delete [] regrets;
    }
    delete [] visited;
    return cycle;
}

int KRegretGenerator::calculateRegret(std::vector<int>& differences, int minDiff) {
    int secondMinDiff = std::numeric_limits<int>::max();
    int countMinDiffAppearances = 0;
    for(int currDiff: differences) {
        if(currDiff == minDiff) {
            countMinDiffAppearances += 1;
        }
        if(currDiff < secondMinDiff && currDiff > minDiff) {
            secondMinDiff = currDiff;
        }
    }
    if(countMinDiffAppearances > 1) {
        return 0;
    }
    return secondMinDiff - minDiff;
}

