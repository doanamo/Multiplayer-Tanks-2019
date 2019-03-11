#pragma once

#include "Precompiled.hpp"
#include "Game/Object.hpp"

class Level : public Object
{
    TYPE_DECLARE(Level, Object);

public:
    Level();
    ~Level();

    void onUpdate(float timeDelta) override;
    void onDraw(float updateAlpha) override;

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