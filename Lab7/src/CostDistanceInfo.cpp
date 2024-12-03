#include "CostDistanceInfo.h"

CostDistanceInfo::CostDistanceInfo(const std::vector<Node>& nodes) {
    this->nodes = nodes;
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

CostDistanceInfo::~CostDistanceInfo() {
    this->releaseResources();
}

size_t CostDistanceInfo::getNumOfNodes() const{
    return this->nodes.size();
}

int CostDistanceInfo::getDistance(size_t n1, size_t n2) const {
    if(n1 < this->nodes.size() && n2 < this->nodes.size()) {
        return this->distanceMatrix[n1][n2];
    }
    return -1;
}

int CostDistanceInfo::euclideanDistance(const Node& a, const Node& b) const {
    float dx = a.x - b.x;
    float dy = a.y - b.y;
    return std::round(std::sqrt(dx * dx + dy * dy));
}

int CostDistanceInfo::getNodeCost(size_t n) const {
    if(n < this->nodes.size()) {
        return this->nodes[n].cost;;
    }
    return -1;
}

void CostDistanceInfo::releaseResources() {
    int n = this->nodes.size();
    for(size_t i = 0; i < n; i++) {
        delete [] distanceMatrix[i];
    }
    delete [] distanceMatrix;
}
