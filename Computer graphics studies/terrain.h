#pragma once
#ifndef TERRAIN_H
#define TERRAIN_H
#include "shader.h"
#include "texture.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "GL/glew.h"


class Terrain
{
public:
	Terrain();
	void Draw();
private:
	GLuint VAO_ = 0U;
	glm::vec3 color_;
	glm::vec3 position_;
	ShaderProgram shader_;
	Texture heightmap_;

};

#endif TERRAIN_H

