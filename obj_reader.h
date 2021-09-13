#pragma once

#ifndef OBJ_READER_H
#define OBJ_READER_H
#include <string>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

struct BoundingBox {
	float maxX = 0.0;
	float maxY = 0.0;
	float maxZ = 0.0;
	float minX = FLT_MAX;
	float minY = FLT_MAX;
	float minZ = FLT_MAX;
};

struct Vertex
{
	glm::vec3 position;
	glm::vec2 texCoord;
	glm::vec3 normal;
};

struct MtlData
{
	float Ns;
	glm::vec3 Ka;
	glm::vec3 Kd;
	glm::vec3 Ks;
	float Ni;
	float d;
	uint8_t illum;
};

struct MeshInfo
{
	std::string name;
	size_t index;
	MtlData mtlData;
};

struct Model
{
	std::vector<Vertex> vertices;
	std::vector<MeshInfo> meshInfo;
};

class ObjReader
{
public:
	static void ReadObj(std::string &filename, Model & vertices, BoundingBox& bb);
	static void ReadMTL(std::string& filename, Model& model);

};

#endif // OBJ_READER_H