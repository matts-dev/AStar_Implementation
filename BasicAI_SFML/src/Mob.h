#pragma once

#include "Utilities/SmartPtrTypedefs.h"
#include <SFML/Graphics.hpp>
#include "MoveTypes.h"

struct MoveData
{
	sf::Vector2f start;
	sf::Vector2f finishLoc;
	float distance;
	float progress;
};

class Mob : public sf::Sprite
{
public:

protected:
	bool bIsMoving = false;

private:
	MoveDir NextMovement = MoveDir::NONE;
	MoveData moveData;
	float MoveSpeedPixelsPerSec = 128; 

	void StartMovement();
	void updatePositions(float deltaSeconds);
public:
	virtual void tick(float DeltaTime);
	void setNextMoveDirection(MoveDir Direction);

	void setPosition(float x, float y);
	virtual void OnNewPositionArrived(sf::Vector2f newPosition) {}
	sf::Vector2i getGridLocation();


	Mob();
	~Mob();
	void setSpeed(float newPixelsPerSecSpeed);
};

