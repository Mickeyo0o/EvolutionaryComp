#include "RandomHamiltonianCycleGenerator.h"
#include <random>
#include <algorithm>

RandomHamiltonianCycleGenerator::RandomHamiltonianCycleGenerator(int num_of_nodes, int nodes_in_cycle, unsigned seed)
{
    this->num_of_nodes = num_of_nodes;
    this->nodes_in_cycle = nodes_in_cycle;
    this->seed = seed;
}
std::vector<int> RandomHamiltonianCycleGenerator::generateCycle()
{
    std::vector<int> indices(num_of_nodes);
    for (int i = 0; i < num_of_nodes; ++i) {
        indices[i] = i;
    }

    std::shuffle(indices.begin(), indices.end(), std::default_random_engine(seed));

    std::vector<int> out(indices.begin(), indices.begin() + std::min(nodes_in_cycle, num_of_nodes));

    seed = seed + 1;

    return out;
}



RandomHamiltonianCycleGenerator::~RandomHamiltonianCycleGenerator()
{
    //dtor
}
