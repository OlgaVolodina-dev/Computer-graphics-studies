#include "lightSource.h"
#include <iostream>


#define LIGHT_SPEED 0.6
LightSource::LightSource(glm::vec3 position):
	IUBOListener(),
	position_(position), 
    shader_(SIMPLE_VERT, SIMPLE_FRAG), 
    color_(glm::vec3(1.0f)), 
    initial_y(position.y)
{
    
    const float vertices[] = {
        -0.5f, -0.5f, -0.5f,
         0.5f, -0.5f, -0.5f, 
         0.5f,  0.5f, -0.5f, 
         0.5f,  0.5f, -0.5f, 
        -0.5f,  0.5f, -0.5f, 
        -0.5f, -0.5f, -0.5f, 

        -0.5f, -0.5f,  0.5f, 
         0.5f, -0.5f,  0.5f, 
         0.5f,  0.5f,  0.5f, 
         0.5f,  0.5f,  0.5f, 
        -0.5f,  0.5f,  0.5f, 
        -0.5f, -0.5f,  0.5f, 

        -0.5f,  0.5f,  0.5f, 
        -0.5f,  0.5f, -0.5f, 
        -0.5f, -0.5f, -0.5f, 
        -0.5f, -0.5f, -0.5f, 
        -0.5f, -0.5f,  0.5f, 
        -0.5f,  0.5f,  0.5f, 

         0.5f,  0.5f,  0.5f, 
         0.5f,  0.5f, -0.5f, 
         0.5f, -0.5f, -0.5f, 
         0.5f, -0.5f, -0.5f, 
         0.5f, -0.5f,  0.5f, 
         0.5f,  0.5f,  0.5f, 

        -0.5f, -0.5f, -0.5f, 
         0.5f, -0.5f, -0.5f, 
         0.5f, -0.5f,  0.5f, 
         0.5f, -0.5f,  0.5f, 
        -0.5f, -0.5f,  0.5f, 
        -0.5f, -0.5f, -0.5f, 

        -0.5f,  0.5f, -0.5f, 
         0.5f,  0.5f, -0.5f, 
         0.5f,  0.5f,  0.5f, 
         0.5f,  0.5f,  0.5f, 
        -0.5f,  0.5f,  0.5f, 
        -0.5f,  0.5f, -0.5f 
    };
    glGenVertexArrays(1, &VAO_);
    GLuint VBO;
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO_);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glm::mat4 modMat;
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, position_);
    modMat = glm::scale(model, glm::vec3(0.2));
    
    GLuint modMatBuf;
    glGenBuffers(1, &modMatBuf);
    glBindBuffer(GL_ARRAY_BUFFER, modMatBuf);
    glBufferData(GL_ARRAY_BUFFER, sizeof(modMat), &modMat, GL_STATIC_DRAW);
    for (int i = 0; i < 4; ++i) {
        glVertexAttribPointer(i + 3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (const void*)(sizeof(float) * (i * 4)));
        glEnableVertexAttribArray(i + 3);
        glVertexAttribDivisor(i + 3, 1);
    }
    glUniformBlockBinding(shader_, 0U, 0U);
}

std::size_t LightSource::GetUBOSize()
{
	return sizeof(glm::vec4)*3;
}

void LightSource::ResolvePosition()
{
    auto timePoint = std::chrono::steady_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(timePoint - start);
    start = timePoint;
    moveFactor += (LIGHT_SPEED * elapsed.count() / 1000.0);
    moveFactor = moveFactor > 3.14 ? moveFactor - 3.14 : moveFactor;
    position_.y = initial_y + std::sin(moveFactor);
}

void LightSource::SetData(std::size_t offset)
{
    ResolvePosition();
	glBufferSubData(GL_UNIFORM_BUFFER, offset, sizeof(glm::vec4), glm::value_ptr(position_));
	glBufferSubData(GL_UNIFORM_BUFFER, offset+ sizeof(glm::vec4), sizeof(glm::vec4), &attenuation);
	glBufferSubData(GL_UNIFORM_BUFFER, offset+ 2 * sizeof(glm::vec4), sizeof(glm::vec4), glm::value_ptr(lightColor_));
}


void LightSource::Draw()
{
    glUseProgram(shader_);
    glBindVertexArray(VAO_);

    glm::mat4 modMat;
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, position_);
    modMat = glm::scale(model, glm::vec3(0.2));

    GLuint modMatBuf;
    glGenBuffers(1, &modMatBuf);
    glBindBuffer(GL_ARRAY_BUFFER, modMatBuf);
    glBufferData(GL_ARRAY_BUFFER, sizeof(modMat), &modMat, GL_STATIC_DRAW);
    for (int i = 0; i < 4; ++i) {
        glVertexAttribPointer(i + 3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (const void*)(sizeof(float) * (i * 4)));
        glEnableVertexAttribArray(i + 3);
        glVertexAttribDivisor(i + 3, 1);
    }
    glUniform3fv(0, 1, glm::value_ptr(lightColor_));

    glDrawArrays(GL_TRIANGLES, 0, 36);
}