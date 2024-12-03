#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <chrono>
#include "HandleData.h"
#include "RandomHamiltonianCycleGenerator.h"
#include "LocalSearchGenerator.h"
#include "LNSGenerator.h"

using namespace std;

int main()
{
    const std::string fileNames[] = {"TSPA", "TSPB"};
    const double correctTimesPerFile[] = {36404.82, 34441.575};

    for(size_t fileNameId = 0; fileNameId <= 1; fileNameId++)
    {
        std::string fileNameNoExt = fileNames[fileNameId];
        std::cout << fileNameNoExt << std::endl;
        std::string fileName = fileNameNoExt + ".csv";
        std::vector<Node> nodeList = loadNodes(fileName);

        std::vector<std::vector<int>> lnsLocalSearchCycles;
        std::vector<std::vector<int>> lnsCycles;
        std::vector<int> lnsLsCosts, lnsCosts;
        std::vector<int> lnsLsIters, lnsIters;


        CostDistanceInfo costDistanceInfo(nodeList);

        int nodesToSelect = (nodeList.size() + 1 ) / 2;

        LNSGenerator lnsGenerator(&costDistanceInfo, nodesToSelect, correctTimesPerFile[fileNameId], 0.25);

        for(int rep = 0; rep < 20; rep++)
        {
            std::cout << "REPETITION " << rep << std::endl;
            lnsGenerator.setLocalSearchFlag(false);
            std::vector<int> lnsNolsCycle = lnsGenerator.generateCycle(rep);
            lnsCycles.push_back(lnsNolsCycle);
            int lnsCycleScore = lnsGenerator.calculateCycleCost(lnsNolsCycle);
            lnsCosts.push_back(lnsCycleScore);
            int lnsNoLsIter = lnsGenerator.lastIterCount;
            lnsIters.push_back(lnsNoLsIter);

            lnsGenerator.setLocalSearchFlag(true);
            std::vector<int> lnsLsCycle = lnsGenerator.generateCycle(rep);
            lnsLocalSearchCycles.push_back(lnsLsCycle);
            int lnsLsCycleScore = lnsGenerator.calculateCycleCost(lnsLsCycle);
            lnsLsCosts.push_back(lnsLsCycleScore);
            int lnsIter = lnsGenerator.lastIterCount;
            lnsLsIters.push_back(lnsIter);

            std::cout << "LNS LS SCORE: " << lnsLsCycleScore << " ITERS: " << lnsIter << " LNS NO LS SCORE " << lnsCycleScore << " ITERS: " << lnsNoLsIter << std::endl;
        }
        saveResults(lnsLocalSearchCycles, lnsLsCosts, fileNameNoExt + "_LnsWithLS.csv");
        saveResults(lnsCycles, lnsCosts, fileNameNoExt + "_LnsNoLS.csv");
        saveRuns(lnsIters, fileNameNoExt + "_LnsNoLS_iters.csv");
        saveRuns(lnsLsIters, fileNameNoExt + "_LnsWithLS_iters.csv");
    }

    return 0;
}
