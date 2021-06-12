#pragma once


#ifndef ITEM_H
#define ITEM_H

#include "obj_reader.h"
#include "texture.h"
#include "shader.h"
#include "shadowManager.h"

struct ItemInitializationInfo
{
	ItemInitializationInfo(std::string filename, std::string colorTex, std::string metallicTex) : 
		obj_file_name(filename), 
		colorTexName(colorTex),
		metallicTexName(metallicTex)
	{}
	std::string obj_file_name;
	std::string colorTexName;
	std::string metallicTexName;
};


class Item : public Object
{
public:
	Item(ItemInitializationInfo info);
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
	glm::vec3 position_ = glm::vec3(0.0, 0.0, -2.0);
	ShaderProgram depthPassShader_;
};

#endif // ITEM_H