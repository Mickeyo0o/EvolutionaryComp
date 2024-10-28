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
#include "KRegretGreedyCycleCombinationGenerator.h"

using namespace std;

int main()
{

    const std::string fileNames[] = {"TSPA", "TSPB"};
    std::vector<std::vector<double>> allTimes;


    for(size_t fileNameId = 0; fileNameId <= 1; fileNameId++) {
        std::string fileNameNoExt = fileNames[fileNameId];
        std::cout << fileNameNoExt;
        std::string fileName = fileNameNoExt + ".csv";
        std::vector<Node> nodeList = loadNodes(fileName);

        CostDistanceInfo costDistanceInfo(nodeList);

        int nodesToSelect = (nodeList.size() + 1 ) / 2;

        for(size_t greedyI = 0; greedyI <= 1; greedyI++)
        {
            bool isGreedy = greedyI == 0;
            for(size_t swapEdgesI = 0; swapEdgesI <= 1; swapEdgesI++)
            {
                bool swapEdges = swapEdgesI == 0;
                for(size_t useRandomStartI = 0; useRandomStartI <= 1; useRandomStartI++)
                {
                    bool useRandomStart = useRandomStartI == 0;
                    std::vector<std::vector<int>> cycles;
                    std::cout<<"Instance name: " << fileNameNoExt << " isGreedy: " << isGreedy << " swapEdges: " << swapEdges << " useRandomStart: " << useRandomStart << std::endl;
                    std::vector<int> costs;
                    std::vector<double> times;
                    for (int repetition = 0; repetition < 200; repetition++)
                    {
                        std::cout << repetition << std::endl;

                        std::vector<int> initialCycle;
                        if(useRandomStart)
                        {
                            RandomHamiltonianCycleGenerator randomGenerator(&costDistanceInfo, nodesToSelect, repetition);
                            initialCycle = randomGenerator.generateCycle(repetition);
                            std::cout << "Score before: " << randomGenerator.calculateCycleCost(initialCycle) << " ";
                        }
                        else
                        {
                            KRegretGreedyCycleCombinationGenerator regretGenerator(&costDistanceInfo, nodesToSelect, 0.5f);
                            initialCycle = regretGenerator.generateCycle(repetition);
                            std::cout << "Score before: " << regretGenerator.calculateCycleCost(initialCycle) << " ";
                        }


                        auto start_time = std::chrono::high_resolution_clock::now();
                        LocalSearchGenerator localRandomGenerator(&costDistanceInfo, initialCycle, isGreedy, swapEdges, repetition);
                        std::vector<int> optimizedInitialCycle = localRandomGenerator.generateCycle(repetition);
                        auto end_time = std::chrono::high_resolution_clock::now();
                        std::chrono::duration<double, std::milli> timePeriod = end_time - start_time;
                        times.push_back(timePeriod.count());

                        costs.push_back(localRandomGenerator.calculateCycleCost(optimizedInitialCycle));
                        for(int n: optimizedInitialCycle) {
                            std::cout << n << " ";
                        }
                        std::cout << std::endl;
                        cycles.push_back(optimizedInitialCycle);
                    }
                    saveResults(cycles, costs, fileNameNoExt + (isGreedy?"Greedy":"Steepest") + (swapEdges?"IntraSwapEdges":"IntraSwapNodes") + (useRandomStart?"RandomStart":"kRegretWeightedStart")+".csv");
                    allTimes.push_back(times);
                }
            }
        }
    }
    saveTimes(allTimes, "times.csv");
    return 0;
}
