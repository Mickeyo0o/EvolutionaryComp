#ifndef RANDOMHAMILTONIANCYCLEGENERATOR_H
#define RANDOMHAMILTONIANCYCLEGENERATOR_H
#include <vector>

class RandomHamiltonianCycleGenerator
{
    public:
        RandomHamiltonianCycleGenerator(int num_of_nodes, int nodes_in_cycle, unsigned seed);
        std::vector<int> generateCycle();
        virtual ~RandomHamiltonianCycleGenerator();

    protected:

    private:
        int num_of_nodes;
        int nodes_in_cycle;
        unsigned seed;
};

#endif // RANDOMHAMILTONIANCYCLEGENERATOR_H
