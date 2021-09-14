#ifndef TREES_H
#define TREES_H
#include "Item.h"
#include <random>
#include <memory>
#include "world_chunk_info.h"
class Trees 
{
public:
    void Init(WorldChunkInfo const &worldChunkInfo);
    void Draw();
    void Update(WorldChunkInfo const &worldChunkInfo);
    void CheckUpdatedData();
private:
    std::default_random_engine generator;
    std::normal_distribution<double> dist{0.0, 2.0};
    void GetTreesPosition(WorldChunkInfo const &worldChunkInfo, std::vector<glm::vec3> &);
    std::unique_ptr<Item> tree = nullptr;
};

#endif // TREES_H