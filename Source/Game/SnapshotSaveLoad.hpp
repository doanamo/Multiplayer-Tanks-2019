#pragma once

#include "Precompiled.hpp"

class GameInstance;

class SnapshotSaveLoad
{
public:
    SnapshotSaveLoad(GameInstance* gameInstance);
    ~SnapshotSaveLoad();

    bool save(std::string filename);
    bool load(std::string filename);

    bool save(MemoryStream& memoryStream);
    bool load(MemoryStream& memoryStream);

private:
    GameInstance* m_gameInstance;
};
