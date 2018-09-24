#include<cmath>
#include "Mob.h"
#include "GameInstance.h"
#include "WorldGrid.h"



void Mob::StartMovement()
{
	if (NextMovement == MoveDir::NONE) return;

	sf::Vector2f startPos = getPosition();
	sf::Vector2f finishPos;
	float moveAmount = 32;
	switch (NextMovement)
	{
		//below cases use int rounding to stop any potential float drift
		case MoveDir::DOWN:
			finishPos.x = (float)(lround(startPos.x));
			finishPos.y = (float)(lround(startPos.y + moveAmount));
			break;				 
		case MoveDir::UP:
			finishPos.x = (float)(lround(startPos.x));
			finishPos.y = (float)(lround(startPos.y - moveAmount));
			break;				 
		case MoveDir::LEFT:
			finishPos.x = (float)(lround(startPos.x - moveAmount));
			finishPos.y = (float)(lround(startPos.y));
			break;
		case MoveDir::RIGHT:
			finishPos.x = (float)(lround(startPos.x + moveAmount));
			finishPos.y = (float)(lround(startPos.y));
			break;
	}

	GameInstance& game = GameInstance::getGameInstance();
	WorldGrid& world = game.getWorldGrid();


	if (world.claimPosition(lround(finishPos.x), lround(finishPos.y)))
	{
		world.leavePosition(lround(startPos.x), lround(startPos.y));
		moveData.distance = moveAmount;
		moveData.start = startPos;
		moveData.finishLoc = finishPos;
		moveData.progress = 0;
		bIsMoving = true;
		NextMovement = MoveDir::NONE;
	}
	else
	{
		NextMovement = MoveDir::NONE;
	}

}

void Mob::updatePositions(float deltaSeconds)
{
	float newProgress = deltaSeconds * MoveSpeedPixelsPerSec;
	moveData.progress += newProgress;
	if (moveData.progress >= moveData.distance)
	{
		bIsMoving = false;
		sf::Sprite::setPosition(moveData.finishLoc);
		OnNewPositionArrived(moveData.finishLoc);
	} 
	else
	{
		//lerp
		sf::Vector2f moveDirection = moveData.finishLoc - moveData.start;
		float lerpFraction = moveData.progress / moveData.distance;
		sf::Vector2f newPostion = moveData.start + sf::Vector2f{moveDirection.x * lerpFraction, moveDirection.y * lerpFraction};
		sf::Sprite::setPosition(newPostion);
	}
}

void Mob::tick(float deltaTimeSecs)
{
	if (bIsMoving)
	{
		updatePositions(deltaTimeSecs);
	}
	else if (NextMovement != MoveDir::NONE)
	{
		StartMovement();
	}

}

void Mob::setNextMoveDirection(MoveDir Direction)
{	
	if(!bIsMoving)
	{
		NextMovement = Direction;
	}
}

void Mob::setPosition(float x, float y)
{
	WorldGrid& world = GameInstance::getGameInstance().getWorldGrid();
	if (world.claimPosition(lround(x), lround(y)))
	{
		sf::Vector2f thisPos = getPosition();
		world.leavePosition(lround(thisPos.x), lround(thisPos.y));
		sf::Sprite::setPosition(x, y);
	}
}

sf::Vector2i Mob::getGridLocation()
{
	sf::Vector2i loc;
	if (bIsMoving)
	{
		loc.x = Utils::floatToGridLoc(moveData.finishLoc.x); 
		loc.y = Utils::floatToGridLoc(moveData.finishLoc.y);
	}
	else
	{
		const sf::Vector2f& position = getPosition();
		loc.x = Utils::floatToGridLoc(position.x);
		loc.y = Utils::floatToGridLoc(position.y);
	}

	return loc;
}

Mob::Mob()
{
}


Mob::~Mob()
{
}

void Mob::setSpeed(float newPixelsPerSecSpeed)
{
	MoveSpeedPixelsPerSec = newPixelsPerSecSpeed;
}
