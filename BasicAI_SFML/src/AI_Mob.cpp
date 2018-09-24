#include "AI_Mob.h"

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
}

void AI_Mob::setTarget(wp<Mob>& newTarget)
{
	target = newTarget;
}

void AI_Mob::OnNewPositionArrived(sf::Vector2f newPosition)
{
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

void AI_Mob::moveToNextAStarInSequence()
{
	const std::vector< sp<AStarNode> >& moveSequence = astarComponent.getSequence();
	if (moveSequence.size() > 0)
	{
		Mob::setNextMoveDirection(moveSequence[0]->dir);
	}
}

