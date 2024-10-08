#include "NNLastGenerator.h"
#include <limits>

NNLastGenerator::NNLastGenerator(const std::vector<std::vector<int>>& combinedMatrix, int nodes_in_cycle)
{
    this->combinedMatrix = combinedMatrix;
    this->nodes_in_cycle = nodes_in_cycle;
}

std::vector<int> NNLastGenerator::generateCycle(int start_pos) {
    std::vector<int> cycle;
    std::vector<bool> visited;
    visited.resize(combinedMatrix.size(), false);
    int currentNode = start_pos;
    visited[currentNode] = true;
    cycle.push_back(currentNode);

    for (int i = 1; i < nodes_in_cycle; ++i) {
        int nearestDistance = std::numeric_limits<int>::max();
        int nearestNode = -1;

        for (int j = 0; j < combinedMatrix.size(); ++j) {
            if (!visited[j]) {
                int distance = combinedMatrix[currentNode][j];
                if (distance < nearestDistance) {
                    nearestDistance = distance;
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
