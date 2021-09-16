#ifndef WORLD_CHUNK_H
#define WORLD_CHUNK_H

#include "camera.h"
#include "terrain.h"
#include "trees.h"
#include <thread>
#include <atomic>
#include "grass.h"

#include "world_chunk_info.h"

class WorldChunk
{
public:
    void Draw();
    void Init(unsigned int numVertices, std::pair<int, int> gridNumber);
    void Update(std::pair<int, int> gridNumber);
    void CheckUpdatedData();
    bool PointInTerrain(glm::vec3 &position);
    std::pair<int, int> GetGridNumber() { return worldChunkInfo.gridNumber_; }

private:
    void GenerateBiomeMap();
    void GenerateHeightMap();
    void UpdateInThread(std::pair<int, int> gridNumber);
    std::atomic<bool> updateDone{false};
    std::thread t;

    Terrain terrain;
    Trees tree;
    Grass grass;

    WorldChunkInfo worldChunkInfo{};

};

#endif