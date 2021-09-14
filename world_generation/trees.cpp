#include "trees.h"
#include <iostream>
#include <random>
#include "terrainGenerator.h"


void Trees::Init(WorldChunkInfo const &worldChunkInfo)
{
    ItemInitializationInfo treesInfo;
	treesInfo.obj_file_name = "obj/trees/lowpolytree.obj";
	treesInfo.vert = SIMPLE_LIGHT_VERT;
	treesInfo.frag = SIMPLE_LIGHT_FRAG;
	treesInfo.colorTexGenerateMipmap = false;
	tree.reset(new Item(treesInfo));
    tree->LoadItem();
    Update(worldChunkInfo);
    CheckUpdatedData();
}

void Trees::Update(WorldChunkInfo const &worldChunkInfo)
{
    std::vector<glm::vec3> positions;
    GetTreesPosition(worldChunkInfo, positions);
    if (positions.size() == 0) {
        tree->Reset();
        return;
    }

    ItemModelPositionInfo& treesModelInfo = tree->GetWriteableModelInfo();
    treesModelInfo.n_indices = positions.size();
	treesModelInfo.position_ = positions;
    treesModelInfo.rotation_axis_.clear();
	treesModelInfo.rotation_angle_.clear();
	treesModelInfo.scale_.clear();
	for (int i = 0; i < treesModelInfo.position_.size(); ++i) {
		treesModelInfo.rotation_axis_.push_back(glm::vec3(1.0, 1.0, 1.0));
		treesModelInfo.rotation_angle_.push_back(1.0);
		treesModelInfo.scale_.push_back(1.0);
	}
}

void Trees::CheckUpdatedData()
{
    tree->LoadBuffers();	
}

void Trees::GetTreesPosition(WorldChunkInfo const &worldChunkInfo, std::vector<glm::vec3> &positions)
{
    int xMin, xMax, zMin, zMax;
    worldChunkInfo.GetBoundingBox(xMin, zMin, xMax, zMax);
    
    for (int x = xMin; x < xMax; ++x) {
        for (int z = zMin; z < zMax; ++z){
            if (TerrainGenerator::GetBiome(x, z) == TerrainGenerator::BIOME::FOREST) {
                float n = dist(generator);
                if (abs(n) > 6.0) {
                    positions.push_back(glm::vec3(x, TerrainGenerator::GetHeight(x, z) + 1.5F, z));
                }
            } 
        }
    }
    
} 

void Trees::Draw()
{
    if (tree) {
        tree->Draw();
    }
}
