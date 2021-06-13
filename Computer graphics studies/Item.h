#pragma once


#ifndef ITEM_H
#define ITEM_H

#include "obj_reader.h"
#include "texture.h"
#include "shader.h"
#include "shadowManager.h"

struct ItemInitializationInfo
{
	std::string obj_file_name;
	std::string colorTexName;
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

class Item : public Object
{
public:
	Item(ItemInitializationInfo & info);
	void LoadItem();
	void Draw();
	void DrawSimple();

	ItemModelPositionInfo& GetModelInfo() { return posInfo; }

private:
	ItemInitializationInfo info;
	ItemModelPositionInfo posInfo;
	std::vector<Vertex> vertices;
	Texture colorTex_;
	Texture metallicTex_;
	ShaderProgram shader_;
	GLuint VAO;
	ShaderProgram depthPassShader_;
	BoundingBox bb;
	std::vector<glm::mat4> modelMatrix_ = { glm::mat4(1.0) };
};

#endif // ITEM_H