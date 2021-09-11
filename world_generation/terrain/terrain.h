#ifndef TERRAIN_H
#define TERRAIN_H
#include "texture.h"
#include "shader.h"
#include "obj_reader.h"
#include <thread>


class Terrain
{
public:
    Terrain();
    void Init(unsigned int numVertices, std::pair<int, int> gridNumber);
    void Draw();
    void Update(std::pair<int, int> gridNumber);
    bool PointInTerrain(glm::vec3 &position);
    std::pair<int, int> GetGridNumber() { return gridNumber_; }
    void CheckUpdatedData();
    static unsigned int GetSize() { return size; }
private:
    void FillVertices();
    std::pair<int, int> xBoundary;
    std::pair<int, int> zBoundary;
    std::pair<int, int> gridNumber_;
    static unsigned int size;
    unsigned int numVertices_ = 0U;
    GLuint VAO;
    GLuint VBO;
    ShaderProgram shader_;
    std::vector<Vertex> vertices{};
    Texture sand_;
    Texture grass_;
    std::thread t;
};

#endif // TERRAIN_H