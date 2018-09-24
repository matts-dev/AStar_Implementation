#include <SFML/Graphics.hpp>
#include <iostream>
#include "Utilities/SmartPtrTypedefs.h"
#include "GameInstance.h"


int main()
{
	GameInstance& game = GameInstance::getGameInstance();
	game.postStaticInitialize();
	game.gameLoop();

	return 0;
}