#pragma once
#include "Mob.h"
#include "AStar_Incremental.h"

class AI_Mob : public Mob	
{
public:
	virtual void tick(float DeltaTime) override;

	void setTarget(sp<Mob>& newTarget);
	void setTarget(wp<Mob>& newTarget);

	virtual void OnNewPositionArrived(sf::Vector2f newPosition) override;

protected:

private:
	wp<Mob> target;
	AStar_Incremental astarComponent;
	void moveToNextAStarInSequence();
};