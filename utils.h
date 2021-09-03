#pragma once

#ifndef UTILS_H
#define UTILS_H
#include "GL/glew.h"
#include <iostream>

#define CHECK_OPENGL_ERROR() \
{\
	const GLenum err = glGetError(); \
	if (GL_NO_ERROR != err) { \
		switch (err) \
		{ \
		case GL_INVALID_ENUM: \
			std::cout << "GL_INVALID_ENUM" << __FILE__ << " " << __LINE__ << std::endl; \
			break; \
		case GL_INVALID_VALUE:    \
			std::cout << "GL_INVALID_VALUE" << __FILE__ <<" " << __LINE__<< std::endl; \
			break; \
		case GL_INVALID_OPERATION: \
			std::cout << "GL_INVALID_OPERATION" << __FILE__ <<" " << __LINE__<< std::endl; \
			break; \
		case GL_STACK_OVERFLOW: \
			std::cout << "GL_STACK_OVERFLOW" << __FILE__ <<" " << __LINE__<< std::endl; \
			break; \
		case GL_STACK_UNDERFLOW: \
			std::cout << "GL_STACK_UNDERFLOW" << __FILE__ << " " <<__LINE__<< std::endl; \
			break; \
		case GL_OUT_OF_MEMORY: \
			std::cout << "GL_OUT_OF_MEMORY" << __FILE__ << " " <<__LINE__<< std::endl; \
			break; \
		default:   \
			std::cout << "Unknown error" << err << __FILE__ << " " << __LINE__<< std::endl; \
		} \
	}\
}

#define CHECK_FRAMEBUFFER_STATUS() \
{\
	GLenum err = glCheckFramebufferStatus(GL_FRAMEBUFFER);\
	if (err != GL_FRAMEBUFFER_COMPLETE) { \
		std::cout << "Incomplete"; \
	} \
}

class Utils
{

};

#endif // UTILS_H

