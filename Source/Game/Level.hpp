#pragma once

#include "Precompiled.hpp"

class Level
{
public:
    Level();
    ~Level();

    bool initialize();
    void update(float timeDelta);
    void draw(float updateAlpha);

private:
    // Constants.
    static const int mapWidth = 2;
    static const int mapHeight = 2;

private:
    // Map elements.
    int mapElements[mapWidth][mapHeight];

    // Map sprites textures.
    std::shared_ptr<sf::Texture> m_textureWater;
    std::shared_ptr<sf::Texture> m_textureGrass;
    std::shared_ptr<sf::Texture> m_textureBrickWall;
    std::shared_ptr<sf::Texture> m_textureMissingTexture;
};
