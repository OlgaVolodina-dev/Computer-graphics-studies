#include "terrain.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include <iostream>
#include <glm/gtc/noise.hpp>

#include "utils.h"
#include "terrainGenerator.h"

unsigned int Terrain::size = 128U;
Terrain::Terrain():
    shader_(TERRAIN_SHADER_VERT, TERRAIN_SHADER_FRAG), 
    sand_("obj/terrain/sand.jpg", true),
    grass_("obj/terrain/grass.jpg", true)
{}

bool Terrain::PointInTerrain(glm::vec3 &position)
{
    return xBoundary.first <= position.x && xBoundary.second >= position.x && 
        zBoundary.first <= position.z && zBoundary.second >= position.z;
}

void Terrain::FillVertices()
{
    int k = 0;
    for (int i = 0; i < numVertices_; ++i) {
        for (int j  = 0; j < numVertices_; ++j) {
            auto createVertex = [&](int i, int j) {
                float x = xBoundary.first + float(i) / numVertices_ * size;
                float z = zBoundary.first + float(j) / numVertices_ * size;
                Vertex v {
                    glm::vec3(x, TerrainGenerator::GetHeight(x, z), z), 
                    glm::vec2(float(i), float(j)),
                    glm::vec3(0.0)
                };
                return v;
            };
            
            vertices[k++] = createVertex(i, j);
            vertices[k++] = createVertex(i+1, j);
            vertices[k++] = createVertex(i, j+1);
            vertices[k++] = createVertex(i, j+1);
            vertices[k++] = createVertex(i+1, j);
            vertices[k++] = createVertex(i+1, j+1);
        } 
    }

    for (int i  = 0; i < vertices.size(); i+=3)
    {
        Vertex &v1 = vertices[i];
        Vertex &v2 = vertices[i+1];
        Vertex &v3 = vertices[i+2];

        glm::vec3 a = v2.position - v1.position;
        glm::vec3 b = v3.position - v1.position;

        glm::vec3 normal = glm::cross(a, b);

        v1.normal = normal;
        v2.normal = normal;
        v3.normal = normal;
    }
}

void Terrain::Init(unsigned int numVertices, std::pair<int, int> gridNumber)
{
    numVertices_ = numVertices; 
    gridNumber_ = gridNumber;
    xBoundary = {gridNumber.first * size, (gridNumber.first + 1) * size};
    zBoundary = {gridNumber.second * size, (gridNumber.second + 1) * size};
    vertices.resize(numVertices * numVertices * 6);
    FillVertices();

    glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, std::size(vertices) * sizeof(Vertex), vertices.data(), GL_DYNAMIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
	glEnableVertexAttribArray(2);

    glm::mat4 modelMatrix_ = glm::mat4(1.0f);

	GLuint modMatBuf;
	glGenBuffers(1, &modMatBuf);
	glBindBuffer(GL_ARRAY_BUFFER, modMatBuf);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4), glm::value_ptr(modelMatrix_), GL_STATIC_DRAW);
	for (int i = 0; i < 4; ++i) {
		glVertexAttribPointer(i + 3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (const void*)(sizeof(float) * (i * 4)));
		glEnableVertexAttribArray(i + 3);
		glVertexAttribDivisor(i + 3, 1);
	}
    glUniformBlockBinding(shader_, 0U, 0U);
}

void Terrain::CheckUpdatedData()
{
    if (t.joinable()) {
        t.join();
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        void* ptr = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
        memcpy(ptr, vertices.data(), std::size(vertices) * sizeof(Vertex));
        bool result = glUnmapBuffer(GL_ARRAY_BUFFER);
        if (!result) {
            std::cout << "Unmap error" << std::endl;
        }
    }
}

void Terrain::Update(std::pair<int, int> gridNumber)
{
    gridNumber_ = gridNumber;
    xBoundary = {gridNumber.first * size, (gridNumber.first + 1) * size};
    zBoundary = {gridNumber.second * size, (gridNumber.second + 1) * size};
    t =  std::thread(&Terrain::FillVertices, this);
}

void Terrain::Draw()
{
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glBindVertexArray(VAO);
	glUseProgram(shader_);
    sand_.Use(0);
    grass_.Use(1);
    glDrawArrays(GL_TRIANGLES, 0, std::size(vertices));

    // glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

}

