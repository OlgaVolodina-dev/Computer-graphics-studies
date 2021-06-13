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
	glm::vec3 position_ = glm::vec3(0.0, 0.0, 0.0) ;
	float scale = 1.0;
	float rotation_angle = 0.0;
	glm::vec3 rotation_axis = glm::vec3(0.0, 0.0, 0.0);
	SHADERS_OPTIONS vert;
	SHADERS_OPTIONS frag;
	bool shadowCaster = true;
};

class Item : public Object
{
public:
	Item(ItemInitializationInfo& info);
	void LoadItem();
	void Draw();
	void DrawSimple();

private:
	ItemInitializationInfo info;
	std::vector<Vertex> vertices;
	Texture colorTex_;
	Texture metallicTex_;
	ShaderProgram shader_;
	GLuint VAO;
	ShaderProgram depthPassShader_;
	BoundingBox bb;
	glm::mat4 modelMatrix_ = glm::mat4(1.0);
};

#endif // ITEM_H