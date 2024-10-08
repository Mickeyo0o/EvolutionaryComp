#ifndef NNLASTGENERATOR_H
#define NNLASTGENERATOR_H
#include <vector>

class NNLastGenerator
{
    public:
        NNLastGenerator(const std::vector<std::vector<int>>& combinedMatrix, int nodes_in_cycle);
        std::vector<int> generateCycle(int start_pos);
        virtual ~NNLastGenerator();

    protected:

    private:
        std::vector<std::vector<int>> combinedMatrix;
        int nodes_in_cycle;
};

#endif // NNLASTGENERATOR_H
