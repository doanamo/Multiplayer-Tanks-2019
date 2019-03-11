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

    // Draw map TEMP HACK.
    sf::RectangleShape mapTileSprite;

    mapTileSprite.setSize(sf::Vector2f(1.f, 1.f));
    mapTileSprite.setOrigin(mapTileSprite.getSize() / 2.0f);
    mapTileSprite.setPosition(0, 0);
    mapTileSprite.setTexture(m_textureWater.get());

    sf::RectangleShape mapTileSprite2;

    mapTileSprite2.setSize(sf::Vector2f(1.f, 1.f));
    mapTileSprite2.setOrigin(mapTileSprite2.getSize() / 2.0f);
    mapTileSprite2.setPosition(1, 1);
    mapTileSprite2.setTexture(m_textureBrickWall.get());

    sf::RectangleShape mapTileSprite3;

    mapTileSprite3.setSize(sf::Vector2f(1.f, 1.f));
    mapTileSprite3.setOrigin(mapTileSprite3.getSize() / 2.0f);
    mapTileSprite3.setPosition(2, 2);
    mapTileSprite3.setTexture(m_textureGrass.get());

    g_render->draw(mapTileSprite);
    g_render->draw(mapTileSprite2);
    g_render->draw(mapTileSprite3);
}