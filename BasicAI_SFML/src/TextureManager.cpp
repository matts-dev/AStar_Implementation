#include "TextureManager.h"
#include <iostream>

const TextureManager & TextureManager::GetManager()
{
	static TextureManager singleton;
	return singleton;
}

TextureManager::~TextureManager()
{

}

TextureManager::TextureManager()
{
	texturemap = new_sp<sf::Texture>();
	if (!texturemap->loadFromFile("Textures/DungeonCrawlSubtexture.png"))
	{
		std::cin.get();
		exit(-1);
	}
}
