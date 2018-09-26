#pragma once
#include<SFML/Graphics.hpp>
#include "Mob.h"
#include "AStar_Incremental.h"

class AI_Mob : public Mob	
{
public:
	virtual void tick(float DeltaTime) override;

	void setTarget(sp<Mob>& newTarget);

	virtual void OnNewPositionArrived(sf::Vector2f newPosition) override;

	void drawAIPathFinding(sf::RenderWindow& window);
protected:
private:
	bool verbose = true;
	wp<Mob> target;
	AStar_Incremental astarComponent;
	void moveToNextAStarInSequence();
};