#include "Precompiled.hpp"
#include "AssetManager.h"

AssetManager::AssetManager()
{

}

AssetManager::~AssetManager()
{
    // Clear resource maps.
    m_textures.clear();
    m_fonts.clear();
}

bool AssetManager::initialize()
{
    // Initialize default resources.
    m_defaultTexture = std::make_shared<sf::Texture>();
    m_defaultFont = std::make_shared<sf::Font>();

    return true;
}

void AssetManager::releaseUnused()
{
    // Release unused textures.
    for(auto it = m_textures.begin(); it != m_textures.end();)
    {
        if(it->second.use_count() <= 1)
        {
            m_textures.erase(it++);
            std::cout << "Released texture: " << it->first << std::endl;
        }
        else
        {
            ++it;
        }
    }

    // Release unused fonts.
    for(auto it = m_fonts.begin(); it != m_fonts.end();)
    {
        if(it->second.use_count() <= 1)
        {
            m_fonts.erase(it++);
            std::cout << "Released font: " << it->first << std::endl;
        }
        else
        {
            ++it;
        }
    }
}

std::shared_ptr<sf::Texture> AssetManager::loadTexture(std::string assetPath)
{
    // Attempt to find and return texture if already loaded.
    auto it = m_textures.find(assetPath);
    if(it != m_textures.end())
        return it->second;

    // Load texture resource.
    std::shared_ptr<sf::Texture> texture = std::make_shared<sf::Texture>();

    if(!texture->loadFromFile(assetPath))
    {
        std::cout << "Failed to load texture: " << assetPath << std::endl;
        texture = m_defaultTexture;
    }
    else
    {
        std::cout << "Loaded texture: " << assetPath << std::endl;
    }
 
    // Add texture to map and return it.
    auto result = m_textures.emplace(assetPath, texture);
    return result.first->second;
}

std::shared_ptr<sf::Font> AssetManager::loadFont(std::string assetPath)
{
    // Attempt to find and return font if already loaded.
    auto it = m_fonts.find(assetPath);
    if(it != m_fonts.end())
        return it->second;

    // Load font resource.
    std::shared_ptr<sf::Font> font = std::make_shared<sf::Font>();

    if(!font->loadFromFile(assetPath))
    {
        std::cout << "Failed to load font: " << assetPath << std::endl;
        font = m_defaultFont;
    }
    else
    {
        std::cout << "Loaded font: " << assetPath << std::endl;
    }

    // Add font to map and return it.
    auto result = m_fonts.emplace(assetPath, font);
    return result.first->second;
}
