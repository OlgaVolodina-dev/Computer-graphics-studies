#include "terrain.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include <iostream>
#include <glm/gtc/noise.hpp>

#include "utils.h"
#include "terrainGenerator.h"

unsigned int Terrain::size = 256U;
Terrain::Terrain():
    shader_(TERRAIN_SHADER_VERT, TERRAIN_SHADER_FRAG), 
    sand_("obj/terrain/sand.jpg", true),
    grass_("obj/terrain/grass.jpg", true),
    forest_("obj/terrain/forest.jpg", true)
{}

bool Terrain::PointInTerrain(glm::vec3 &position)
{
    return xBoundary.first <= position.x && xBoundary.second >= position.x && 
        zBoundary.first <= position.z && zBoundary.second >= position.z;
}

void Terrain::GetBoundingBox(int &xMin, int &zMin, int &xMax, int &zMax)
{
    std::tie(xMin, xMax) = xBoundary;
    std::tie(zMin, zMax) = zBoundary;
}

void Terrain::FillVertices()
{
    int k = 0;
    for (int i = 0; i <= numVertices_; ++i) {
        for (int j  = 0; j <= numVertices_; ++j) {
            float x = xBoundary.first + float(i) / numVertices_ * size;
            float z = zBoundary.first + float(j) / numVertices_ * size;
            
            const float shift = 1.0 / numVertices_ * size * 2.0;
            glm::vec3 position(x, TerrainGenerator::GetHeight(x, z), z);
            glm::vec3 v1(x + shift, TerrainGenerator::GetHeight(x + shift, z + shift), z + shift);
            glm::vec3 v2(x - shift, TerrainGenerator::GetHeight(x - shift, z- shift), z-shift);
            glm::vec3 v3(x - shift, TerrainGenerator::GetHeight(x - shift, z + shift), z+ shift);
            glm::vec3 v4(x+ shift, TerrainGenerator::GetHeight(x+ shift, z - shift), z - shift);

            glm::vec3 a = v1 - v2;
            glm::vec3 b = v3 - v4;

            glm::vec3 normal = glm::normalize(glm::cross(b, a));
            Vertex v {
                glm::vec3(position),
                // OpenGL texture start point is lower left
                glm::vec2(float(j) / numVertices_, float(i) / numVertices_),
                glm::vec3(normal)
            };
            vertices[k++]  = std::move(v);
        } 
    }
    k = 0;
    for (int i = 0; i < numVertices_; ++i) {
        for (int j  = 0; j < numVertices_; ++j) {
            float x = xBoundary.first + (float(i) + 0.5) / numVertices_ * size;
            float z = zBoundary.first + (float(j) + 0.5) / numVertices_ * size;
            biomeMap_[k++] = static_cast<uint8_t>(TerrainGenerator::GetBiome(x, z));
        }
    }
}

void Terrain::InitBiomeTex()
{
    if (!biomeTex_) {
        glGenTextures(1, &biomeTex_);
    }
    glBindTexture(GL_TEXTURE_2D, biomeTex_);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_R8UI, numVertices_, numVertices_, 0, GL_RED_INTEGER, GL_UNSIGNED_BYTE, biomeMap_.data());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
}

void Terrain::Init(unsigned int numVertices, std::pair<int, int> gridNumber)
{
    numVertices_ = numVertices; 
    gridNumber_ = gridNumber;
    xBoundary = {gridNumber.first * size, (gridNumber.first + 1) * size};
    zBoundary = {gridNumber.second * size, (gridNumber.second + 1) * size};
    vertices.resize((numVertices_ + 1) * (numVertices_ + 1));
    biomeMap_.resize((numVertices_ + 1) * (numVertices_ + 1));
    FillVertices();
    InitBiomeTex();
    
    int row_shift = numVertices_ + 1;
    for (int i = 0; i < numVertices_; ++i) {
        for (int j = 0; j < numVertices_; ++j) {

            indices.push_back(i * row_shift + j);
            indices.push_back(i * row_shift + j + 1);
            indices.push_back((i+1) * row_shift + j);
        }
    }

    for (int i = 1; i <= numVertices_; ++i) {
        for (int j = 1; j <= numVertices_; ++j) {

            indices.push_back(i * row_shift + j - 1);
            indices.push_back(i * row_shift + j);
            indices.push_back((i-1) * row_shift + j);
        }
    }

    glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

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

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, std::size(indices) * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);


    glUniformBlockBinding(shader_, 0U, 0U);
}

bool Terrain::CheckUpdatedData()
{
    if (t.joinable()) {
        t.join();
        InitBiomeTex();
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        void* ptr = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
        memcpy(ptr, vertices.data(), std::size(vertices) * sizeof(Vertex));
        bool result = glUnmapBuffer(GL_ARRAY_BUFFER);
        if (!result) {
            std::cout << "Unmap error" << std::endl;
        }
        return true;
    }
    return false;
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
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, biomeTex_);
    glBindVertexArray(VAO);
	glUseProgram(shader_);
    sand_.Use(1);
    grass_.Use(2);
    forest_.Use(3);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, (void*)0);

    // glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

}

