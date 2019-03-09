#pragma once

#include "Precompiled.hpp"

class AssetManager
{
public:
    AssetManager();
    ~AssetManager();

    bool initialize();
    void releaseUnused();

    std::shared_ptr<sf::Texture> loadTexture(std::string assetPath);
    std::shared_ptr<sf::Font> loadFont(std::string assetPath);

private:
    std::shared_ptr<sf::Texture> m_defaultTexture;
    std::shared_ptr<sf::Font> m_defaultFont;

    std::map<std::string, std::shared_ptr<sf::Texture>> m_textures;
    std::map<std::string, std::shared_ptr<sf::Font>> m_fonts;
};
