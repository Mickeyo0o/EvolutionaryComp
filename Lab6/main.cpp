#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <chrono>
#include "HandleData.h"
#include "RandomHamiltonianCycleGenerator.h"
#include "LocalSearchDeltaGenerator.h"
#include "MultipleStartLocalSearchGenerator.h"
#include "IteratedLocalSearchGenerator.h"

using namespace std;

int main()
{
    const std::string fileNames[] = {"TSPA", "TSPB"};
    std::vector<std::vector<double>> allTimes;
    std::vector<double> localMultiTimes; // Iterated search runs as long as multi start local search, so its times are the same

    for(size_t fileNameId = 0; fileNameId <= 1; fileNameId++) {
        std::string fileNameNoExt = fileNames[fileNameId];
        std::cout << fileNameNoExt << std::endl;
        std::string fileName = fileNameNoExt + ".csv";
        std::vector<Node> nodeList = loadNodes(fileName);

        CostDistanceInfo costDistanceInfo(nodeList);

        int nodesToSelect = (nodeList.size() + 1 ) / 2;

        std::vector<std::vector<int>> localMultiCycles;
        std::vector<std::vector<int>> localIteratedCycles;

        std::vector<int> localMultiCosts;
        std::vector<int> localIteratedCosts;

        for(int rep = 0; rep < 20; rep++)
        {
            std::cout << "REPETITION " << rep << std::endl;
            MultipleStartLocalSearchGenerator multiGenerator(&costDistanceInfo, nodesToSelect, 200);
            auto startTime = std::chrono::high_resolution_clock::now();
            std::vector<int> multiCycle = multiGenerator.generateCycle(rep);
            auto endTime = std::chrono::high_resolution_clock::now();

            std::chrono::duration<double, std::milli> multiTime = endTime - startTime;

            localMultiCycles.push_back(multiCycle);

            IteratedLocalSearchGenerator iterGenerator(&costDistanceInfo, nodesToSelect, multiTime, 10);

            startTime = std::chrono::high_resolution_clock::now();
            std::vector<int> iterCycle = iterGenerator.generateCycle(rep);
            endTime = std::chrono::high_resolution_clock::now();

            std::chrono::duration<double, std::milli> iterTime = endTime - startTime;

            localIteratedCycles.push_back(iterCycle);

            int optimizedCostMulti = multiGenerator.calculateCycleCost(multiCycle);
            int optimizedCostIter = iterGenerator.calculateCycleCost(iterCycle);

            localMultiCosts.push_back(optimizedCostMulti);
            localIteratedCosts.push_back(optimizedCostIter);

            std::cout << "MULTI" << std::endl;
            for(int n: multiCycle)
            {
                std::cout << n << " ";
            }
            std::cout << std::endl;
            std::cout << "ITER" << std::endl;
            for(int n: iterCycle)
            {
                std::cout << n << " ";
            }
            std::cout << std::endl;
            std::cout << "ITERATIONS DONE BY ITERATED LOCAL SEARCH: " << iterGenerator.iterationsDone << std::endl;
            std::cout << " LOCAL SEARCH MULTI COST: " << optimizedCostMulti << " LOCAL SEARCH ITERATED COST: " << optimizedCostIter << std::endl;

            std::cout << "LOCAL MULTI TIME: " << multiTime.count() << std::endl;

            localMultiTimes.push_back(multiTime.count());
        }
        saveResults(localMultiCycles, localMultiCosts, fileNameNoExt + "_MultistartLocalSearch.csv");
        saveResults(localIteratedCycles, localIteratedCosts, fileNameNoExt + "_IteratedLocalSearch.csv");
    }
    allTimes = {localMultiTimes};
    saveTimes(allTimes, "times.csv");
    return 0;
}
