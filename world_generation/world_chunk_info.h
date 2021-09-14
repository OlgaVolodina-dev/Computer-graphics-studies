#ifndef WORLD_CHUNK_INFO
#define WORLD_CHUNK_INFO

#include <vector>
#include <stdint.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

struct WorldChunkInfo
{
    std::vector<uint8_t> biomeMap_;
    std::vector<float> terrainHeightMap_;
    std::pair<int, int> xBoundary;
    std::pair<int, int> zBoundary;
    unsigned int numVertices_ = 0U;
    std::pair<int, int> gridNumber_;
    size_t size = 256U;

    void GetBoundingBox(int &xMin, int &zMin, int &xMax, int &zMax) const {
        std::tie(xMin, xMax) = xBoundary;
        std::tie(zMin, zMax) = zBoundary;
    }
    
    bool PointInTerrain(glm::vec3 &position) const
    {
        return xBoundary.first <= position.x && xBoundary.second >= position.x && 
        zBoundary.first <= position.z && zBoundary.second >= position.z;
    }
};

#endif // WORLD_CHUNK_INFO