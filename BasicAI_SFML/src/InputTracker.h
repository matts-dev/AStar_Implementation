#pragma once

#include <set>
#include <stack>
#include <SFML/Graphics.hpp>

class InputTracker final
{
public:
	InputTracker();
	~InputTracker();

	bool isKeyJustPressed(sf::Keyboard::Key key);
	bool isKeyDown(sf::Keyboard::Key key);
	void updateState();

private:
	std::set<sf::Keyboard::Key> keysCurrentlyPressed;

};

