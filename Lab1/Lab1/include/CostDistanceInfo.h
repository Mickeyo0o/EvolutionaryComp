#ifndef DISTANCEMATRIX_H
#define DISTANCEMATRIX_H
#include <vector>
#include <cmath>
#include "Node.h"


class CostDistanceInfo
{
    public:
        CostDistanceInfo(const std::vector<Node>& nodes);
        ~CostDistanceInfo();
        int getDistance(size_t n1, size_t n2) const;
        int getNodeCost(size_t n) const;
        size_t getNumOfNodes() const;
        void releaseResources();

    protected:

    private:
        std::vector<Node> nodes;
        int** distanceMatrix;

        int euclideanDistance(const Node& a, const Node& b) const;

};

#endif // DISTANCEMATRIX_H
