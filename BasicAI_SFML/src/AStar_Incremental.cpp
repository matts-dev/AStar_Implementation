#include "AStar_Incremental.h"
#include "WorldGrid.h"
#include "GameInstance.h"
#include <stack>

bool AStar_Incremental::hasNodeBeenVisited(sp<AStarNode> node)
{
	using UMIter = std::unordered_multimap<int, sp<AStarNode> >::iterator;

	bool bAlreadyVisited = false;

	//get all values for key in O(1*)
	int hash = WorldGrid::hashGrid(node->location);
	std::pair<UMIter, UMIter> start_end = memo.equal_range(hash);
	UMIter start = start_end.first;
	UMIter end = start_end.second;

	//hopefully this is implemented to be O(m) where m is the bucket size.
	for(UMIter bucketIter = start; bucketIter != end; ++bucketIter)
	{
		sp<AStarNode> previousNode = bucketIter->second;
		sf::Vector2i& previousVisit = previousNode->location;
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
	for (int step = 0; step < iterationStepsPerTick && !bSearchComplete; ++step)
	{
		doOneIteration();
	}
}

void AStar_Incremental::doOneIteration(sp<AStarNode> startNode /*= nullptr*/)
{
	WorldGrid& worldGrid = GameInstance::getGameInstance().getWorldGrid();
	sp<AStarNode> curNode;

	//pop node off priority queue
	if (priorityQueue.size() > 0 || startNode)
	{
		if (!startNode)
		{
			curNode = priorityQueue.top();
			priorityQueue.pop();
		}
		else
		{
			curNode = startNode;
		}

		generateNewStatesFromNode(curNode, generatedNodes);
		for (sp<AStarNode>& newNode : generatedNodes)
		{
			int distToTarget = getDistanceSquared(newNode->location, cachedTargetPos);
			newNode->distanceToTargetSquared = distToTarget;
			newNode->previousNode = curNode;

			//check after new node is configured
			if (newNode->location == cachedTargetPos)
			{
				bSearchComplete = true;
				cacheSolutionPath(newNode);
				return;
			}
			if (!hasNodeBeenVisited(newNode) && !worldGrid.isPositionOccupied(newNode->location.x, newNode->location.y))
			{
				priorityQueue.push(newNode);

				int hash = WorldGrid::hashGrid(newNode->location);
				memo.emplace(hash, newNode);
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
	memo.clear();
	solutionPath.clear();
	while (!priorityQueue.empty()) priorityQueue.pop();

	bSearchComplete = false;
	cachedTargetPos = targetPos;

	sp<AStarNode> startNode = new_sp<AStarNode>();
	startNode->location = currentPos;
	startNode->locBeforeThis = currentPos; //has to start from somerwhere for visualization
	doOneIteration(startNode);

	doXIterationSteps();

	return bSearchComplete;
}

const std::vector<sp<AStarNode>>& AStar_Incremental::getSequence()
{
	//should probably make a deep copy for better encapsulation, but leaving for now.
	return solutionPath;
}

void drawPathLine(sf::RenderWindow& window, sf::Color color, AStarNode& node)
{
	sf::Vertex lineBuffer[] = {
		sf::Vertex(sf::Vector2f(0, 0)),
		sf::Vertex(sf::Vector2f(100, 100))
	};

	lineBuffer[0] = sf::Vector2f((float)node.locBeforeThis.x, (float)node.locBeforeThis.y) + sf::Vector2f(16.0f, 16.0f);
	lineBuffer[0].color = color;
	lineBuffer[1] = sf::Vector2f((float)node.location.x, (float)node.location.y) + sf::Vector2f(16.0f, 16.0f);;
	lineBuffer[1].color = color;
	window.draw(lineBuffer, 2, sf::Lines);
}

void AStar_Incremental::draw(sf::RenderWindow& window)
{
	static std::vector<sp<AStarNode>> storage;
	storage.reserve(100);
	storage.clear();

	//DRAW VISITED PATH
	for (auto& kv_pair : memo)
	{
		//this is probably a very slow iteration because it is going over every single bucket
		sp<AStarNode>& curr = kv_pair.second;
		drawPathLine(window, sf::Color::Blue, *curr);
	}

	//DRAW UPCOMING IN QUEUE
	//std priority queue doesn't expose iterators, must empty queue and then repopulate the queue
	while (!priorityQueue.empty()){
		sp<AStarNode> curr = priorityQueue.top();
		priorityQueue.pop();
		storage.push_back(curr);
		drawPathLine(window, sf::Color::Red, *curr);
	}
	for(size_t i = 0; i < storage.size(); ++i)
	{
		priorityQueue.push(storage[i]);
	}
	storage.clear();
	

	//DRAW SOLUTION PATH
	for (sp<AStarNode>& solutionNode : solutionPath)
	{
		drawPathLine(window, sf::Color::Yellow, *solutionNode);
	}

}

void AddNodeHelper(MoveDir dir, sf::Vector2i offset, sf::Vector2i srcLoc, std::vector<sp<AStarNode>>& outNodes)
{
	sp<AStarNode> newNode = new_sp<AStarNode>();
	newNode->locBeforeThis = srcLoc;
	newNode->directionToGetHere = dir;
	newNode->location = offset + srcLoc;
	outNodes.push_back(newNode);
}

void AStar_Incremental::generateNewStatesFromNode(sp < AStarNode>& srcNode, std::vector<sp<AStarNode>>& generatedNodes)
{
	generatedNodes.clear();
	AddNodeHelper(MoveDir::UP, { 0, -32 }, srcNode->location, generatedNodes);
	AddNodeHelper(MoveDir::DOWN, { 0, 32 }, srcNode->location, generatedNodes);
	AddNodeHelper(MoveDir::LEFT, { -32, 0 }, srcNode->location, generatedNodes);
	AddNodeHelper(MoveDir::RIGHT, { 32, 0 }, srcNode->location, generatedNodes);
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

	MoveDir shouldTakeDirection = nodeIter->directionToGetHere;
	nodeIter->takeDirectionFromHere = MoveDir::NONE;

	//fill stack to reverse list
	while (nodeIter->previousNode)
	{
		reverseStack.push(nodeIter->previousNode);

		nodeIter = nodeIter->previousNode;

		nodeIter->takeDirectionFromHere = shouldTakeDirection;
		shouldTakeDirection = nodeIter->directionToGetHere;
	}

	//pop stack to get reversed order as solution path
	//while (reverseStack.size() != 1) //first node is start node, it shouldn't be part of solution.
	while (!reverseStack.empty())
	{
		solutionPath.push_back(reverseStack.top());
		reverseStack.pop();
	}

}
