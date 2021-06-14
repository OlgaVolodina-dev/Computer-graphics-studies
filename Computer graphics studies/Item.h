#pragma once


#ifndef ITEM_H
#define ITEM_H

#include "obj_reader.h"
#include "texture.h"
#include "shader.h"

struct ItemInitializationInfo
{
	std::string obj_file_name;
	std::string colorTexName;
	bool colorTexGenerateMipmap = false;
	std::string metallicTexName;
	SHADERS_OPTIONS vert;
	SHADERS_OPTIONS frag;
	bool shadowCaster = true;
	size_t n_indices = 1;
};

struct ItemModelPositionInfo
{
	ItemModelPositionInfo() = default;

	std::vector<glm::vec3> position_;
	std::vector<float> scale_ = { 1 };
	std::vector<float> rotation_angle_ = { 0.0 };
	std::vector<glm::vec3> rotation_axis_ = { glm::vec3(1.0, 0.0, 0.0) };
};

class Item
{
public:
	Item(ItemInitializationInfo& info);
	~Item();
	void LoadItem();
	void Draw();
	void DrawSimple();
	void DrawSimpleColor();
	ItemModelPositionInfo& GetWriteableModelInfo() { return posInfo; }
	const ItemModelPositionInfo& GetReadOnlyModelInfo() { return posInfo; }
	GLuint GetMainProgram() { return shader_; }
	std::vector<BoundingBox>& GetBoundingBoxs() { return bb_world; }

private:
	void CalculateBoundingBoxes();
	ItemInitializationInfo info;
	ItemModelPositionInfo posInfo;
	std::vector<Vertex> vertices;
	Texture colorTex_;
	Texture metallicTex_;
	ShaderProgram shader_;
	ShaderProgram simpleColorShader_;
	GLuint VAO;
	ShaderProgram depthPassShader_;
	BoundingBox bb_local;
	std::vector<glm::mat4> modelMatrix_ = { glm::mat4(1.0) };
	std::vector<BoundingBox> bb_world;
};

#endif // ITEM_H