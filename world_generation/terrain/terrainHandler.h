#ifndef TERRAIN_HANDLER_H
#define TERRAIN_HANDLER_H

#include "terrain.h"
#include "camera.h"

typedef std::pair<int, int> gridPair_t;

class TerrainHandler
{
public:
    void Process(Camera &camera);
    void Init();
    void Draw();
    void GetVisibleSpace(int &xMin, int &zMin, int &xMax, int &zMax);

private:
    void GetFreeTerrainsAnsNums(std::vector<Terrain *> &, std::vector<gridPair_t> &);
    bool TerrainExists(int, int, std::vector<bool> &remain);
    std::pair<int, int> FindNewGridCenter(glm::vec3 &position);
    std::pair<int, int> gridCenter_{0, 0};
    std::vector<Terrain> terrains_{9};
    std::array<std::array<std::pair<int, int>, 3>, 3> grid_{};
    std::vector<Terrain> lowPolyTerrains_{16};
    
};


#endif // TERRAIN_HANDLER_H