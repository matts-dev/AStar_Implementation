#include "InputTracker.h"



InputTracker::InputTracker()
{
}


InputTracker::~InputTracker()
{
}

bool InputTracker::isKeyJustPressed(sf::Keyboard::Key key)
{
	if (sf::Keyboard::isKeyPressed(key))
	{
		if (keysCurrentlyPressed.find(key) == keysCurrentlyPressed.end())
		{
			keysCurrentlyPressed.insert(key);
			return true;
		}
	}

	return false;
}

void InputTracker::updateState()
{
	std::stack<sf::Keyboard::Key> keysToRemove;

	//find all keys that are no longer pressed but still in the container
	for (const auto& key : keysCurrentlyPressed)
	{
		if (sf::Keyboard::isKeyPressed(key))
		{
			//avoid removing from a container we're iterating over
			keysToRemove.push(key);
		}
	}

	//remove all the keys that were not pressed
	while (!keysToRemove.empty())
	{
		sf::Keyboard::Key removeKey = keysToRemove.top();
		keysCurrentlyPressed.erase(removeKey);
		keysToRemove.pop();
	}
}

bool InputTracker::isKeyDown(sf::Keyboard::Key key)
{
	if (sf::Keyboard::isKeyPressed(key))
	{
		//don't insert into pressed keys
		return true;
	}

	return false;
}
