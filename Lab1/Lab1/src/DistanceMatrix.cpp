#include "DistanceMatrix.h"
#include <cmath>

DistanceMatrix::DistanceMatrix(const std::vector<Node>& nodes)
{
    this->nodes = nodes;
}

int DistanceMatrix::euclideanDistance(const Node& a, const Node& b) const {
    float dx = a.x - b.x;
    float dy = a.y - b.y;
    return std::round(std::sqrt(dx * dx + dy * dy));
}

std::vector<std::vector<int>> DistanceMatrix::calculateDistanceMatrix() const {
    std::vector<std::vector<int>> distanceMatrix(nodes.size(), std::vector<int>(nodes.size()));

    for (size_t i = 0; i < nodes.size(); ++i) {
        for (size_t j = 0; j < nodes.size(); ++j) {
            distanceMatrix[i][j] = euclideanDistance(nodes[i], nodes[j]);
        }
    }
    return distanceMatrix;
}

std::vector<std::vector<int>> DistanceMatrix::combineCosts(std::vector<std::vector<int>> distanceMatrix, std::vector<int> costList) const {
    std::vector<std::vector<int>> combinedMatrix(nodes.size(), std::vector<int>(nodes.size()));

    for (size_t i = 0; i < nodes.size(); ++i) {
        for (size_t j = 0; j < nodes.size(); ++j) {
            combinedMatrix[i][j] = distanceMatrix[i][j] + costList[j];
        }
    }
    return combinedMatrix;
}


DistanceMatrix::~DistanceMatrix()
{
    //dtor
}
