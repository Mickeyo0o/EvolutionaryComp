#ifndef HANDLEDATA_H
#define HANDLEDATA_H

#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include "Node.h"

std::vector<Node> loadNodes(std::string fileName);
void saveResults(std::vector<std::vector<int>> cycles, std::string fileName);

#endif // HANDLEDATA_H
