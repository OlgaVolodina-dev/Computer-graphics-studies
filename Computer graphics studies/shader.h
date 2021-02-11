


#ifndef SHADER_PROGRAM_H
#define SHADER_PROGRAM_H

#include <string>
#include <unordered_map>
#include "GL/glew.h"

enum SHADERS_OPTIONS {
	SIMPLE_VERT = 0,
	SIMPLE_FRAG, 
	PHONG_VERT,
	PHONG_FRAG
};


typedef void (*ShaderSourceHandler)(std::string&);
class ShaderProgram
{
public:
	ShaderProgram(SHADERS_OPTIONS, SHADERS_OPTIONS);
	void Use();
	operator GLuint() { return program_; };
private:
	static void SimpleShaderVert(std::string& data);
	static void SimpleShaderFrag(std::string& data);
	static void PhongShaderVert(std::string& data);
	static void PhongShaderFrag(std::string& data);
	static std::unordered_map<SHADERS_OPTIONS, ShaderSourceHandler> shaderSourceHandler_;
	GLuint CompileShader(std::string&, GLenum);
	GLuint program_ = 0;
};

#endif // SHADER_PROGRAM_H
