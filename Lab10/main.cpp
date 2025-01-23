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

//To beat: a: 69256, b: 43634 (averages)

int main()
{
    const std::string fileNames[] = {"TSPA", "TSPB"};
    const double correctTimesPerFile[] = {36404.82, 34441.575};

    for(size_t fileNameId = 1; fileNameId <= 1; fileNameId++) {
        std::string fileNameNoExt = fileNames[fileNameId];
        std::cout << fileNameNoExt;
        std::string fileName = fileNameNoExt + ".csv";
        std::vector<Node> nodeList = loadNodes(fileName);

        CostDistanceInfo costDistanceInfo(nodeList);

        int nodesToSelect = (nodeList.size() + 1 ) / 2;


        std::cout<<"Instance name: " << fileNameNoExt << std::endl;
        std::vector<std::vector<int>> cycles;
        std::vector<int> costs;
        std::vector<double> iters;
        int avg = 0;
        for (int repetition = 0; repetition < 20; repetition++)
        {
            HEAGenerator generator(&costDistanceInfo, nodesToSelect, 20, correctTimesPerFile[fileNameId]);
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
            avg += cost;
            std::cout << "PARTIAL AVERAGE: " << avg / float(repetition + 1) << std::endl;
        }
        saveResults(cycles, costs, fileNameNoExt + "Res.csv");
        std::vector<std::vector<double>> i;
        i.push_back(iters);
        saveTimes(i, fileNameNoExt + "RUNS.csv");
        std::cout << "AVERAGE COST: " << avg / 20.0 << std::endl;
    }
    return 0;
}
