#include "terrain.h"


#include <vector>
#include <iostream>
#include <glm/gtc/noise.hpp>

#include "utils.h"
#include "terrainGenerator.h"

Terrain::Terrain():
    shader_(TERRAIN_SHADER_VERT, TERRAIN_SHADER_FRAG), 
    sand_("obj/terrain/sand.jpg", true),
    grass_("obj/terrain/grass.jpg", true),
    forest_("obj/terrain/forest.jpg", true)
{}

void Terrain::InitBiomeTex(WorldChunkInfo const &worldChunkInfo)
{
    auto pixelSize = worldChunkInfo.numVertices_ + 1; 

    if (!biomeTex_) {
        glGenTextures(1, &biomeTex_);
    }
    glBindTexture(GL_TEXTURE_2D, biomeTex_);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_R8UI, pixelSize, pixelSize, 0, GL_RED_INTEGER, GL_UNSIGNED_BYTE, worldChunkInfo.biomeMap_.data());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
}

void Terrain::InitHeightmapTex(WorldChunkInfo const &worldChunkInfo) {
    auto pixelSize = worldChunkInfo.numVertices_ + 1; 

    if (!heightMap_) {
        glGenTextures(1, &heightMap_);
    }
    glBindTexture(GL_TEXTURE_2D, heightMap_);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F, pixelSize , pixelSize , 0, GL_RED, GL_FLOAT, worldChunkInfo.terrainHeightMap_.data());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

}

void Terrain::Init(WorldChunkInfo const &worldChunkInfo)
{
    auto &numVertices = worldChunkInfo.numVertices_;
    auto &size = worldChunkInfo.size;

    vertices.resize((numVertices  + 1) * (numVertices  + 1));
    int k = 0;
    for (int i = 0; i <= numVertices; ++i) {
        for (int j  = 0; j <= numVertices; ++j) {
            float x = float(i) / numVertices * size;
            float z = float(j) / numVertices * size;
            
            const float shift = 1.0 / numVertices * size * 2.0;
            glm::vec2 position(x, z);
            TerrainVertex v {
                glm::vec2(position),
                // OpenGL texture start point is lower left
                glm::vec2(float(2 * j + 1) / (2 * (numVertices + 1) ), float(2 * i + 1) / (2 * (numVertices + 1)))
            };
            vertices[k++]  = std::move(v);
        } 
    }

    InitBiomeTex(worldChunkInfo);
    InitHeightmapTex(worldChunkInfo);
    
    int row_shift = numVertices + 1;
    for (int i = 0; i < numVertices; ++i) {
        for (int j = 0; j < numVertices; ++j) {

            indices.push_back(i * row_shift + j);
            indices.push_back(i * row_shift + j + 1);
            indices.push_back((i+1) * row_shift + j);
        }
    }

    for (int i = 1; i <= numVertices; ++i) {
        for (int j = 1; j <= numVertices; ++j) {

            indices.push_back(i * row_shift + j - 1);
            indices.push_back(i * row_shift + j);
            indices.push_back((i-1) * row_shift + j);
        }
    }

    glGenVertexArrays(1, &VAO);
    GLuint VBO;
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, std::size(vertices) * sizeof(TerrainVertex), vertices.data(), GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
	glEnableVertexAttribArray(1);

    glm::mat4 modelMatrix_ = glm::mat4(1.0f);
    glm::mat4 model = glm::mat4(1.0f);
    modelMatrix_ = glm::translate(model, glm::vec3(worldChunkInfo.xBoundary.first, 0.0, worldChunkInfo.zBoundary.first));

	glGenBuffers(1, &VBOMatrix);
	glBindBuffer(GL_ARRAY_BUFFER, VBOMatrix);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4), glm::value_ptr(modelMatrix_), GL_STATIC_DRAW);
	for (int i = 0; i < 4; ++i) {
        GLint attribPointer = i + 2;
		glVertexAttribPointer(attribPointer, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (const void*)(sizeof(float) * (i * 4)));
		glEnableVertexAttribArray(attribPointer);
		glVertexAttribDivisor(attribPointer, 1);
	}

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, std::size(indices) * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);


    glUniformBlockBinding(shader_, 0U, 0U);
}

void Terrain::CheckUpdatedData(WorldChunkInfo const &worldChunkInfo)
{
    InitBiomeTex(worldChunkInfo);
    InitHeightmapTex(worldChunkInfo);
    glm::mat4 modelMatrix_ = glm::mat4(1.0f);
    glm::mat4 model = glm::mat4(1.0f);
    modelMatrix_ = glm::translate(model, glm::vec3(worldChunkInfo.xBoundary.first, 0.0, worldChunkInfo.zBoundary.first));
    glBindBuffer(GL_ARRAY_BUFFER, VBOMatrix);
    void* ptr = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
    memcpy(ptr, glm::value_ptr(modelMatrix_), sizeof(glm::mat4));
    bool result = glUnmapBuffer(GL_ARRAY_BUFFER);
    if (!result) {
        std::cout << "Unmap error" << std::endl;
    }
}

void Terrain::Update(WorldChunkInfo const &worldChunkInfo)
{

}

void Terrain::Draw()
{
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, biomeTex_);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, heightMap_);

    glBindVertexArray(VAO);
	glUseProgram(shader_);
    sand_.Use(2);
    grass_.Use(3);
    forest_.Use(4);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, (void*)0);

    // glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

}

