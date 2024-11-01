#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <chrono>
#include "HandleData.h"
#include "RandomHamiltonianCycleGenerator.h"
#include "LocalSearchCandidateMovesGenerator.h"
#include "LocalSearchGenerator.h"

using namespace std;

int main()
{
    const std::string fileNames[] = {"TSPA", "TSPB"};

    for(size_t fileNameId = 0; fileNameId <= 1; fileNameId++) {
        std::string fileNameNoExt = fileNames[fileNameId];
        std::cout << fileNameNoExt << std::endl;
        std::string fileName = fileNameNoExt + ".csv";
        std::vector<Node> nodeList = loadNodes(fileName);

        CostDistanceInfo costDistanceInfo(nodeList);

        int nodesToSelect = (nodeList.size() + 1 ) / 2;

        std::vector<std::vector<int>> localNormalCycles;
        std::vector<std::vector<int>> localCandidateCycles;

        for(int rep = 0; rep < 200; rep++)
        {
            RandomHamiltonianCycleGenerator randomGeneraror(&costDistanceInfo, nodesToSelect, rep);
            std::vector<int> randomCycle = randomGeneraror.generateCycle(rep);

            LocalSearchCandidateMovesGenerator localCandidateGenerator(&costDistanceInfo, nodesToSelect, randomCycle);
            auto startTime = std::chrono::high_resolution_clock::now();
            std::vector<int> optimizedCycleCandidate = localCandidateGenerator.generateCycle(rep);
            auto endTime = std::chrono::high_resolution_clock::now();

            localCandidateCycles.push_back(optimizedCycleCandidate);

            std::chrono::duration<double, std::milli> candidateTime = endTime - startTime;

            LocalSearchGenerator localSearchGenerator(&costDistanceInfo, randomCycle);
            startTime = std::chrono::high_resolution_clock::now();
            std::vector<int> optimizedCycle = localSearchGenerator.generateCycle(rep);
            endTime = std::chrono::high_resolution_clock::now();

            localNormalCycles.push_back(optimizedCycle);

            std::chrono::duration<double, std::milli> normalTime = endTime - startTime;

            int randomCost = randomGeneraror.calculateCycleCost(randomCycle);
            int optimizedCost = localSearchGenerator.calculateCycleCost(optimizedCycle);
            int optimizedCandidateCost = localCandidateGenerator.calculateCycleCost(optimizedCycleCandidate);

            for(int n: optimizedCycleCandidate)
            {
                std::cout << n << " ";
            }
            std::cout << std::endl;
            std::cout << "RANDOM COST: " << randomCost << " LOCAL SEARCH NORMAL COST: " << optimizedCost << " LOCAL SEARCH CANDIDATE COST: " << optimizedCandidateCost << std::endl;

            std::cout << "LOCAL NORMAL TIME: " << normalTime.count() << " LOCAL CANDIDATE TIME: " << candidateTime.count() << std::endl;
        }

        saveResults(localNormalCycles, fileNameNoExt + "_NormalLocalSearch.csv");
        saveResults(localCandidateCycles, fileNameNoExt + "_CandidateLocalSearch.csv");
    }
    return 0;
}
