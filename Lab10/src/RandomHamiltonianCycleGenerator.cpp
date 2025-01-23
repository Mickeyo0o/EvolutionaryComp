#include "RandomHamiltonianCycleGenerator.h"
#include <random>
#include <algorithm>

RandomHamiltonianCycleGenerator::RandomHamiltonianCycleGenerator(const CostDistanceInfo* costDistanceInfo, int nodes_in_cycle, unsigned int seed): Generator(costDistanceInfo, nodes_in_cycle)
{
    this->seed = seed;
    rng = new std::default_random_engine(seed);
}
std::vector<int> RandomHamiltonianCycleGenerator::generateCycle(int start_pos)
{
    int num_of_nodes = this->costDistanceInfo->getNumOfNodes();
    std::vector<int> indices(num_of_nodes);
    for (int i = 0; i < num_of_nodes; ++i) {
        indices[i] = i;
    }

    std::shuffle(indices.begin(), indices.end(), *rng);

    std::vector<int> out(indices.begin(), indices.begin() + std::min(nodesInCycle, num_of_nodes));

    return out;
}



RandomHamiltonianCycleGenerator::~RandomHamiltonianCycleGenerator()
{
    delete rng;
}
