#ifndef TERRAIN_H
#define TERRAIN_H
#include "texture.h"
#include "shader.h"
#include "obj_reader.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "world_chunk_info.h"

class Terrain
{
public:
    Terrain();
    void Init(WorldChunkInfo const &worldChunkInfo);
    void Draw();
    void Update(WorldChunkInfo const &worldChunkInfo);
    void CheckUpdatedData(WorldChunkInfo const &worldChunkInfo);
private:
    struct TerrainVertex {
        glm::vec2 position;
        glm::vec2 texCoords;
    };
    void InitBiomeTex(WorldChunkInfo const &worldChunkInfo);
    void InitHeightmapTex(WorldChunkInfo const &worldChunkInfo); 

    GLuint VAO;
    
    GLuint VBOMatrix;
    GLuint EBO;
    ShaderProgram shader_;
    std::vector<TerrainVertex> vertices{};
    std::vector<unsigned int> indices{};
    Texture sand_;
    Texture grass_;
    Texture forest_;
    GLuint biomeTex_ = 0U;
    GLuint heightMap_ = 0U;
};

#endif // TERRAIN_H