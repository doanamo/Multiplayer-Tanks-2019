#include "Precompiled.hpp"
#include "SnapshotSaveLoad.hpp"
#include "Game/GameInstance.hpp"
#include "Game/World/World.hpp"

SnapshotSaveLoad::SnapshotSaveLoad(GameInstance* gameInstance) :
    m_gameInstance(gameInstance)
{
}

SnapshotSaveLoad::~SnapshotSaveLoad()
{
}

bool SnapshotSaveLoad::save(std::string filename)
{
    // Check if we have valid game instance reference.
    VERIFY(m_gameInstance);

    // Save game instance to memory stream.
    MemoryStream memoryStream;
    if(!this->save(memoryStream))
    {
        LOG_ERROR("Could not save game instance into memory stream!", filename.c_str());
        return false;
    }

    // Write buffer to disc.
    std::ofstream file(filename, std::ios::binary | std::ios::trunc);

    if(!file.is_open())
    {
        LOG_ERROR("Could not open \"%s\" file for writing!", filename.c_str());
        return false;
    }

    file.write(memoryStream.data(), memoryStream.size());
    file.close();

    // Success!
    LOG_INFO("Saved game instance into \"%s\" file.", filename.c_str());
    return false;
}

bool SnapshotSaveLoad::load(std::string filename)
{
    // Check if we have valid game instance reference.
    VERIFY(m_gameInstance);

    // Open and read file into memory buffer.
    std::ifstream file(filename, std::ios::binary | std::ios::ate);

    if(!file.is_open())
    {
        LOG_ERROR("Could not open \"%s\" file for reading!", filename.c_str());
        return false;
    }

    std::size_t size = (std::size_t)file.tellg();
    file.seekg(0, std::ios::beg);

    // Read snapshot data into memory stream.
    MemoryStream memoryStream;
    memoryStream.resize(size);

    if(!file.read(memoryStream.data(), size))
    {
        LOG_ERROR("Could not read \"%s\" file!", filename.c_str());
        return false;
    }

    file.close();

    // Load game instance from memory stream.
    if(!this->load(memoryStream))
    {
        LOG_ERROR("Could not load game instance from \"%s\" file!", filename.c_str());
        return false;
    }

    // Success!
    LOG_INFO("Loaded game instance from \"%s\" file.", filename.c_str());
    return true;
}

bool SnapshotSaveLoad::save(MemoryStream& memoryStream)
{
    // Check if we have valid game instance reference.
    VERIFY(m_gameInstance);

    // Prepare world for saving.
    m_gameInstance->getWorld().flushObjects();

    // Serialize game instance to memory stream.
    if(!serialize(memoryStream, *m_gameInstance))
    {
        LOG_ERROR("Could not serialize game instance!");
        return false;
    }

    // Success!
    return true;
}

bool SnapshotSaveLoad::load(MemoryStream& memoryStream)
{
    // Check if we have valid game instance reference.
    VERIFY(m_gameInstance);

    // Verify that game instance is fresh.
    // Here we do this by checking if world is empty.
    VERIFY(m_gameInstance->getWorld().getObjectCount() == 0, "World appears to have already been populated!");

    // Deserialize game instance from memory stream.
    if(!deserialize(memoryStream, m_gameInstance))
    {
        LOG_ERROR("Could not deserialize game instance!");
        return false;
    }

    // Success!
    return true;
}
