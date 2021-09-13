#include "world_chunk.h"
#include <iostream>

void WorldChunk::Init(unsigned int numVertices, std::pair<int, int> gridNumber)
{
    terrain.Init(numVertices, gridNumber);
    tree.Init(terrain);
}

void WorldChunk::Update(std::pair<int, int> gridNumber) {
    terrain.Update(gridNumber);
}

void WorldChunk::CheckUpdatedData() {
    if (terrain.CheckUpdatedData()) {
        tree.Update(terrain);
    }
}

void WorldChunk::Draw()
{
    terrain.Draw();
    tree.Draw();
}