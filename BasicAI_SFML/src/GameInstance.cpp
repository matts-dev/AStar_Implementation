#include "GameInstance.h"
#include "Background.h"
#include "TextureManager.h"
#include "InputTracker.h"
#include <chrono>
#include <random>
#include "MoveTypes.h"

void GameInstance::pollInput()
{
	static InputTracker ioTracker;
	ioTracker.updateState();

	if (ioTracker.isKeyJustPressed(sf::Keyboard::Escape))
	{	
		bShutdown = true;
	}

	if (ioTracker.isKeyDown(sf::Keyboard::W))
	{
		player->setNextMoveDirection(MoveDir::UP);
	}
	if (ioTracker.isKeyDown(sf::Keyboard::S))
	{
		player->setNextMoveDirection(MoveDir::DOWN);
	}
	if (ioTracker.isKeyDown(sf::Keyboard::A))
	{
		player->setNextMoveDirection(MoveDir::LEFT);
	}
	if (ioTracker.isKeyDown(sf::Keyboard::D))
	{
		player->setNextMoveDirection(MoveDir::RIGHT);
	}
}

void GameInstance::updateDeltaTime()
{
	static auto last_frame = std::chrono::steady_clock::now();
	using std::chrono::duration_cast;
	using std::chrono::milliseconds;
	using std::chrono::nanoseconds;

	//set up delta time
	auto now = std::chrono::steady_clock::now();
	milliseconds delta_milis = duration_cast<milliseconds>(now - last_frame);
	auto delta_ms = static_cast<float>(delta_milis.count());

	//Let's set up delta_time to be a fraction of a second. This may not be standard practice (it might be), but it allows us to
	//use speeds variables like "move 1200 pixels a second" which kind of gives us a mental idea of what the values we're using
	//So, if a delta_time has a value of 0.5 means half of a second occured since the last frame occured; (0.5 * 1second).
	//Now, we can base all our speeds on pixels per second! 
	deltaTimeSecs = delta_ms / 1000.f;

	//update last frame after processing of tick.
	last_frame = now;
}

void GameInstance::tick()
{
	player->tick(deltaTimeSecs);
	enemy->tick(deltaTimeSecs);
}

void GameInstance::render()
{
	window->clear();
	background->render(*window);
	for (auto& collideable : collidables)
	{
		window->draw(*collideable);
	}
	window->draw(*player);
	window->draw(*enemy);
	window->display();
}


GameInstance::GameInstance()
{
	//becareful about members of classes that rely on GameInstance at construction, otherwise a circular dependency will arise at static initialization time. It will compile but will not give expected runtime behavior.
	window = new_sp<sf::RenderWindow>(sf::VideoMode(width, height), "Basic AI!");
	window->setFramerateLimit(60);
}

GameInstance& GameInstance::getGameInstance()
{
	static GameInstance singleton;
	return singleton;
}

void GameInstance::postStaticInitialize()
{
	//this is a place for game objects that use gameinstance for initialization (such as getting the size of the window)
	if (!bPostStaticInitializeFinished)
	{
		background = new_sp<Background>();

		const TextureManager& manager = TextureManager::GetManager();
		const sf::Texture& textureMap = manager.getTextureMap();
		player = new_sp<Mob>();
		player->setTexture(textureMap);
		player->setTextureRect(sf::IntRect(1760, 352, 32, 32));

		//position player at a multiple of 32 using integer division tricks
		float playerX = float(32*((width / 2) / 32));
		float playerY = float(32*((height / 2) / 32));
		player->setPosition(playerX, playerY);
		player->setSpeed(500);

		enemy = new_sp<Mob>();
		enemy->setTexture(textureMap);
		enemy->setTextureRect(sf::IntRect(1760, 384, 32, 32));

		std::random_device rng;
		std::seed_seq seed{ rng(), rng(), rng(), rng(), rng(), rng(), rng(), rng() }; 
		//std::seed_seq seed{ 1, 11, 7, 8 , 19, 12, 101, 7997};
		std::mt19937 eng(seed);                   
		int boundary = 10;
		std::uniform_int_distribution<int> uniform_int(-boundary, boundary);


		for(int i = 0; i < 20; ++i)
		{
			collidables.push_back(new_sp<Mob>());
			collidables[i]->setTexture(textureMap);
			collidables[i]->setTextureRect(sf::IntRect(1888, 384, 32, 32));
			//collidables[i]->setPosition((float)(positions[i].x), (float)(positions[i].y));
			float x;
			float y;
			do{
				x = (float)(uniform_int(eng) * 32) + playerX;
				y = (float)(uniform_int(eng) * 32) + playerY;
			} while (getWorldGrid().isPositionOccupied(lround(x), lround(y)));
			std::cout << "hole at " << x << " " << y << std::endl;
			collidables[i]->setPosition(x, y);
		}


		bPostStaticInitializeFinished = true;
	}
}

WorldGrid& GameInstance::getWorldGrid()
{
	return worldGrid;
}

void GameInstance::gameLoop()
{
	while (window->isOpen() && !bShutdown)
	{
		updateDeltaTime();
		
		sf::Event event;
		while (window->pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window->close();
		}
		pollInput();

		tick();
		render();

	}
}
