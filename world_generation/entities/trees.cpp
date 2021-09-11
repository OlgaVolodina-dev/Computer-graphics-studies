#include "trees.h"
#include <iostream>
#include "world_generation/terrain/TerrainGenerator.h"

void Trees::Init(TerrainHandler &terrainHandler)
{
    std::vector<glm::vec3> positions;
    GetTreesPosition(terrainHandler, positions);

    if (positions.size() == 0) {
        abort();
    }
    ItemInitializationInfo treesInfo;
	treesInfo.obj_file_name = "obj/trees/lowpolytree.obj";
	treesInfo.vert = SIMPLE_LIGHT_VERT;
	treesInfo.frag = SIMPLE_LIGHT_FRAG;
	treesInfo.colorTexGenerateMipmap = false;
    treesInfo.n_indices = positions.size();

	tree.reset(new Item(treesInfo));

	ItemModelPositionInfo& treesModelInfo = tree->GetWriteableModelInfo();
	treesModelInfo.position_ = positions;
    treesModelInfo.rotation_axis_.clear();
	treesModelInfo.rotation_angle_.clear();
	treesModelInfo.scale_.clear();
	for (int i = 0; i < treesModelInfo.position_.size(); ++i) {
		treesModelInfo.rotation_axis_.push_back(glm::vec3(1.0, 1.0, 1.0));
		treesModelInfo.rotation_angle_.push_back(1.0);
		treesModelInfo.scale_.push_back(1.0);
	}
    tree->LoadItem();
	
}

void Trees::GetTreesPosition(TerrainHandler & terrainHandler, std::vector<glm::vec3> &positions)
{
    int xMin, xMax, zMin, zMax;
    std::cout<< xMax;
    terrainHandler.GetVisibleSpace(xMin, zMin, xMax, zMax);
    const double mean = 0.0;
    const double stddev = 2.0;
    std::default_random_engine generator;
    std::normal_distribution<double> dist(mean, stddev);
    for (int x = xMin; x < xMax; ++x) {
        for (int z = xMin; z < zMax; ++z){
            if (TerrainGenerator::GetBiome(x, z) == TerrainGenerator::BIOME::FOREST) {
                float n = dist(generator);
                if (abs(n) > 6.0) {
                    positions.push_back(glm::vec3(x, TerrainGenerator::GetHeight(x, z) + 2.5F, z));
                }
            }

        }
    }
    std::cout << positions.size() << std::endl;
} 

void Trees::Draw()
{
    tree->Draw();
}
