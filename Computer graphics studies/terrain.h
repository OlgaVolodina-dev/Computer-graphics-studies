#pragma once

#ifndef TERRAIN_H
#define TERRAIN_H
#include "shader.h"
#include "texture.h"
#include "shadowManager.h"
class Terrain : public Object
{
public:
	Terrain();
	void DrawSimple();
	void Draw();
private:
	GLuint VAO = 0U;
	ShaderProgram shader_;
	ShaderProgram depthPassShader_;
	Texture texture_;
	glm::vec3 position_ = glm::vec3(0.0, -1.0, 0.0);


};

#endif // TERRAIN_H
