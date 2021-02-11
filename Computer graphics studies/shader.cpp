#include "shader.h"
#include <iostream>

std::unordered_map<SHADERS_OPTIONS, ShaderSourceHandler> ShaderProgram::shaderSourceHandler_{ {
	{SHADERS_OPTIONS::SIMPLE_VERT, &ShaderProgram::SimpleShaderVert},
	{SHADERS_OPTIONS::SIMPLE_FRAG, &ShaderProgram::SimpleShaderFrag},
	{SHADERS_OPTIONS::PHONG_VERT, &ShaderProgram::PhongShaderVert},
	{SHADERS_OPTIONS::PHONG_FRAG, &ShaderProgram::PhongShaderFrag}
} };


void ShaderProgram::SimpleShaderVert(std::string& data) {
	data =
#include "simple.vert"
	;
}

void ShaderProgram::SimpleShaderFrag(std::string& data) {
	data =
#include "simple.frag"
	;
}

void ShaderProgram::PhongShaderVert(std::string& data) {
	data =
#include "phong.vert"
		;
}

void ShaderProgram::PhongShaderFrag(std::string& data) {
	data =
#include "phong.frag"
		;
}


void ShaderProgram::Use() {
	glUseProgram(program_);
}

GLuint  ShaderProgram::CompileShader(std::string& data, GLenum shader_t) {
	struct StringHelper {
		const char* p;
		StringHelper(const std::string& s) : p(s.c_str()) {}
		operator const char** () { return &p; }
	};
	int success;
	char infoLog[512];
	GLuint shader = glCreateShader(shader_t);
	glShaderSource(shader, 1, StringHelper(data), NULL);
	glCompileShader(shader);
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(shader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	};
	return shader;
 }

ShaderProgram::ShaderProgram(SHADERS_OPTIONS vertex_shader, SHADERS_OPTIONS fragment_shader)
{
	std::string vertex_data;
	std::string fragment_data;
	(*shaderSourceHandler_[vertex_shader])(vertex_data);
	(*shaderSourceHandler_[fragment_shader])(fragment_data);

	GLuint vertex = CompileShader(vertex_data, GL_VERTEX_SHADER);
	GLuint fragment = CompileShader(fragment_data, GL_FRAGMENT_SHADER);
	int success;
	char infoLog[512];
	program_ = glCreateProgram();

	glAttachShader(program_, vertex);
	glAttachShader(program_, fragment);
	glLinkProgram(program_);
	glGetProgramiv(program_, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(program_, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}

	glDeleteShader(vertex);
	glDeleteShader(fragment);
}