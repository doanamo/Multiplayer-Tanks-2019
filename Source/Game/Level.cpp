#include "Precompiled.hpp"
#include "Game/Level.hpp"
#include "Game/World.hpp"
#include "System/AssetManager.h"

Level::Level()
{
    // Load map tiles textures.
    m_textureWater = g_assetManager->loadTexture("Data/Sprites/water_base.png");
    m_textureGrass = g_assetManager->loadTexture("Data/Sprites/grass_base.png");
    m_textureBrickWall = g_assetManager->loadTexture("Data/Sprites/brickWall_base.png");
    m_textureMissingTexture = g_assetManager->loadTexture("Data/Sprites/missingTexture_base.png");

    // Load level TODO: Implement loading from file.
    for (int x = 0; x < mapWidth; x++)
    {
        for (int y = 0; y < mapHeight; y++)
        {
            // some random stuff to fill a map
            if (x % 4 == 0) mapElements[x][y] = 1;
            else if (y % 5 == 0) mapElements[x][y] = 2;
            else if (x == 6 && y == 6) mapElements[x][y] = 666; // missing txt test 
            else mapElements[x][y] = 0;
        }
    }
}

Level::~Level()
{}

void Level::onUpdate(float timeDelta)
{
    Object::onUpdate(timeDelta);
}

void Level::onDraw(float updateAlpha)
{
    Object::onDraw(updateAlpha);

    // Draw map WIP.
    for (int x = 0; x < mapWidth; x++)
    {
        for (int y = 0; y < mapHeight; y++)
        {
            sf::RectangleShape mapTileSprite;

            mapTileSprite.setSize(sf::Vector2f(1.f, 1.f));
            mapTileSprite.setOrigin(mapTileSprite.getSize() / 2.0f);
            mapTileSprite.setPosition(x-((int)(Level::mapWidth/2)), y-((int)(Level::mapHeight/2)));
            
            switch (mapElements[x][y])
            {
            case 0:
                mapTileSprite.setTexture(m_textureWater.get());
                break;
            case 1:
                mapTileSprite.setTexture(m_textureGrass.get());
                break;
            case 2:
                mapTileSprite.setTexture(m_textureBrickWall.get());
                break;
            default:
                mapTileSprite.setTexture(m_textureMissingTexture.get());
                break;
            }
            
            g_render->draw(mapTileSprite);
        }
    }

}
