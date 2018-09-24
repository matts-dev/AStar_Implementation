#pragma once
#include <SFML/Graphics.hpp>
#include "Utilities/SmartPtrTypedefs.h"
class TextureManager
{
public:
	static const TextureManager & GetManager();
	const sf::Texture& getTextureMap() const { return *texturemap; }
	~TextureManager();

private:
	sp<sf::Texture> texturemap;

	TextureManager();
	TextureManager(const TextureManager& copy) = delete;
	TextureManager(TextureManager&& move) = delete;
	TextureManager& operator=(const TextureManager& copy) = delete;
	TextureManager& operator=(TextureManager&& move) = delete;

};

