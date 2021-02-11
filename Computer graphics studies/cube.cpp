#include "cube.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Cube::Cube() {
    const float vertices[] = {
    -1.0f, -1.0f, 0.5f, 0.0f, 0.0f,
    1.0f, -1.0f, 0.5f, 1.0f, 0.0f,
    1.0f,  1.0f, 0.5f, 1.0f, 1.0f
    };

    glm::vec3 cubePositions[] = {
     glm::vec3(0.0f,  0.0f,  0.0f),
    };

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

}