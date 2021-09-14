#include "world_chunk.h"
#include <iostream>
#include "terrainGenerator.h"
#include "Log.h"

void WorldChunk::Init(unsigned int numVertices, std::pair<int, int> gridNumber)
{
    auto &size = worldChunkInfo.size;
    worldChunkInfo.numVertices_ = numVertices; 
    worldChunkInfo.xBoundary = {gridNumber.first * size, (gridNumber.first + 1) * size};
    worldChunkInfo.zBoundary = {gridNumber.second * size, (gridNumber.second + 1) * size};
    GenerateBiomeMap();
    GenerateHeightMap();
    terrain.Init(worldChunkInfo);
    tree.Init(worldChunkInfo);
}

void WorldChunk::GenerateBiomeMap()
{
    auto &numVertices = worldChunkInfo.numVertices_;
    auto &size = worldChunkInfo.size;
    if (worldChunkInfo.biomeMap_.empty()) {
        worldChunkInfo.biomeMap_.resize((numVertices + 1) * (numVertices + 1));
    }
    int k = 0;
    for (int i = 0; i <= worldChunkInfo.numVertices_; ++i) {
        for (int j  = 0; j <= worldChunkInfo.numVertices_; ++j) {
            float x = worldChunkInfo.xBoundary.first + float(i) / worldChunkInfo.numVertices_ * size;
            float z = worldChunkInfo.zBoundary.first + float(j) / worldChunkInfo.numVertices_ * size;
            worldChunkInfo.biomeMap_[k++] = static_cast<uint8_t>(TerrainGenerator::GetBiome(x, z));
        }
    }
}

void WorldChunk::GenerateHeightMap()
{
    auto &numVertices = worldChunkInfo.numVertices_;
    auto &xBoundary =  worldChunkInfo.xBoundary;
    auto &zBoundary =  worldChunkInfo.zBoundary;
    auto &size = worldChunkInfo.size;

    if (worldChunkInfo.terrainHeightMap_.empty()) {
        worldChunkInfo.terrainHeightMap_.resize((numVertices + 1) * (numVertices + 1));
    }

    int k = 0;
    for (int i = 0; i <= numVertices; ++i) {
        for (int j  = 0; j <= numVertices; ++j) {
            float x = xBoundary.first + float(i) / numVertices * size;
            float z = zBoundary.first + float(j) / numVertices * size;
            worldChunkInfo.terrainHeightMap_[k++] = static_cast<float>(TerrainGenerator::GetHeight(x, z));
        } 
    }
}

void WorldChunk::UpdateInThread(std::pair<int, int> gridNumber)
{
    GenerateBiomeMap();
    GenerateHeightMap();
    terrain.Update(worldChunkInfo);
    tree.Update(worldChunkInfo);
    updateDone = true;
}

void WorldChunk::Update(std::pair<int, int> gridNumber) {
    t = std::thread(WorldChunk::UpdateInThread, this, gridNumber);
}

void WorldChunk::CheckUpdatedData() {
    if (updateDone) {
        t.join();
        updateDone = false;
        terrain.CheckUpdatedData(worldChunkInfo);
        tree.CheckUpdatedData();
    }
}

void WorldChunk::Draw()
{
    terrain.Draw();
    tree.Draw();
}

bool WorldChunk::PointInTerrain(glm::vec3 &position)
{
    return worldChunkInfo.PointInTerrain(position);
}