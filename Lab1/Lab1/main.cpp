#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include "Node.h"
#include "RandomHamiltonianCycleGenerator.h"
#include "DistanceMatrix.h"

using namespace std;

int main()
{
    std::ifstream file("TSPA.csv");
    //std::ifstream file("TSPB.csv");
    if (!file.is_open()) {
        std::cerr << "Error opening file" << std::endl;
        return 1;
    }

    std::vector<Node> nodeList;

    std::string line;
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string token;
        int x, y, cost;

        std::getline(ss, token, ';');
        x = std::stoi(token);

        std::getline(ss, token, ';');
        y = std::stoi(token);

        std::getline(ss, token, ';');
        cost = std::stod(token);

        nodeList.emplace_back(x, y, cost);
    }

    file.close();

    DistanceMatrix distanceMatrix(nodeList);
    std::vector<std::vector<int>> matrix = distanceMatrix.calculateDistanceMatrix();

    std::vector<int> costList;
    for (const auto& node : nodeList) {
        costList.push_back(node.cost);
    }

    std::vector<std::vector<int>> combinedMatrix = distanceMatrix.combineCosts(matrix, costList);

    int nodes_to_select = (nodeList.size() + 1 ) / 2;

    RandomHamiltonianCycleGenerator generator(nodeList.size(), nodes_to_select, 777);

    for (int repetition; repetition < 200; repetition = repetition + 1)
    {
        std::vector<int> cycle = generator.generateCycle();

        std::cout << "Generated Random Cycle Indices: ";
        for (int index : cycle) {
            std::cout << index << " ";
        }
        std::cout << std::endl;
    }



    return 0;
}
