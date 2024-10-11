#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include "HandleData.h"
#include "Node.h"
#include "RandomHamiltonianCycleGenerator.h"
#include "CostDistanceInfo.h"
#include "NNLastGenerator.h"
#include "NNAllGenerator.h"
#include "GreedyCycleGenerator.h"

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

        RandomHamiltonianCycleGenerator generatorRandom(&costDistanceInfo, nodesToSelect, 777);
        NNLastGenerator generatorNNLast(&costDistanceInfo, nodesToSelect);
        NNAllGenerator generatorNNAll(&costDistanceInfo, nodesToSelect);
        GreedyCycleGenerator generatorGreedyCycle(&costDistanceInfo, nodesToSelect);

        std::vector<std::vector<int>> randomCycles;
        std::vector<std::vector<int>> NNLastCycles;
        std::vector<std::vector<int>> NNAllCycles;
        std::vector<std::vector<int>> greedyCycles;

        std::vector<int> randomCosts;
        std::vector<int> NNLastCosts;
        std::vector<int> NNAllCosts;
        std::vector<int> greedyCosts;

        for (int repetition = 0; repetition < 200; repetition++)
        {
            std::cout << repetition << std::endl;

            std::vector<int> cycleRandom = generatorRandom.generateCycle(repetition);
            randomCycles.push_back(cycleRandom);

            std::cout << "Generated Random Cycle Indices: ";
            for (int index : cycleRandom) {
                std::cout << index << " ";
            }
            int cost = generatorRandom.calculateCycleCost(cycleRandom);
            randomCosts.push_back(cost);
            std::cout << std::endl << "Cycle cost: " << cost;
            std::cout << std::endl;

            std::vector<int> cycleNNLast = generatorNNLast.generateCycle(repetition);
            NNLastCycles.push_back(cycleNNLast);

            std::cout << "Generated NNLast Cycle Indices: ";
            for (int index : cycleNNLast) {
                std::cout << index << " ";
            }
            cost = generatorNNLast.calculateCycleCost(cycleNNLast);
            NNLastCosts.push_back(cost);
            std::cout << std::endl << "Cycle cost: " << cost;
            std::cout << std::endl;

            std::vector<int> cycleNNAll = generatorNNAll.generateCycle(repetition);
            NNAllCycles.push_back(cycleNNAll);

            std::cout << "Generated NNAll Cycle Indices: ";
            for (int index : cycleNNAll) {
                std::cout << index << " ";
            }
            cost = generatorNNLast.calculateCycleCost(cycleNNAll);
            NNAllCosts.push_back(cost);
            std::cout << std::endl << "Cycle cost: " << cost;
            std::cout << std::endl;

            std::vector<int> cycleGreedy = generatorGreedyCycle.generateCycle(repetition);
            greedyCycles.push_back(cycleGreedy);

            std::cout << "Generated Greedy Cycle Indices: ";
            for (int index : cycleGreedy) {
                std::cout << index << " ";
            }

            cost = generatorGreedyCycle.calculateCycleCost(cycleGreedy);
            greedyCosts.push_back(cost);
            std::cout << std::endl << "Cycle cost: " << cost;
            std::cout << std::endl;
        }
        saveResults(randomCycles, randomCosts, fileNameNoExt + "Random.csv");
        saveResults(NNLastCycles, NNLastCosts, fileNameNoExt + "NNLast.csv");
        saveResults(NNAllCycles, NNAllCosts, fileNameNoExt + "NNAll.csv");
        saveResults(greedyCycles, greedyCosts, fileNameNoExt + "Greedy.csv");
    }
    return 0;
}