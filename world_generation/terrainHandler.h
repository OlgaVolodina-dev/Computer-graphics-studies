#ifndef TERRAIN_HANDLER_H
#define TERRAIN_HANDLER_H

#include "world_chunk.h"
#include "camera.h"

typedef std::pair<int, int> gridPair_t;

class TerrainHandler
{
public:
    void Process(Camera &camera);
    void Init();
    void Draw();
private:
    void GetFreeTerrainsAnsNums(std::vector<WorldChunk *> &, std::vector<gridPair_t> &);
    bool TerrainExists(int, int, std::vector<bool> &remain);
    std::pair<int, int> FindNewGridCenter(glm::vec3 &position, WorldChunk *&);
    std::pair<int, int> gridCenter_{0, 0};
    std::vector<WorldChunk> terrains_{9};
    std::array<std::array<std::pair<int, int>, 3>, 3> grid_{};
    
};


#endif // TERRAIN_HANDLER_H