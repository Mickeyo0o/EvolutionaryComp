#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <chrono>
#include "HandleData.h"
#include "Node.h"
#include "CostDistanceInfo.h"
#include "KRegretGenerator.h"
#include "KRegretGreedyCycleCombinationGenerator.h"

int main()
{
    std::vector<double> KRegretTimes;
    std::vector<double> KRegretCombTimes;

    const std::string fileNames[] = {"TSPA", "TSPB"};
    for(size_t fileNameId = 0; fileNameId <= 1; fileNameId++) {
        std::string fileNameNoExt = fileNames[fileNameId];
        std::cout << fileNameNoExt << std::endl;
        std::string fileName = fileNameNoExt + ".csv";
        std::vector<Node> nodeList = loadNodes(fileName);

        CostDistanceInfo costDistanceInfo(nodeList);

        int nodesToSelect = (nodeList.size() + 1 ) / 2;
        KRegretGenerator kRegretGenerator(&costDistanceInfo, nodesToSelect);
        KRegretGreedyCycleCombinationGenerator kRegretGreedyCycleCombinationGenerator(&costDistanceInfo, nodesToSelect, 0.5f);

        std::vector<std::vector<int>> kRegretCycles;
        std::vector<std::vector<int>> kRegretGreedyCycles;

        std::vector<int> kRegretCosts;
        std::vector<int> kRegretGreedyCosts;

        for (int repetition = 0; repetition < 200; repetition++)
        {
            std::cout << repetition << std::endl;

            auto start_time = std::chrono::high_resolution_clock::now();
            std::vector<int> kRegretCycle = kRegretGenerator.generateCycle(repetition);
            auto end_time = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double, std::milli> timePeriod = end_time - start_time;
            KRegretTimes.push_back(timePeriod.count());
            kRegretCycles.push_back(kRegretCycle);

            std::cout << "KRegretCycle: " << std::endl;
            for(int i: kRegretCycle) {
                std::cout << i << " ";
            }
            std::cout << std::endl;

            int cost = kRegretGenerator.calculateCycleCost(kRegretCycle);
            kRegretCosts.push_back(cost);
            std::cout << std::endl << "Cycle cost: " << cost;
            std::cout << std::endl;

            start_time = std::chrono::high_resolution_clock::now();
            std::vector<int> kRegretGreedyCycle = kRegretGreedyCycleCombinationGenerator.generateCycle(repetition);
            end_time = std::chrono::high_resolution_clock::now();
            timePeriod = end_time - start_time;
            KRegretCombTimes.push_back(timePeriod.count());
            kRegretGreedyCycles.push_back(kRegretGreedyCycle);

            std::cout << "KRegretGreedyCycleCombinationCycle: " << std::endl;
            for(int i: kRegretGreedyCycle) {
                std::cout << i << " ";
            }
            cost = kRegretGreedyCycleCombinationGenerator.calculateCycleCost(kRegretGreedyCycle);
            kRegretGreedyCosts.push_back(cost);
            std::cout << std::endl << "Cycle cost: " << cost;
            std::cout << std::endl;
        }
        saveResults(kRegretCycles, kRegretCosts, fileNameNoExt + "KRegret.csv");
        saveResults(kRegretGreedyCycles, kRegretGreedyCosts, fileNameNoExt + "KRegretGreedyCombination.csv");
    }

    std::vector<std::vector<double>> times = {KRegretTimes, KRegretCombTimes};
    saveTimes(times, "times.csv");
    return 0;
}
