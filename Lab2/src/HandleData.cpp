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

void saveResults(std::vector<std::vector<int>> cycles, std::vector<int> costs, std::string fileName) {
    std::ofstream file(fileName);
    if(!file.is_open()) {
        std::cerr << "Error opening file" << std::endl;
        throw 1;
    }
    for (size_t i = 0; i < cycles.size(); ++i) {
        for (int node_id : cycles[i]) {
            file << node_id << ",";
        }
        file << costs[i] << std::endl;  // Add the cost at the end of each cycle
    }
    file.close();
}

void saveTimes(std::vector<std::vector<double>> times, std::string fileName) {
    std::ofstream file(fileName);
    if (!file.is_open()) {
        std::cerr << "Error opening file" << std::endl;
        throw 1;
    }

    for (size_t row = 0; row < times[0].size(); ++row) {
        for (size_t col = 0; col < times.size(); ++col) {
            file << times[col][row] << ",";
        }
        file << "\n";
    }
    file.close();
}
