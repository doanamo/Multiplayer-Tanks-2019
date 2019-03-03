#include "Precompiled.hpp"
#include "AssetManager.h"

void AssetManager::loadTexture(std::string textureName, std::string pathAndFilename)
{
	sf::Texture texture;
	if (texture.loadFromFile(pathAndFilename))
	{
		this->_textures[textureName] = texture;
	}
}

sf::Texture & AssetManager::getTexture(std::string textureName)
{
	return this->_textures.at(textureName);
}

void AssetManager::loadFont(std::string fontName, std::string pathAndFilename)
{
	sf::Font font;
	if (font.loadFromFile(pathAndFilename))
	{
		this->_fonts[fontName] = font;
	}
}

sf::Font & AssetManager::getFont(std::string fontName)
{
	return this->_fonts.at(fontName);
}
