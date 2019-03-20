#pragma once

#include "Precompiled.hpp"

class Level : public Serializable
{
public:
    Level();
    ~Level();

    bool initialize();
    void tick(float timeDelta);
    void draw(float timeAlpha);

protected:
    // Serialization methods.
    bool onSerialize(MemoryBuffer& buffer) override;
    bool onDeserialize(MemoryBuffer& buffer) override;

private:
    // Constants.
    static const int mapWidth = 10;
    static const int mapHeight = 10;

private:
    // Map elements.
    int mapElements[mapWidth][mapHeight];

    // Map sprites textures.
    std::shared_ptr<sf::Texture> m_textureWater;
    std::shared_ptr<sf::Texture> m_textureGrass;
    std::shared_ptr<sf::Texture> m_textureBrickWall;
    std::shared_ptr<sf::Texture> m_textureMissingTexture;
};
