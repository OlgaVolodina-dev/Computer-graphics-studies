#include "grass.h"
#include "terrainGenerator.h"


Grass::Grass() :
shader_(GRASS_VERT, GRASS_FRAG), 
grass_("obj/grass/grass.png", false)
{}

void Grass::Init(WorldChunkInfo const &worldChunkInfo) {
    const float vertices[] = {
        -1.0, 1.0, 0.0, 1.0,
        1.0, -1.0, 1.0, 0.0,
        -1.0, -1.0, 0.0, 0.0,

        -1.0, 1.0, 0.0, 1.0,
        1.0, 1.0, 1.0, 1.0,
        1.0, -1.0, 1.0, 0.0
    };

    glGenVertexArrays(1, &VAO);
    GLuint VBO;
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, std::size(vertices) * sizeof(float), vertices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
	glEnableVertexAttribArray(1);

    Update(worldChunkInfo);
	glGenBuffers(1, &VBOPositions);
    CheckUpdatedData();
    // glm::vec3 position = glm::vec3(0.0, TerrainGenerator::GetHeight(0.0, 0.0) + 50.0F, 0.0);

	// glBindBuffer(GL_ARRAY_BUFFER, VBOPositions);

	// glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3), glm::value_ptr(position), GL_STATIC_DRAW);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (const void*)(0));
    glEnableVertexAttribArray(2);
    glVertexAttribDivisor(2, 1);

    glUniformBlockBinding(shader_, 0U, 0U);
}

void Grass::Update(WorldChunkInfo const &worldChunkInfo)
{
    positions.clear();
    int k = 0;
    for (int i = 0; i <= worldChunkInfo.numVertices_; ++i) {
        for (int j  = 0; j <= worldChunkInfo.numVertices_; ++j) {
            float x = worldChunkInfo.xBoundary.first + float(i) / worldChunkInfo.numVertices_ * worldChunkInfo.size;
            float z = worldChunkInfo.zBoundary.first + float(j) / worldChunkInfo.numVertices_ * worldChunkInfo.size;
            if (worldChunkInfo.biomeMap_[k] == static_cast<uint8_t>(TerrainGenerator::BIOME::MEADOW)) {
                float n = dist(generator);
                if (abs(n) > 3.0) {
                    positions.push_back(glm::vec3(x, worldChunkInfo.terrainHeightMap_[k] + 1.5F, z));
                }
            }
            k++; 
        }
    }
}

void Grass::CheckUpdatedData()
{
    glBindBuffer(GL_ARRAY_BUFFER, VBOPositions);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * positions.size(), positions.data(), GL_STATIC_DRAW);
}

void Grass::Draw() {
    grass_.Use(0);    
    glBindVertexArray(VAO);
	glUseProgram(shader_);
    glDrawArraysInstanced(GL_TRIANGLES, 0, 6, positions.size());
}