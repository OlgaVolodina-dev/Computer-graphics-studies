#include "shader.h"
#include <iostream>

std::unordered_map<SHADERS_OPTIONS, ShaderSourceHandler> ShaderProgram::shaderSourceHandler_{ {
	{SHADERS_OPTIONS::SIMPLE_VERT, &ShaderProgram::SimpleShaderVert},
	{SHADERS_OPTIONS::SIMPLE_FRAG, &ShaderProgram::SimpleShaderFrag},
	{SHADERS_OPTIONS::TESSELATION_VERT, &ShaderProgram::TesselationVert},
	{SHADERS_OPTIONS::TESSELATION_TES, &ShaderProgram::TesselationTES},
	{SHADERS_OPTIONS::TESSELATION_TC, &ShaderProgram::TesselationTC},
	{SHADERS_OPTIONS::TESSELATION_FRAG, &ShaderProgram::TesselationFrag},
	{SHADERS_OPTIONS::PHONG_VERT, &ShaderProgram::PhongShaderVert},
	{SHADERS_OPTIONS::PHONG_FRAG, &ShaderProgram::PhongShaderFrag}, 
	{SHADERS_OPTIONS::DEPTH_PASS_VERT, &ShaderProgram::DepthPassShaderVert}, 
	{SHADERS_OPTIONS::DEPTH_PASS_FRAG, &ShaderProgram::DepthPassShaderFrag}, 

} };

void ShaderProgram::DepthPassShaderVert(std::string& data) {
	data =
#include "depth.vert"
		;
}

void ShaderProgram::DepthPassShaderFrag(std::string& data) {
	data =
#include "depth.frag"
		;
}

void ShaderProgram::TesselationVert(std::string& data) {
	data =
#include "tesselation.vert"
		;
}

void ShaderProgram::TesselationFrag(std::string& data) {
	data =
#include "tesselation.frag"
		;
}

void ShaderProgram::TesselationTES(std::string& data) {
	data =
#include "tesselation.te"
	;
}

void ShaderProgram::TesselationTC(std::string& data) {
	data =
#include "tesselation.tc"
	;
}

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
		std::cout << vertex_data << std::endl;
	}

	glDeleteShader(vertex);
	glDeleteShader(fragment);
}

ShaderProgram::ShaderProgram(SHADERS_OPTIONS vertex_shader, SHADERS_OPTIONS TC, SHADERS_OPTIONS TES, SHADERS_OPTIONS fragment_shader)
{
	std::string vertex_data;
	std::string fragment_data;
	std::string tes_data;
	std::string tc_data;
	(*shaderSourceHandler_[vertex_shader])(vertex_data);
	(*shaderSourceHandler_[fragment_shader])(fragment_data);
	(*shaderSourceHandler_[TES])(tes_data);
	(*shaderSourceHandler_[TC])(tc_data);

	GLuint vertex = CompileShader(vertex_data, GL_VERTEX_SHADER);
	GLuint tes = CompileShader(tes_data, GL_TESS_EVALUATION_SHADER);
	GLuint tc = CompileShader(tc_data, GL_TESS_CONTROL_SHADER);
	GLuint fragment = CompileShader(fragment_data, GL_FRAGMENT_SHADER);
	int success;
	char infoLog[512];
	program_ = glCreateProgram();

	glAttachShader(program_, vertex);
	glAttachShader(program_, tes);
	glAttachShader(program_, tc);
	glAttachShader(program_, fragment);
	glLinkProgram(program_);
	glGetProgramiv(program_, GL_LINK_STATUS, &success);
	std::cout << "????" << std::endl;
	if (!success)
	{
		glGetProgramInfoLog(program_, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}

	glDeleteShader(vertex);
	glDeleteShader(fragment);
	glDeleteShader(tes);
}
