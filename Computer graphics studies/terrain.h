#pragma once
#ifndef TERRAIN_H
#define TERRAIN_H
#include "shader.h"
#include "texture.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "GL/glew.h"
#include "shadowManager.h"


class Terrain : public Object
{
public:
	Terrain();
	void Draw();
	void DrawSimple();

private:
	GLuint VAO_ = 0U;
	glm::vec3 color_;
	glm::vec3 position_;
	ShaderProgram shader_;
	ShaderProgram depthPassShader_;
	Texture heightmap_;
	Texture colormap_;

};

#endif //TERRAIN_H

