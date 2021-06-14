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
	GLuint cacheTex = 0;
	void Blur(GLuint, size_t, size_t, bool generate_mipmap = true);
	void UpdateWindowSize(int width, int height);
	void CreateTextures();
private:
	GLuint emptyVAO;
	GLuint pingFbo;
	GLuint pongFbo;
	ShaderProgram one_axis_blur;
	int scr_width = 800, scr_height = 600;

};
#endif
