


#ifndef SHADER_PROGRAM_H
#define SHADER_PROGRAM_H

#include <string>
#include <unordered_map>
#include "GL/glew.h"

enum SHADERS_OPTIONS {
	SIMPLE_VERT = 0,
	SIMPLE_FRAG, 
	PHONG_VERT,
	PHONG_FRAG,
	TESSELATION_VERT,
	TESSELATION_FRAG,
	TESSELATION_TES, 
	TESSELATION_TC , 
	DEPTH_PASS_VERT,
	DEPTH_PASS_FRAG,
	QUAD_VERT,
	QUAD_FRAG, 
	BLOOM_PREPROCESSING_FRAG,
	BLOOM_POSTPROCESSING_FRAG

};


typedef void (*ShaderSourceHandler)(std::string&);
class ShaderProgram
{
public:
	ShaderProgram(SHADERS_OPTIONS, SHADERS_OPTIONS);
	ShaderProgram(SHADERS_OPTIONS vertex_shader, SHADERS_OPTIONS TC, SHADERS_OPTIONS TES, SHADERS_OPTIONS fragment_shader);
	void Use();
	operator GLuint() { return program_; };
private:

	static void DepthPassShaderVert(std::string& data);
	static void DepthPassShaderFrag(std::string& data);
	static void SimpleShaderVert(std::string& data);
	static void SimpleShaderFrag(std::string& data);
	static void PhongShaderVert(std::string& data);
	static void PhongShaderFrag(std::string& data);
	static void TesselationVert(std::string& data);
	static void TesselationFrag(std::string& data);
	static void TesselationTES(std::string& data);
	static void TesselationTC(std::string& data);
	static void QuadShaderVert(std::string& data);
	static void QuadShaderFrag(std::string& data);
	static void BloomPreprocessingShaderFrag(std::string& data);
	static void BloomPostprocessingShaderFrag(std::string& data);
	static std::unordered_map<SHADERS_OPTIONS, ShaderSourceHandler> shaderSourceHandler_;
	GLuint CompileShader(std::string&, GLenum);
	GLuint program_ = 0;
};

#endif // SHADER_PROGRAM_H
