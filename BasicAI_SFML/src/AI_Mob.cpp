#include "AI_Mob.h"
#include <stdio.h>

void AI_Mob::tick(float DeltaTime)
{
	astarComponent.tick(DeltaTime);
	if (!bIsMoving)
	{
		moveToNextAStarInSequence();
	}
	Mob::tick(DeltaTime);
}

void AI_Mob::setTarget(sp<Mob>& newTarget)
{
	target = newTarget;
	OnNewPositionArrived(getPosition());
}

void AI_Mob::OnNewPositionArrived(sf::Vector2f newPosition)
{
	if (verbose) printf("AI:newPostionArrived\n");
	if (sp<Mob> targetSP = target.lock())
	{
		const sf::Vector2i& targetPosition = targetSP->getGridLocation();
		const sf::Vector2i currentGrid = { Utils::floatToGridLoc(newPosition.x), Utils::floatToGridLoc(newPosition.y) };

		//iterative astar may not find ta first call and should be given time to complete steps in tick
		//however, it may be a quick find so, if it completes the search immediately, go ahead and update the position
		if (astarComponent.startFind(currentGrid, targetPosition))
		{
			moveToNextAStarInSequence();
		}
	}

}

void AI_Mob::drawAIPathFinding(sf::RenderWindow& window)
{
	astarComponent.draw(window);
}

void AI_Mob::moveToNextAStarInSequence()
{

	const std::vector< sp<AStarNode> >& moveSequence = astarComponent.getSequence();
	if (moveSequence.size() > 0)
	{
		MoveDir direction = moveSequence[0]->takeDirectionFromHere;
		Mob::setNextMoveDirection(direction);
		if (verbose) printf("AI:moveToNext %s\n", Utils::toString(direction));
	}

	if (verbose)
	{
		for (size_t i = 0; i < moveSequence.size(); ++i)
		{
			printf("AI:moveToNext %s\n", Utils::toString(moveSequence[i]->takeDirectionFromHere));
		}
	}
}

