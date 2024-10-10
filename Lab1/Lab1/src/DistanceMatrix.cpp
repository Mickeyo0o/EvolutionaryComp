#include "CostDistanceInfo.h"
#include <cmath>

CostDistanceInfo::CostDistanceInfo(const std::vector<Node>& nodes) {
    this->nodes = nodes;
    calculateDistanceMatrix();
}

CostDistanceInfo::~CostDistanceInfo() {
    for(size_t i = 0; i < n; i++) {
        delete [] distanceMatrix[i];
    }
    delete [] distanceMatrix;
}

int getDistance(size_t n1, size_t n2) {
    return this->distanceMatrix[n1][n2];
}

int CostDistanceInfo::euclideanDistance(const Node& a, const Node& b) const {
    float dx = a.x - b.x;
    float dy = a.y - b.y;
    return std::round(std::sqrt(dx * dx + dy * dy));
}

int CostDistanceInfo::getNodeCost(size_t n) {
    if(this->nodes.size() >= n) {
        return -1;
    }
    return this->nodes[n].cost;
}

void CostDistanceInfo::calculateDistanceMatrix() const {
    int n = this->nodes.size();
    this->distanceMatrix = new int*[n];
    for(size_t i = 0; i < n; i++) {
        distanceMatrix[i] = new int[n];
    }
    for (size_t i = 0; i < nodes.size(); ++i) {
        for (size_t j = i + 1; j < nodes.size(); ++j) { // No need to recalculate from j = 0
            int dist = euclideanDistance(nodes[i], nodes[j]);
            distanceMatrix[i][j] = dist;
            distanceMatrix[j][i] = dist;
        }
    }
}
