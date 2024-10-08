#ifndef DISTANCEMATRIX_H
#define DISTANCEMATRIX_H
#include <vector>
#include "Node.h"


class DistanceMatrix
{
    public:
        DistanceMatrix(const std::vector<Node>& nodes);
        virtual ~DistanceMatrix();
        std::vector<std::vector<int>> calculateDistanceMatrix() const;
        std::vector<std::vector<int>> combineCosts(std::vector<std::vector<int>> distanceMatrix, std::vector<int> costList) const;

    protected:

    private:
        std::vector<Node> nodes;
        int euclideanDistance(const Node& a, const Node& b) const;
};

#endif // DISTANCEMATRIX_H
