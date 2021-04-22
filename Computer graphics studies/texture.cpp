#include "texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <iostream>

Texture::Texture(std::string path)
{

		int width, height, nrChannels;
		unsigned char* data = stbi_load(path.data(), &width, &height, &nrChannels, 0);

		if (!data) {
			std::cout << "Load texture failed";
			return;
		}
		glGenTextures(1, &texture_);
		glBindTexture(GL_TEXTURE_2D, texture_);
		std::cout << nrChannels << std::endl;
		if (nrChannels == 3) {
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		} else if (nrChannels == 1) {
			glTexImage2D(GL_TEXTURE_2D, 0, GL_R8UI, width, height, 0, GL_RED, GL_UNSIGNED_BYTE, data);
		}
		else if (nrChannels == 4) {
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

		}
		auto err = glGetError();
		if (err != GL_NO_ERROR) {
			if (err == GL_INVALID_ENUM) {
				std::cout << "GL_INVALID_ENUM" << std::endl;

			}
			else if (err == GL_INVALID_VALUE) {

				std::cout << "GL_INVALID_VALUE" << std::endl;
			}
			else if (err == GL_INVALID_OPERATION) {

				std::cout << "GL_INVALID_OPERATION" << std::endl;
			}
			std::cout << "ERROR" << std::endl;
		}
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glGenerateMipmap(GL_TEXTURE_2D);
		stbi_image_free(data);
}

void Texture::Use(unsigned int active)
{
	glActiveTexture(GL_TEXTURE0 + active);
	glBindTexture(GL_TEXTURE_2D, texture_);
}