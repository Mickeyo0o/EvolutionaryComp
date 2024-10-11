#ifndef DISTANCEMATRIX_H
#define DISTANCEMATRIX_H
#include <vector>
#include "Node.h"


class CostDistanceInfo
{
    public:
        CostDistanceInfo(const std::vector<Node>& nodes, );
        ~CostDistanceInfo();
        int getDistance(size_t n1, size_t n2);
        int getNodeCost(size_t n);

    protected:

    private:
        std::vector<Node> nodes;
        int** distanceMatrix;

        int euclideanDistance(const Node& a, const Node& b) const;
        int** calculateDistanceMatrix() const;

};

#endif // DISTANCEMATRIX_H
