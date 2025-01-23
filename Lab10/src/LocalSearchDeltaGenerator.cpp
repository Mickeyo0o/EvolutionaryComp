#include "LocalSearchDeltaGenerator.h"

LocalSearchDeltaGenerator::LocalSearchDeltaGenerator(const CostDistanceInfo* costDistanceInfo, std::vector<int>initialCycle):
    Generator(costDistanceInfo, initialCycle.size()), initialCycle(initialCycle)
{

}

LocalSearchDeltaGenerator::~LocalSearchDeltaGenerator()
{
    //dtor
}

bool _cmp(NeighbourhoodMove* a, NeighbourhoodMove* b)
{
    if(a->functionDelta == b->functionDelta)
    {
        return a > b;
    }
    else return a->functionDelta < b->functionDelta;
}

std::vector<int> LocalSearchDeltaGenerator::generateCycle(int start_pos)
{
    std::vector<int> currentCycle;
    for(int nodeId: initialCycle)
    {
        currentCycle.push_back(nodeId);
    }
    int* positionInCycleCache = new int[costDistanceInfo->getNumOfNodes()];
    int posInCycle = 0;
    for(size_t i = 0; i < costDistanceInfo->getNumOfNodes(); i++)
    {
        positionInCycleCache[i] = -1; //-1 means element at i is not in the cycle
    }
    for(int elInCycle: currentCycle)
    {
        positionInCycleCache[elInCycle] = posInCycle;
        posInCycle++;
    }
    std::set<NeighbourhoodMove*, bool(*)(NeighbourhoodMove*, NeighbourhoodMove*)> sortedMovesList(_cmp);
    std::vector<int> enteringNodesIds;
    for(int nodeId: currentCycle)
    {
        enteringNodesIds.push_back(nodeId); // Generate moves for the initial cycle
    }
    bool moveFound = false;
    do
    {
        moveFound = false;
        std::vector<NeighbourhoodMove*> movesToRemove;
        generateMoves(sortedMovesList, currentCycle, positionInCycleCache, enteringNodesIds);
        enteringNodesIds.clear();
        for(NeighbourhoodMove* nMove: sortedMovesList)
        {
            if(nMove->isApplicable(currentCycle, positionInCycleCache))
            {
                //std::cout << nMove ->debugInfo() << std::endl;
                nMove->performMove(currentCycle, positionInCycleCache);
                moveFound = true;
                enteringNodesIds = nMove->getEnteringIds();
                break;
            }
            else if(!nMove->shouldBeLeftInLM(currentCycle, positionInCycleCache))
            {
                movesToRemove.push_back(nMove);
            }
        }
        for(NeighbourhoodMove* nMove: movesToRemove)
        {
            sortedMovesList.erase(nMove);
            delete nMove;
        }
        //std::cout << sortedMovesList.size() << " " << calculateCycleCost(currentCycle) << std::endl;
    } while(moveFound);

    for(NeighbourhoodMove* nMove: sortedMovesList)
    {
        delete nMove;
    }

    return currentCycle;
}

void LocalSearchDeltaGenerator::generateMoves(std::set<NeighbourhoodMove*, bool(*)(NeighbourhoodMove*, NeighbourhoodMove*)>& movesList, const std::vector<int>& currentCycle, int* cyclePosCache, const std::vector<int>& enteringNodesIds)
{
    // Inter moves
    std::vector<int> indicesNotInCycle;
    for(int i = 0; i < costDistanceInfo->getNumOfNodes(); i++)
    {
        if(cyclePosCache[i] == -1)
        {
            indicesNotInCycle.push_back(i);
        }
    }
    for(int nodeNotInCycleId: indicesNotInCycle)
    {
        for(int nodeInCycle: enteringNodesIds)
        {
            InterNodeNeighbourhoodMove* nMove = new InterNodeNeighbourhoodMove(nodeInCycle, nodeNotInCycleId);
            nMove->functionDelta = nMove->calculateFunctionDelta(costDistanceInfo, currentCycle, cyclePosCache);
            if(nMove->functionDelta < 0) // Move is improving, it should be added
            {
                addMoveToList(movesList, nMove);
            }
        }
    }
    // Intra moves
    for(int enteringId: enteringNodesIds)
    {
        int enteringIdCyclePos = cyclePosCache[enteringId];
        int enteringIdSuccPos = (enteringIdCyclePos + 1) % currentCycle.size();
        int enteringIdSuccId = currentCycle[enteringIdSuccPos];

        for(int nodeInCycleId: currentCycle)
        {
            int nodeInCyclePosInCycle = cyclePosCache[nodeInCycleId];
            int nodeInCycleSuccPos = (nodeInCyclePosInCycle + 1) % currentCycle.size();
            int nodeInCycleSucc = currentCycle[nodeInCycleSuccPos];

            if(enteringIdSuccPos == nodeInCyclePosInCycle ||
               enteringIdCyclePos == nodeInCycleSuccPos ||
               enteringId == nodeInCycleId) continue;

            IntraNodeChangeEdgeNeighbourhoodMove* nMove = new IntraNodeChangeEdgeNeighbourhoodMove(
                enteringId, nodeInCycleId,
                enteringIdSuccId, nodeInCycleSucc
            );
            IntraNodeChangeEdgeNeighbourhoodMove* nMoveReversed = new IntraNodeChangeEdgeNeighbourhoodMove(
                enteringIdSuccId, nodeInCycleSucc,
                enteringId, nodeInCycleId
            );

            int evaluation = nMove->calculateFunctionDelta(costDistanceInfo, currentCycle, cyclePosCache);
            if(evaluation < 0)
            {
                nMove->functionDelta = evaluation;
                nMoveReversed->functionDelta = evaluation; // This has the same evaluation as normal move
                nMoveReversed->n1cs = nMove->n1cs;
                nMoveReversed->n2cs = nMove->n2cs;
                addMoveToList(movesList, nMoveReversed);
                addMoveToList(movesList, nMove);
            }
        }
    }
}

void LocalSearchDeltaGenerator::addMoveToList(std::set<NeighbourhoodMove*, bool(*)(NeighbourhoodMove*, NeighbourhoodMove*)>& movesList, NeighbourhoodMove* nMove)
{
    movesList.insert(nMove);
}
