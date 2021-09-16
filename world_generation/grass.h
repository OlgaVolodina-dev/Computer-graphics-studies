#ifndef GRASS_H
#define GRASS_H
#include "world_chunk_info.h"
#include "shader.h"
#include "texture.h"

#include <random>

class Grass {
public:
    Grass();
    void Init(WorldChunkInfo const &worldChunkInfo);
    void Update(WorldChunkInfo const &worldChunkInfo);
    void CheckUpdatedData();
    void Draw();

private:
    std::default_random_engine generator;
    std::normal_distribution<double> dist{0.0, 2.0};
    GLuint VAO;
    GLuint VBOPositions;
    ShaderProgram shader_;
    Texture grass_;
    std::vector<glm::vec3> positions;


};

#endif // GRASS_H