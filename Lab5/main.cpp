#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <chrono>
#include "HandleData.h"
#include "RandomHamiltonianCycleGenerator.h"
#include "LocalSearchGenerator.h"
#include "LocalSearchDeltaGenerator.h"

using namespace std;

int main()
{
    const std::string fileNames[] = {"TSPA", "TSPB"};
    std::vector<std::vector<double>> allTimes;
    std::vector<double> localNormalTimes;
    std::vector<double> localDeltaTimes;
    std::vector<double> randomTimes;

    for(size_t fileNameId = 0; fileNameId <= 1; fileNameId++) {
        std::string fileNameNoExt = fileNames[fileNameId];
        std::cout << fileNameNoExt << std::endl;
        std::string fileName = fileNameNoExt + ".csv";
        std::vector<Node> nodeList = loadNodes(fileName);

        CostDistanceInfo costDistanceInfo(nodeList);

        int nodesToSelect = (nodeList.size() + 1 ) / 2;

        std::vector<std::vector<int>> randomCycles;
        std::vector<std::vector<int>> localNormalCycles;
        std::vector<std::vector<int>> localDeltaCycles;

        std::vector<int> randomCosts;
        std::vector<int> localNormalCosts;
        std::vector<int> localDeltaCosts;

        for(int rep = 0; rep < 200; rep++)
        {
            RandomHamiltonianCycleGenerator randomGeneraror(&costDistanceInfo, nodesToSelect, rep);
            auto startTime = std::chrono::high_resolution_clock::now();
            std::vector<int> randomCycle = randomGeneraror.generateCycle(rep);
            auto endTime = std::chrono::high_resolution_clock::now();

            std::chrono::duration<double, std::milli> randomTime = endTime - startTime;

            randomCycles.push_back(randomCycle);

            LocalSearchGenerator localSearchGenerator(&costDistanceInfo, randomCycle);
            startTime = std::chrono::high_resolution_clock::now();
            std::vector<int> optimizedCycle = localSearchGenerator.generateCycle(rep);
            endTime = std::chrono::high_resolution_clock::now();

            localNormalCycles.push_back(optimizedCycle);

            std::chrono::duration<double, std::milli> normalTime = endTime - startTime;

            LocalSearchDeltaGenerator localSearchDeltaGenerator(&costDistanceInfo, randomCycle);
            startTime = std::chrono::high_resolution_clock::now();
            std::vector<int> optimizedCycleDelta = localSearchDeltaGenerator.generateCycle(rep);
            endTime = std::chrono::high_resolution_clock::now();

            localDeltaCycles.push_back(optimizedCycleDelta);

            std::chrono::duration<double, std::milli> deltaTime = endTime - startTime;

            int randomCost = randomGeneraror.calculateCycleCost(randomCycle);
            int optimizedCost = localSearchGenerator.calculateCycleCost(optimizedCycle);
            int optimizedCostDelta = localSearchDeltaGenerator.calculateCycleCost(optimizedCycleDelta);

            randomCosts.push_back(randomCost);
            localNormalCosts.push_back(optimizedCost);
            localDeltaCosts.push_back(optimizedCostDelta);

            for(int n: optimizedCycleDelta)
            {
                //std::cout << n << " ";
            }
            //std::cout << std::endl;
            std::cout << "RANDOM COST: " << randomCost << " LOCAL SEARCH NORMAL COST: " << optimizedCost << " LOCAL SEARCH DELTA COST: " << optimizedCostDelta << std::endl;

            std::cout << "LOCAL NORMAL TIME: " << normalTime.count() << " LOCAL DELTA TIME: " << deltaTime.count() <<std::endl;

            randomTimes.push_back(randomTime.count());
            localNormalTimes.push_back(normalTime.count());
            localDeltaTimes.push_back(deltaTime.count());
        }
        int avgNormalCost = 0;
        int avgDeltaCost = 0;
        for(int c: localNormalCosts)
        {
            avgNormalCost += c;
        }
        for(int c: localDeltaCosts)
        {
            avgDeltaCost += c;
        }
        std::cout << "\n\n\n\n\n\n\n\n\nNORMAL AVG: "<<avgNormalCost / localNormalCosts.size() << " DELTA AVG: " << avgDeltaCost / localDeltaCosts.size() << std::endl;
        /*saveResults(randomCycles, randomCosts, fileNameNoExt + "_Random.csv");
        saveResults(localNormalCycles, localNormalCosts, fileNameNoExt + "_NormalLocalSearch.csv");
        saveResults(localCandidateCycles, localCandidateCosts, fileNameNoExt + "_CandidateLocalSearch.csv");*/
    }
    allTimes = {randomTimes, localNormalTimes};
    //saveTimes(allTimes, "times.csv");
    return 0;
}
