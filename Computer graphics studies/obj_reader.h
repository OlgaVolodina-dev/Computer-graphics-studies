#pragma once

#ifndef OBJ_READER_H
#define OBJ_READER_H
#include <string>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
struct Vertex
{
	glm::vec3 position;
	glm::vec2 texCoord;
	glm::vec3 normal;
};

class ObjReader
{
public:
	static void ReadObj(std::string &filename, std::vector<Vertex> & vertices);
};

#endif // OBJ_READER_H