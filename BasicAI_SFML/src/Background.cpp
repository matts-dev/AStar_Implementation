#include "Background.h"
#include "TextureManager.h"


Background::Background()
{
	centerSprite = new_sp<sf::Sprite>();

	
	const TextureManager& manager = TextureManager::GetManager();
	const sf::Texture& textureMap = manager.getTextureMap();

	centerSprite->setTexture(textureMap);
	centerSprite->setTextureRect(sf::IntRect(800, 288, 32, 32));

	GameInstance& game = GameInstance::getGameInstance();
	centerSprite->setPosition(game.width / 2.0f, game.height / 2.0f);

}

Background::~Background()
{

}

void Background::render(sf::RenderWindow& window) const
{
	using namespace sf;

	sf::Vector2u screenSize = window.getSize();
	const sf::IntRect& spriteSize = centerSprite->getTextureRect();
	const sf::Vector2f centerPosition = centerSprite->getPosition();

	int offscreenNudge = 4;
	int numColumns = (screenSize.x / spriteSize.width) + offscreenNudge;
	int numRows = (screenSize.y / spriteSize.height) + offscreenNudge;

	float startX = centerPosition.x - ((numColumns / 2) *spriteSize.width);
	float startY = centerPosition.y - ((numRows / 2) *spriteSize.height);
	centerSprite->setPosition(startX, startY);

	for (int cols = 0; cols < numColumns; ++cols)
	{
		for (int rows = 0; rows < numRows; ++rows)
		{
			centerSprite->setPosition(startX + cols * spriteSize.width, startY + rows * spriteSize.height);
			window.draw(*centerSprite);
		}
	}

	centerSprite->setPosition(centerPosition);
}
