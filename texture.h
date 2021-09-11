#ifndef TEXTURE_H
#define TEXTURE_H
#include <string>
#include "GL/glew.h"
class Texture
{
public:
	Texture(std::string, bool generateMipmap = false);
	void Use(unsigned int active = 0);
	int GetWidth() { return width_; }
	int GetHeight() { return height_; }

private:
	GLuint texture_;
	int width_ = 0;
	int height_ = 0;
};
#endif // TEXTURE_H
