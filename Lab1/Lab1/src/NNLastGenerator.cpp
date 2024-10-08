#include "NNLastGenerator.h"

NNLastGenerator::NNLastGenerator(const std::vector<std::vector<int>>& combinedMatrix, int nodes_in_cycle)
{
    this->combinedMatrix = combinedMatrix;
    this->nodes_in_cycle = nodes_in_cycle;
}

std::vector<int> NNLastGenerator::generateCycle(int start_pos) {
    std::vector<int> cycle;
    std::vector<bool> visited;
    visited.resize(combinedMatrix.size(), false);
    int currentNode = startNode;
    visited[currentNode] = true;
    cycle.push_back(currentNode);

    // Generate the nearest neighbor cycle
    for (int i = 1; i < numNodes; ++i) {
        float nearestDistance = std::numeric_limits<float>::max();
        int nearestNode = -1;

        for (int j = 0; j < numNodes; ++j) {
            // Check the distance from current node to j
            // Only consider unvisited nodes
            if (!visited[j]) {
                float distance = distanceMatrix[currentNode][j];
                if (distance < nearestDistance) {
                    nearestDistance = distance;
                    nearestNode = j;
                }
            }
        }

        // Add the nearest node to the cycle
        if (nearestNode != -1) {
            visited[nearestNode] = true;
            cycle.push_back(nearestNode);
            currentNode = nearestNode; // Move to the nearest node
        }
    }

    // Return to the starting node to complete the cycle
    cycle.push_back(startNode);

    return cycle;
}

NNLastGenerator::~NNLastGenerator()
{
    //dtor
}
