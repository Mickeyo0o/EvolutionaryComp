#include "HEAGenerator.h"


HEAGenerator::HEAGenerator(const CostDistanceInfo* costDistanceInfo, int nodesInCycle,
                           int popSize, bool useSimpleRecombination, bool localSearchAfterRecomb,
                           float timeForGen):
                               Generator(costDistanceInfo, nodesInCycle),
                               popSize(popSize), useSimpleRecombination(useSimpleRecombination),
                               localSearchAfterRecomb(localSearchAfterRecomb), timeForGen(timeForGen)
{

}


HEAGenerator::~HEAGenerator()
{

}

std::vector<int> HEAGenerator::generateCycle(int start_pos)
{
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    srand(seed);
    RandomHamiltonianCycleGenerator randomGenerator(costDistanceInfo, nodesInCycle, start_pos);
    SolutionWrapper population[popSize];
    std::vector<int> costsInPop;
    for(unsigned int i = 0; i < popSize; i++)
    {
        bool solOk = false;
        while(!solOk)
        {
            solOk = false;
            std::vector<int> sol = randomGenerator.generateCycle(seed);
            LocalSearchGenerator gen(costDistanceInfo, sol, false, true, start_pos);
            sol = gen.generateCycle(start_pos);
            int cost = gen.calculateCycleCost(sol);
            if(std::find(costsInPop.begin(), costsInPop.end(), cost) == costsInPop.end())
            {
                costsInPop.push_back(cost);
                solOk = true;
                population[i] = SolutionWrapper{sol, cost};
            }
        }
    }
    sortPopulation(population);
    auto startTime = std::chrono::high_resolution_clock::now();
    auto currTime = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> timePassed = currTime - startTime;
    while(timePassed.count() < timeForGen)
    {
        int parentAIndex = (rand() % popSize);
        int parentBIndexOffsetFromA = (rand() % (popSize - 1)) + 1;
        int parentBIndex = (parentAIndex + parentBIndexOffsetFromA) % popSize;
        //std::cout << parentAIndex << " " << parentBIndex << std::endl;
        SolutionWrapper child;
        if(useSimpleRecombination)
        {
            //std::cout << parentAIndex << " " << parentBIndex << std::endl;
            child = recombinationSimple(population[parentAIndex], population[parentBIndex], seed);
        }
        else
        {
            child = recombinationComplex(population[parentAIndex], population[parentBIndex], seed);
        }
        if(localSearchAfterRecomb)
        {
            LocalSearchGenerator gen(costDistanceInfo, child.solution, false, true, start_pos);
            child.solution = gen.generateCycle(start_pos);
            child.cost = gen.calculateCycleCost(child.solution);
        }
        int childIndex = popSize;
        bool addChild = true;
        for(int i = popSize - 1; i >= 0; i--)
        {
            if(child.cost > population[i].cost)
            {
                break;
            }
            else if(child.cost == population[i].cost)
            {
                addChild = false;
                break;
            }
            childIndex = i;
        }
        //std::cout << childIndex << " " << child.cost << " " << addChild << std::endl;
        if(childIndex < popSize && addChild)
        {
            for(int i = popSize - 1; i > childIndex; i--)
            {
                population[i] = population[i - 1];
            }
            population[childIndex] = child;
        }
        for(SolutionWrapper p: population)
        {
            //std::cout << p.cost << std::endl;
        }
        currTime = std::chrono::high_resolution_clock::now();
        timePassed = currTime - startTime;
        //std::cout << timePassed.count() << std::endl;
        iters++;
    }
    return population[0].solution;
}

std::vector<std::vector<int>> findCommonComponents(const std::vector<int>& a, const std::vector<int>& b)
{
    std::vector<std::vector<int>> ret;
    std::vector<int> aSorted;
    std::vector<int> bSorted;
    for(int i: a)
    {
        aSorted.push_back(i);
    }
    for(int i: b)
    {
        bSorted.push_back(i);
    }

    std::sort(aSorted.begin(), aSorted.end());
    std::sort(bSorted.begin(), bSorted.end());
    std::vector<int> commonElements;
    std::set_intersection(aSorted.begin(), aSorted.end(), bSorted.begin(), bSorted.end(), std::back_inserter(commonElements));
    for(int i: commonElements)
    {
      //  std::cout << i << " ";
    }
    //std::cout << std::endl;
    //std::cout << commonElements.size() << std::endl;
    while(!commonElements.empty())
    {
        //std::cout << "COMMON SIZE: " << commonElements.size() << std::endl;
        std::vector<int> currentCommonComponent;
        int el = commonElements.back();
        auto aPos = std::find(a.begin(), a.end(), el);
        auto bPos = std::find(b.begin(), b.end(), el);
        //std::cout << "BEGIN NEW COMP: " << *aPos << " " << *bPos << " " << el << std::endl;
        bool currentComponentAddBegin = false;
        do
        {
            el = *aPos;
            currentCommonComponent.push_back(el);
            auto commonElementIter = std::find(commonElements.begin(), commonElements.end(), el);
            if(commonElementIter != commonElements.end())
            {
                commonElements.erase(commonElementIter);
            }
            else
            {
                currentComponentAddBegin = true;
            }
            aPos = std::next(aPos, 1);
            bPos = std::next(bPos, 1);
            if(aPos == a.cend())
            {
                aPos = a.cbegin();
            }
            if(bPos == b.cend())
            {
                bPos = b.cbegin();
            }
            //std::cout << *aPos << " " << *bPos << " " << el << std::endl;
        } while(*aPos == *bPos);
        //std::cout << "COMPONENT FOUND" << std::endl;
        if(currentComponentAddBegin)
        {
            //std::cout << "ADD TO FIRST" << std::endl;
            std::vector<int> firstComp = ret.front();
            for(auto it = currentCommonComponent.rbegin(); it != currentCommonComponent.rend(); ++it)
            {
                firstComp.insert(firstComp.begin(), *it);
            }
        }
        else
        {
            ret.push_back(currentCommonComponent);
        }
    }
    return ret;
}

HEAGenerator::SolutionWrapper HEAGenerator::recombinationSimple(const SolutionWrapper& parentA, const SolutionWrapper& parentB, unsigned seed)
{
    std::vector<std::vector<int>> commonParts = findCommonComponents(parentA.solution, parentB.solution);
    std::vector<int> commonElements;
    for(auto v: commonParts)
    {
        //std::cout << "COMMON PART: ";
        for(int i: v)
        {
            //std::cout << i << " ";
            commonElements.push_back(i);
        }
        //std::cout << "\n";
    }
    std::vector<int> unusedNodes;
    for(int i = 0; i < costDistanceInfo->getNumOfNodes(); i++)
    {
        if(std::find(commonElements.begin(), commonElements.end(), i) == commonElements.end())
        {
            unusedNodes.push_back(i);
        }
    }
    std::shuffle(commonParts.begin(), commonParts.end(), std::default_random_engine(seed));
    std::shuffle(unusedNodes.begin(), unusedNodes.end(), std::default_random_engine(seed));
    int gapsToFillSize = nodesInCycle - commonElements.size();
    int gapCount = commonParts.size();
    std::vector<int> gapsSize;
    for(int i = 0; i < gapCount; i++)
    {
        int gap = (rand() % gapsToFillSize);
        gapsToFillSize -= gap;
        gapsSize.push_back(gap);
    }
    gapsSize.push_back(gapsToFillSize);
    std::shuffle(gapsSize.begin(), gapsSize.end(), std::default_random_engine(seed));
    std::vector<int> newSolution;
    while(!commonParts.empty())
    {
        int gapSize = gapsSize.back();
        gapsSize.pop_back();
        for(int i = 0; i < gapSize; i++)
        {
            int nodeId = unusedNodes.back();
            unusedNodes.pop_back();
            newSolution.push_back(nodeId);
        }
        std::vector<int> currCommonPart = commonParts.back();
        commonParts.pop_back();
        for(int nodeId: currCommonPart)
        {
            newSolution.push_back(nodeId);
        }
    }
    //std::cout << gapsSize.size() << std::endl;
    int gapSize = gapsSize.back();
    gapsSize.pop_back();
    for(int i = 0; i < gapSize; i++)
    {
        int nodeId = unusedNodes.back();
        unusedNodes.pop_back();
        newSolution.push_back(nodeId);
    }
    RandomHamiltonianCycleGenerator gen(costDistanceInfo, nodesInCycle, seed); // This is used only to calculate the cycle cost
    int cost = gen.calculateCycleCost(newSolution);
    return SolutionWrapper{newSolution, cost};
}

HEAGenerator::SolutionWrapper HEAGenerator::recombinationComplex(const SolutionWrapper& parentA, const SolutionWrapper& parentB, unsigned seed)
{
    std::vector<std::vector<int>> commonParts = findCommonComponents(parentA.solution, parentB.solution);
    std::shuffle(commonParts.begin(), commonParts.end(), std::default_random_engine(seed));
    std::vector<int> solution;
    for(auto v: commonParts)
    {
        for(int i: v)
        {
            solution.push_back(i);
        }
    }
    KRegretGreedyCycleCombinationGenerator kregretGenerator(costDistanceInfo, nodesInCycle, 0.6f);
    std::vector<int> newSolution = kregretGenerator.generateCycle(0, solution);
    int cost = kregretGenerator.calculateCycleCost(newSolution);
    //std::cout << cost << std::endl;
    return SolutionWrapper{newSolution, cost};
}

void HEAGenerator::sortPopulation(SolutionWrapper* population)
{
    std::sort(population, population + popSize, [](const SolutionWrapper& a, const SolutionWrapper& b){
        return a.cost < b.cost;
    });
}


