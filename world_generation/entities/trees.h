#ifndef TREES_H
#define TREES_H
#include "Item.h"
#include "world_generation/terrain/terrainHandler.h"
class Trees 
{
public:
    void Init(TerrainHandler &);
    void Draw();
private:
    void GetTreesPosition(TerrainHandler &, std::vector<glm::vec3> &);
    std::unique_ptr<Item> tree;
};

#endif // TREES_H