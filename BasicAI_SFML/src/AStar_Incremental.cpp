#include "AStar_Incremental.h"
#include "WorldGrid.h"
#include "GameInstance.h"
#include <stack>

bool AStar_Incremental::hasNodeBeenVisited(sp<AStarNode> node)
{
	using UMIter = std::unordered_multimap<int, sf::Vector2i>::iterator;

	bool bAlreadyVisited = false;

	//get all values for key in O(1*)
	int hash = WorldGrid::hashGrid(node->location);
	std::pair<UMIter, UMIter> start_end = memo.equal_range(hash);
	UMIter start = start_end.first;
	UMIter end = start_end.second;

	//hopefully this is implemented to be O(m) where m is the bucket size.
	for(UMIter bucketIter = start; bucketIter != end; ++bucketIter)
	{
		sf::Vector2i& previousVisit = bucketIter->second;
		if (previousVisit == node->location)
		{
			bAlreadyVisited = true;
			break;
		}
	}

	return bAlreadyVisited;
}

void AStar_Incremental::doXIterationSteps()
{
	for (int step = 0; step < iterationStepsPerTick; ++step)
	{
		doOneIteration();
	}
}

void AStar_Incremental::doOneIteration()
{
	WorldGrid& worldGrid = GameInstance::getGameInstance().getWorldGrid();
	//pop node off priority queue
	if (priorityQueue.size() > 0)
	{
		sp<AStarNode> curNode = priorityQueue.top();
		priorityQueue.pop();

		generateNewNodesFromNode(curNode, generatedNodes);
		for (sp<AStarNode>& newNode : generatedNodes)
		{
			if (newNode->location == cachedTargetPos)
			{
				bSearchComplete = true;
				cacheSolutionPath(newNode);
				return;
			}
			if (!hasNodeBeenVisited(newNode) && !worldGrid.isPositionOccupied(newNode->location.x, newNode->location.y))
			{
				int distToTarget = getDistanceSquared(newNode->location, cachedTargetPos);
				newNode->distanceToTargetSquared = distToTarget;
				newNode->previousNode = curNode;
				priorityQueue.push(newNode);
			}
		}
	}
}

AStar_Incremental::AStar_Incremental()
{
	generatedNodes.reserve(8);
	solutionPath.reserve(8);
}

void AStar_Incremental::tick(float DeltaTimeSecs)
{
	if (!bSearchComplete)
	{
		doXIterationSteps();
	}
}

bool AStar_Incremental::startFind(const sf::Vector2i currentPos, const sf::Vector2i& targetPos)
{
	bSearchComplete = false;
	cachedTargetPos = targetPos;

	doXIterationSteps();

	return bSearchComplete;
}

const std::vector<sp<AStarNode>>& AStar_Incremental::getSequence()
{
	//should probably make a deep copy for better encapsulation, but leaving for now.
	return solutionPath;
}


void AddNodeHelper(MoveDir dir, sf::Vector2i offset, sf::Vector2i srcLoc, std::vector<sp<AStarNode>>& outNodes)
{
	sp<AStarNode> newNode = new_sp<AStarNode>();
	newNode->locBeforeThis = srcLoc;
	newNode->dir = dir;
	newNode->location = offset + srcLoc;
	outNodes.push_back(newNode);
}

void AStar_Incremental::generateNewNodesFromNode(sp < AStarNode>& srcNode, std::vector<sp<AStarNode>>& outNodes)
{
	outNodes.clear();
	AddNodeHelper(MoveDir::UP, { 0, -32 }, srcNode->location, outNodes);
	AddNodeHelper(MoveDir::DOWN, { 0, 32 }, srcNode->location, outNodes);
	AddNodeHelper(MoveDir::LEFT, { -32, 0 }, srcNode->location, outNodes);
	AddNodeHelper(MoveDir::RIGHT, { 32, 0 }, srcNode->location, outNodes);
}

int AStar_Incremental::getDistanceSquared(sf::Vector2i fromPnt, sf::Vector2i targetPnt)
{
	sf::Vector2i distVec = targetPnt - fromPnt;

	//wish sf vectors had a dot product method
	return distVec.x * distVec.x + distVec.y * distVec.y;
}



void AStar_Incremental::cacheSolutionPath(sp<AStarNode>& newNode)
{
	solutionPath.clear();

	sp< AStarNode > nodeIter = newNode;
	
	std::stack< sp<AStarNode> > reverseStack;
	reverseStack.push(nodeIter);

	//fill stack to reverse list
	while (nodeIter->previousNode)
	{
		reverseStack.push(nodeIter->previousNode);
		nodeIter = nodeIter->previousNode;
	}

	//pop stack to get reversed order as solution path
	while (!reverseStack.empty())
	{
		solutionPath.push_back(reverseStack.top());
		reverseStack.pop();
	}

}
