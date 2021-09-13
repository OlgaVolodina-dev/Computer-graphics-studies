#ifndef TERRAIN_GENERATOR_H
#define TERRAIN_GENERATOR_H
#include <random>
class TerrainGenerator
{
public:
    enum class BIOME : uint8_t {
        OCEAN,
        FOREST, 
        MEADOW
    };
    static float GetHeight(int x, int z);
    static BIOME GetBiome(int x, int z);
private:
    static float GetNoise(int x, int z);
    static float GetSmoothNoise(int x, int z);
    static float GetInterpolatedNoise(int x, int z);

};


#endif // TERRAIN_GENERATOR_H