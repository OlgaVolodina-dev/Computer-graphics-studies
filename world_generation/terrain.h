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
    bool CheckUpdatedData();
    static unsigned int GetSize() { return size; }
    void GetBoundingBox(int &xMin, int &zMin, int &xMax, int &zMax);
private:
    void FillVertices();
    void InitBiomeTex();
    std::pair<int, int> xBoundary;
    std::pair<int, int> zBoundary;
    std::pair<int, int> gridNumber_;
    static unsigned int size;
    unsigned int numVertices_ = 0U;
    GLuint VAO;
    GLuint VBO;
    GLuint EBO;
    ShaderProgram shader_;
    std::vector<Vertex> vertices{};
    std::vector<unsigned int> indices{};
    Texture sand_;
    Texture grass_;
    Texture forest_;
    std::thread t;
    GLuint biomeTex_ = 0U;
    std::vector<uint8_t> biomeMap_;
};

#endif // TERRAIN_H