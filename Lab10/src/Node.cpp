#include "Node.h"
#include <iostream>

Node::Node(int x_coord, int y_coord, int node_cost)
{
    this->x = x_coord;
    this->y = y_coord;
    this->cost = node_cost;
};

void Node::displayNode() const {
        std::cout << "Node(" << this->x << ", " << this->y << ") with cost: " << this->cost << std::endl;
    }

Node::~Node()
{
    //dtor
}
