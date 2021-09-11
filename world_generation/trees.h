#ifndef TREES_H
#define TREES_H
#include "Item.h"
#include "terrain.h"
#include <random>
class Trees 
{
public:
    void Init(Terrain &);
    void Draw();
    void Update(Terrain &terrain);
private:
    std::default_random_engine generator;
    std::normal_distribution<double> dist{0.0, 2.0};
    void GetTreesPosition(Terrain &, std::vector<glm::vec3> &);
    std::unique_ptr<Item> tree = nullptr;
};

#endif // TREES_H