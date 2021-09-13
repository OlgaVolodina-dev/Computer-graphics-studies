#include "texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <iostream>
#include "utils.h"

Texture::Texture(std::string path, bool generateMipMap)
{
	if (path.empty()) {
		return;
	}
	int nrChannels;
	unsigned char* data = stbi_load(path.data(), &width_, &height_, &nrChannels, 0);
	if (!data) {
		std::cout << "Load texture failed" <<  " " << path << std::endl;
		return;
	}
	glGenTextures(1, &texture_);
	glBindTexture(GL_TEXTURE_2D, texture_);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	if (nrChannels == 3) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width_, height_, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	} else if (nrChannels == 1) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_R16, width_, height_, 0, GL_RED, GL_UNSIGNED_BYTE, data);
	} else if (nrChannels == 4) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width_, height_, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	}
	CHECK_OPENGL_ERROR()
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	if (!generateMipMap) {
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	}
	else {
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	}
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	if (generateMipMap) {
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	stbi_image_free(data);
}

void Texture::Use(unsigned int active)
{
	glActiveTexture(GL_TEXTURE0 + active);
	glBindTexture(GL_TEXTURE_2D, texture_);
}