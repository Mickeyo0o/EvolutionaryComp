#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <chrono>
#include "HandleData.h"
#include "Node.h"
#include "RandomHamiltonianCycleGenerator.h"
#include "CostDistanceInfo.h"
#include "LocalSearchGenerator.h"
#include "HEAGenerator.h"

using namespace std;

int main()
{
    const std::string fileNames[] = {"TSPA", "TSPB"};
    const double correctTimesPerFile[] = {36404.82, 34441.575};

    for(size_t fileNameId = 0; fileNameId <= 1; fileNameId++) {
        std::string fileNameNoExt = fileNames[fileNameId];
        std::cout << fileNameNoExt;
        std::string fileName = fileNameNoExt + ".csv";
        std::vector<Node> nodeList = loadNodes(fileName);

        CostDistanceInfo costDistanceInfo(nodeList);

        int nodesToSelect = (nodeList.size() + 1 ) / 2;

        std::cout<<"Instance name: " << fileNameNoExt << std::endl;
        for (int simpleRecomb = 0; simpleRecomb <= 1; simpleRecomb++)
        {
            for(int localSearchAfterRecomb = 0; localSearchAfterRecomb <= 1; localSearchAfterRecomb++)
            {
                std::vector<std::vector<int>> cycles;
                std::vector<int> costs;
                std::vector<double> iters;
                for (int repetition = 0; repetition < 20; repetition++)
                {
                    std::cout << "RECOMBINATION SIMPLE: " << simpleRecomb << " LOCAL SEARCH AFTER RECOMB: "<< localSearchAfterRecomb << " REP: " << repetition << std::endl;
                    if(localSearchAfterRecomb == 0 && simpleRecomb == 1) break; // This combination is not needed
                    HEAGenerator generator(&costDistanceInfo, nodesToSelect, 20, simpleRecomb, localSearchAfterRecomb, correctTimesPerFile[fileNameId]);
                    std::vector<int> cycle = generator.generateCycle(repetition);
                    for(int i: cycle)
                    {
                        std::cout << i << ", ";
                    }
                    std::cout << std::endl;
                    cycles.push_back(cycle);
                    int cost = generator.calculateCycleCost(cycle);
                    std::cout << "Cost: " << cost << std::endl;
                    costs.push_back(cost);
                    iters.push_back(generator.iters);
                }
                saveResults(cycles, costs, fileNameNoExt + (localSearchAfterRecomb == 1?"LocalSearchAfterRecomb":"NoLocalSearchAfterRecomd") + (simpleRecomb==1?"SimpleRecomb":"ComplexRecomb") + ".csv");
                std::vector<std::vector<double>> i;
                i.push_back(iters);
                saveTimes(i, fileNameNoExt + (localSearchAfterRecomb == 1?"LocalSearchAfterRecomb":"NoLocalSearchAfterRecomd") + (simpleRecomb==1?"SimpleRecomb":"ComplexRecomb") + "RUNS.csv");
            }
        }
    }
    return 0;
}
