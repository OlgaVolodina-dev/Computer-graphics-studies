#include "terrainGenerator.h"
#include <glm/gtc/noise.hpp>
#include <iostream>

float TerrainGenerator::GetHeight(int x, int z)
{
    return glm::perlin(glm::vec2(x/32.0F, z/32.0F)) * 10.0F +
            glm::perlin(glm::vec2(x/16.0F, z/16.0F)) * 1.0F +
            glm::perlin(glm::vec2(x/256.0F, z/256.0F)) * 50.0F; 
}


TerrainGenerator::BIOME TerrainGenerator::GetBiome(int x, int z) {
    float y = GetHeight(x, z);
    if (y < -4.0) {
        return BIOME::OCEAN;
    }
    return BIOME::FOREST;
}