#include "HEAGenerator.h"


HEAGenerator::HEAGenerator(const CostDistanceInfo* costDistanceInfo, int nodesInCycle,
                           int popSize, float timeForGen):
                               Generator(costDistanceInfo, nodesInCycle),
                               popSize(popSize), timeForGen(timeForGen)
{

}


HEAGenerator::~HEAGenerator()
{

}

/*
    This method calculates a penalty part of the objective func - if the solution has
    more than thresh nodes from parents (basically if the parents of the solution had
    more than thresh common nodes), then penalty is applied to that child. The threshold
    increases with iterations - th eidea is that initially we want diversity, but then when
    the algorithm is close to the end, we focus more on exploitation, thus we allow more
    similar solutions in the population
*/
float HEAGenerator::penalty(const HEAGenerator::SolutionWrapper& solution, int thresh)
{
    if(solution.nodesFromParentsCount < thresh) return 0;
    else return (solution.nodesFromParentsCount - thresh) * 4;
}

/*
    Select a parent proportionally to its fitness
*/
int HEAGenerator::getParentId(SolutionWrapper* pop, int popSize)
{
    float* costs = new float[popSize];
    float sum = 0;
    const float thresh = 69800; // This threshold boosts the probability of being chosen for the solutions that have greater value of obj func
    for(int i = 0; i < popSize; i++)
    {
        float tmp = 1.f / pop[i].costNoPenalty;
        if(pop[i].costNoPenalty < thresh)
        {
            tmp += (thresh - pop[i].costNoPenalty)/1000.f;
        }
        costs[i] = tmp;
        sum += tmp;
    }
    for(int i = 0; i < popSize; i++)
    {
        costs[i] /= sum;
        if(i > 0) costs[i] += costs[i - 1];
    }
    float r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
    int id = -1;
    for(int i = 0; i < popSize; i++)
    {
        if(r < costs[i])
        {
            id = i;
            break;
        }
    }
    delete [] costs;
    return id;
}

/*
    Select both parents
*/
std::vector<int> HEAGenerator::getParents(SolutionWrapper* pop)
{
    int parentA = getParentId(pop, popSize);
    SolutionWrapper* tmp = new SolutionWrapper[popSize - 1];
    for(int i = 1; i < popSize; i++)
    {
        tmp[i - 1] = pop[i];
    }
    int parentB = getParentId(tmp, popSize - 1);
    if(parentB >= parentA) parentB += 1;
    delete [] tmp;
    std::vector<int> ret;
    ret.push_back(parentA);
    ret.push_back(parentB);
    return ret;
}

std::vector<int> HEAGenerator::generateCycle(int start_pos)
{
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    srand(seed);
    KRegretGreedyCycleCombinationGenerator kGenerator(costDistanceInfo, nodesInCycle, .3f);
    RandomHamiltonianCycleGenerator randomGenerator(costDistanceInfo, nodesInCycle, seed);
    SolutionWrapper population[popSize];
    std::vector<int> costsInPop;
    int triesCount = start_pos * popSize;
    int i = 0;
    const int nnSize = 5; // The size of the candidate nodes list for candidate local search - It seems that 5 is the optimal value, but you can try with 4 or 6
    while(i < popSize) // Here we initialize the population. 4 instances are created with kGreedy constr heuristic, the rest are random. Then the solutions
                        // are optimized with candidate local search - the idea is to introduce variance initially, thus we use two types of pop creation
    {
        bool solOk = false;
        while(!solOk)
        {
            solOk = false;
            std::vector<int> sol;
            if(triesCount < costDistanceInfo->getNumOfNodes() && i < 4) // change this condition to add more kGreedy instances, however 4 seems to work pretty well
            {
                sol = kGenerator.generateCycle(triesCount);
            }
            else
            {
                sol = randomGenerator.generateCycle(triesCount % costDistanceInfo->getNumOfNodes());
            }
            //std::cout << sol.size() << std::endl;
            LocalSearchCandidateMovesGenerator gen(costDistanceInfo, nodesInCycle, sol, nnSize);
            sol = gen.generateCycle(start_pos);
            int cost = randomGenerator.calculateCycleCost(sol);
            if(std::find(costsInPop.begin(), costsInPop.end(), cost) == costsInPop.end())
            {
                costsInPop.push_back(cost);
                //std::cout << cost << std::endl;
                solOk = true;
                population[i] = SolutionWrapper{sol, cost, cost, 0};
                i++;
            }
            triesCount++;
        }
    }
    sortPopulation(population);
    auto startTime = std::chrono::high_resolution_clock::now();
    auto currTime = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> timePassed = currTime - startTime;
    int iterCount = 0;
    while(timePassed.count() < timeForGen)
    {
        int thresh = std::min(90.0, 10*exp(float(iterCount) / 750.f) + 40);
        int mutThresh = std::max(1.0, 15*exp(-float(iterCount) / 500.f) + 1);
        // The formulas above regulate the penalty threshold and the number of mutations, you can change the
        // coefficients freely, because I'm not confident this is the best set.
        //std::cout << "THRESH: " << thresh << " MUT THRESH: " << mutThresh << std::endl;
        std::vector<int> ps = getParents(population);
        int parentAIndex = ps[0];
        int parentBIndex = ps[1];
        //std::cout << parentAIndex << " " << parentBIndex << std::endl;
        SolutionWrapper child;
            //std::cout << parentAIndex << " " << parentBIndex << std::endl;
        child = recombinationComplex(population[parentAIndex], population[parentBIndex], ++seed);
        if(static_cast <float> (rand()) / static_cast <float> (RAND_MAX) < 0.6f) mutate(&child, mutThresh); // 0.6f is the probability of mutation, feel free to change it
        LocalSearchCandidateMovesGenerator gen(costDistanceInfo, nodesInCycle, child.solution, nnSize);
        child.solution = gen.generateCycle(start_pos);
        child.costNoPenalty = gen.calculateCycleCost(child.solution);
        child.cost = child.costNoPenalty + penalty(child, thresh);
        //float childPen = penalty(child, thresh);
        int childIndex = popSize;
        bool addChild = true;
        for(int i = popSize - 1; i >= 0; i--)
        {
            if(child.costNoPenalty == population[i].costNoPenalty)
            {
                addChild = false;
                break;
            }
            else if(child.cost > population[i].cost)
            {
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
        iterCount++;
        //std::cout << "============" << std::endl;
        for(int i = 0; i < popSize; i++)
        {
            //std::cout << population[i].cost << std::endl;
        }
    }
    return population[0].solution;
}

HEAGenerator::SolutionWrapper* HEAGenerator::mutate(SolutionWrapper* solution, int mutThresh)
{
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> moveType(0,1);
    std::vector<int> currentCycle = solution->solution;
    for(int i = 0; i < mutThresh; i++)
    {
        bool isInterMove = moveType(rng) == 0;
        if(isInterMove)
        {
            std::vector<int> nodesNotInCycle;
            for(int i = 0; i < costDistanceInfo->getNumOfNodes(); i++)
            {
                if(std::find(currentCycle.begin(), currentCycle.end(), i) == currentCycle.end())
                {
                    nodesNotInCycle.push_back(i);
                }
            }
            std::uniform_int_distribution<std::mt19937::result_type> nodeInCycleIdIndex(0, currentCycle.size() - 1);
            std::uniform_int_distribution<std::mt19937::result_type> nodeNotInCycleIdIndex(0, nodesNotInCycle.size() - 1);
            InterNodeNeighbourhoodMoveLocalSearch localMove(nodeInCycleIdIndex(rng), nodesNotInCycle[nodeNotInCycleIdIndex(rng)]);
            localMove.performMove(currentCycle, NULL);
        }
        else
        {
            std::uniform_int_distribution<std::mt19937::result_type> nodeCycleIdIndex(0, currentCycle.size() - 1);
            int node1Index = nodeCycleIdIndex(rng);
            int node2Index = nodeCycleIdIndex(rng);
            if(abs(node1Index - node2Index) < 2)
            {
                node2Index = (node2Index + 2) % currentCycle.size();
            }
            IntraNodeChangeEdgeNeighbourhoodMoveLocalSearch localMove(node1Index, node2Index);
            localMove.performMove(currentCycle, NULL);
        }
    }

    solution->solution = currentCycle;
    return solution;
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
    //std::shuffle(commonParts.begin(), commonParts.end(), std::default_random_engine(seed));
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
    //std::shuffle(commonParts.begin(), commonParts.end(), std::default_random_engine(seed));
    std::vector<int> solution;
    int c = 0;
    for(auto v: commonParts)
    {
        for(int i: v)
        {
            solution.push_back(i);
            c++;
        }
    }
    //std::cout << c << std::endl;
    KRegretGreedyCycleCombinationGenerator kregretGenerator(costDistanceInfo, nodesInCycle, 0.3f); //0.3f seems to work well
    std::vector<int> newSolution = kregretGenerator.generateCycle(0, solution);
    int cost = kregretGenerator.calculateCycleCost(newSolution);
    return SolutionWrapper{newSolution, cost, c};
}

void HEAGenerator::sortPopulation(SolutionWrapper* population)
{
    std::sort(population, population + popSize, [](const SolutionWrapper& a, const SolutionWrapper& b){
        return a.cost < b.cost;
    });
}


