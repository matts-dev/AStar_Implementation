#pragma once

#include <SFML/Graphics.hpp>
#include "GameInstance.h"

class Background
{
public:
	sp<sf::Sprite> centerSprite;
	int gridSize;

	Background();
	~Background();
	void render(sf::RenderWindow& window) const;

public: //mutators
	void setCenterPosition(const sf::Vector2f position) { centerSprite->setPosition(position); }
	const sf::Vector2f getCeneter(const sf::Vector2f position) { return centerSprite->getPosition(); }
};

