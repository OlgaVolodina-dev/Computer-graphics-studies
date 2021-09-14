#include "terrainHandler.h"
#include "Log.h"
void TerrainHandler::Init()
{
    int k = 0;
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            int x = gridCenter_.first + i - 1;
            int z = gridCenter_.second + j - 1;
            terrains_[k++].Init(32U, {x, z});
        }
    }
}

std::pair<int, int> TerrainHandler::FindNewGridCenter(glm::vec3 &position, WorldChunk*& newCenterTerrain)
{
    for (auto &terrain: terrains_) {
        if (terrain.PointInTerrain(position)) {
            newCenterTerrain = &terrain;
            return terrain.GetGridNumber();
        }
    }
    Log::Error( "Grid center  not found %f %f %f", position.x, position.y, position.z);
    abort();
    return {0, 0};
}

bool TerrainHandler::TerrainExists(int x, int z, std::vector<bool> &remain)
{
    for (int i = 0; i < terrains_.size(); ++i) {
        gridPair_t n = terrains_[i].GetGridNumber();
        if (n.first == x && n.second == z) {
            remain[i] = true;
            return true;
        }
    }
    return false;
}

void TerrainHandler::GetFreeTerrainsAnsNums(std::vector<WorldChunk *> &freeTerrains, std::vector<gridPair_t> &freenums)
{
    std::vector<bool> rt(9, false);
    for (int i = gridCenter_.first - 1; i <= gridCenter_.first + 1; ++i) {
        for (int j = gridCenter_.second - 1; j <= gridCenter_.second + 1; ++j) {
            if (!TerrainExists(i, j, rt)) {
                freenums.push_back({i, j});
            }
        }
    }

    for (int i = 0; i < terrains_.size(); ++i) {
        if (rt[i] == false){
            freeTerrains.push_back(&terrains_[i]);
        }
    }
}

void TerrainHandler::Process(Camera & camera)
{
    glm::vec3 cameraPosition = camera.GetPosition();
    for (auto &terrain : terrains_) {
        terrain.CheckUpdatedData();
    }
    for (auto &terrain : terrains_) {
        if (terrain.GetGridNumber().first == gridCenter_.first &&
            terrain.GetGridNumber().second == gridCenter_.second) {
                if (terrain.PointInTerrain(cameraPosition)) {
                    return;
                }
            }
    }
    WorldChunk * newTerrainCenter = nullptr;
    gridCenter_ = FindNewGridCenter(cameraPosition, newTerrainCenter);
    if (newTerrainCenter == nullptr) {
        std::cout << "new terrain center in null" << std::endl;
        abort();
    }
    //std::swap(terrains_[1], *newTerrainCenter);
    std::vector<WorldChunk *> freeTerrains;
    std::vector<gridPair_t> freeNums;
    GetFreeTerrainsAnsNums(freeTerrains, freeNums);
    if (freeTerrains.size() != freeNums.size()) {
        std::cout << "Free terrains != free gridNums " <<freeNums.size() << " " << freeTerrains.size() << std::endl;
        abort();
    }
    for (int i = 0; i < freeTerrains.size(); ++i) {
        freeTerrains[i]->Update(freeNums[i]);
    }
}

void TerrainHandler::Draw() {
    for (auto &terrain : terrains_) {
            terrain.Draw();
    }
}