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

using namespace std;

int main()
{
    const std::string fileNames[] = {"TSPA", "TSPB"};

    for(size_t fileNameId = 0; fileNameId <= 1; fileNameId++) {
        std::string fileNameNoExt = fileNames[fileNameId];
        std::cout << fileNameNoExt;
        std::string fileName = fileNameNoExt + ".csv";
        std::vector<Node> nodeList = loadNodes(fileName);

        CostDistanceInfo costDistanceInfo(nodeList);

        int nodesToSelect = (nodeList.size() + 1 ) / 2;

        std::vector<std::vector<int>> cycles;
        std::cout<<"Instance name: " << fileNameNoExt << std::endl;
        std::vector<int> costs;
        std::vector<double> times;
        for (int repetition = 0; repetition < 1000; repetition++)
        {
            std::cout << repetition << std::endl;
            std::vector<int> initialCycle;
            RandomHamiltonianCycleGenerator randomGenerator(&costDistanceInfo, nodesToSelect, repetition);
            initialCycle = randomGenerator.generateCycle(repetition);
            std::cout << "Score before: " << randomGenerator.calculateCycleCost(initialCycle) << " ";

            LocalSearchGenerator localRandomGenerator(&costDistanceInfo, initialCycle, true, true, repetition);
            std::vector<int> optimizedInitialCycle = localRandomGenerator.generateCycle(repetition);

            costs.push_back(localRandomGenerator.calculateCycleCost(optimizedInitialCycle));
            for(int n: optimizedInitialCycle) {
                std::cout << n << " ";
            }
            std::cout << std::endl;
            cycles.push_back(optimizedInitialCycle);
        }
        saveResults(cycles, costs, fileNameNoExt + "Greedy" + "IntraSwapEdges" + "RandomStart"+".csv");
    }
    return 0;
}
