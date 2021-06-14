#pragma once

#ifndef GAUSSIAN_BLUR_H
#define GAUSSIAN_BLUR_H
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "GL/glew.h"
#include "shader.h"
class GaussianBlur
{
public:
	GaussianBlur();
	GLuint cacheTex;
	void Blur(GLuint, size_t, size_t, bool generate_mipmap = true);
private:
	GLuint emptyVAO;
	GLuint pingFbo;
	GLuint pongFbo;
	ShaderProgram one_axis_blur;

};
#endif
