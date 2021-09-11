#ifndef WORLD_HANDLER_H
#define WORLD_HANDLER_H

#include "terrain/terrainHandler.h"
#include "entities/trees.h"
#include "camera.h"
class WorldHandler
{
public:
    void Init();
    void Draw();
    void Process(Camera &);
    TerrainHandler& GetTerrainHander() { return terrainHandler; }
private:
    TerrainHandler terrainHandler;
    Trees tree;
};

#endif