#ifndef TEXTURE_H
#define TEXTURE_H
#include <string>
#include "GL/glew.h"
class Texture
{
public:

	Texture(std::string, bool generateMipmap = false);
	void Use(unsigned int active = 0);
private:
	GLuint texture_;
};
#endif // TEXTURE_H
