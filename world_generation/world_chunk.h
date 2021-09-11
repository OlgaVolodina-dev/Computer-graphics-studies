#ifndef WORLD_CHUNK_H
#define WORLD_CHUNK_H

#include "camera.h"
#include "terrain.h"
#include "trees.h"
class WorldChunk
{
public:
    void Draw();
    void Init(unsigned int numVertices, std::pair<int, int> gridNumber);
    void Update(std::pair<int, int> gridNumber);
    bool PointInTerrain(glm::vec3 &position) {return terrain.PointInTerrain(position); }
    std::pair<int, int> GetGridNumber() {  return terrain.GetGridNumber();}
    void CheckUpdatedData();
private:
    Terrain terrain;
    Trees tree;
};

#endif