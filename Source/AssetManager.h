#pragma once

#include <map>

#include <SFML/Graphics.hpp>

class AssetManager
{
public:
	AssetManager() {};
	virtual ~AssetManager() {};

	void loadTexture(std::string textureName, std::string pathAndFilename);
	sf::Texture& getTexture(std::string textureName);

	void loadFont(std::string fontName, std::string pathAndFilename);
	sf::Font& getFont(std::string fontName);

private:
	std::map<std::string, sf::Texture> _textures;
	std::map<std::string, sf::Font> _fonts;
};