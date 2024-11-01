#include "HandleData.h"

std::vector<Node> loadNodes(std::string fileName) {
    std::ifstream file(fileName);
    if (!file.is_open()) {
        std::cerr << "Error opening file" << std::endl;
        throw 1;
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

    return nodeList;
}

void saveResults(std::vector<std::vector<int>> cycles, std::string fileName) {
    std::ofstream file(fileName);
    if(!file.is_open()) {
        std::cerr << "Error opening file" << std::endl;
        throw 1;
    }
    for(std::vector<int> cycle: cycles) {
        for(int node_id: cycle) {
            file << node_id << ",";
        }
        file << std::endl;
    }
    file.close();
}
