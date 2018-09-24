#pragma once
#include<SFML/Graphics.hpp>
#include "MoveTypes.h"
#include <queue>
#include "Utilities/SmartPtrTypedefs.h"
#include <unordered_map>

struct AStarNode
{
	MoveDir dir;
	sf::Vector2i location;
	sf::Vector2i locBeforeThis;
	sp<AStarNode> previousNode;

	//using distance squared conveys same information as distance, but avoids expensive sqrt
	int distanceToTargetSquared; //int because used as key in priorityQueue
};

struct CompareNodes{
	bool operator()(const sp<AStarNode>& a, const sp<AStarNode>& b)	{
		return a->distanceToTargetSquared < b->distanceToTargetSquared;
	}
};

/** ASTAR implementation*/
class AStar_Incremental
{
private: //fields
	std::priority_queue<sp<AStarNode>, std::vector< sp<AStarNode> >, CompareNodes> priorityQueue;
	bool bSearchComplete = true;
	int iterationStepsPerTick = 32;
	std::vector< sp<AStarNode> > solutionPath;

private: //helpers
	std::vector< sp<AStarNode> > generatedNodes;
	std::unordered_multimap< int, sf::Vector2i > memo;
	sf::Vector2i cachedTargetPos;
	bool hasNodeBeenVisited(sp<AStarNode> node);

private: //methods
	void doXIterationSteps();
	void doOneIteration();

public:
	AStar_Incremental();

	void tick(float DeltaTimeSecs);
	bool startFind(const sf::Vector2i currentPos, const sf::Vector2i& targetPos);
	const std::vector<sp<AStarNode>>& getSequence();

private:
	void generateNewNodesFromNode(sp < AStarNode>& srcNode, std::vector<sp<AStarNode>>& generatedNodes);

	int getDistanceSquared(sf::Vector2i fromPnt, sf::Vector2i targetPnt);
	void cacheSolutionPath(sp<AStarNode>& newNode);
};