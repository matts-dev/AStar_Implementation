#pragma once

#include <iostream>
#include "Utilities/SmartPtrTypedefs.h"
#include <SFML/Graphics.hpp>
//#include "Mob.h"
#include "WorldGrid.h"

class Mob;
class AI_Mob;
class Background;

class GameInstance
{
public: //fields
	const int width = 800;
	const int height = 600;

private: //fields
	sp<sf::RenderWindow> window;

	sp<Mob> player;
	sp<AI_Mob> enemy;
	sp<Background> background;
	std::vector<sp<Mob>> collidables;
	WorldGrid worldGrid;

	bool bShutdown = false;
	bool bPostStaticInitializeFinished = false;
	float deltaTimeSecs = 0;

private: //member functions
	void pollInput();
	void updateDeltaTime();
	void tick();
	void render();

	//construction
	GameInstance();
	GameInstance(const GameInstance& copy) = delete;
	GameInstance(GameInstance&& move) = delete;
	GameInstance& operator=(const GameInstance& copy) = delete;
	GameInstance& operator=(GameInstance&& move) = delete;


public:
	void gameLoop();
	static GameInstance& getGameInstance();
	void postStaticInitialize();
	WorldGrid& getWorldGrid();

};