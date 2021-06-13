#include "engine.h"



void Engine::ItemSetting()
{
	ItemInitializationInfo terrainInfo;
	terrainInfo.obj_file_name = "obj/Landscape/terrain.obj";
	terrainInfo.colorTexName = "obj/Landscape/terrain_BaseColor.png";
	terrainInfo.scale = 0.01;
	terrainInfo.position_ = glm::vec3(glm::vec3(0.0, -11.0, 0.0));
	terrainInfo.vert = PHONG_VERT;
	terrainInfo.frag = PHONG_FRAG;
	terrainInfo.shadowCaster = false;

	Item terrain(terrainInfo);
	items_.push_back(terrain);

	ItemInitializationInfo hornInfo;
	hornInfo.obj_file_name = "obj/Horn/Horn.obj";
	hornInfo.colorTexName = "obj/Horn/Horn_BaseColor.png";
	hornInfo.metallicTexName = "obj/Horn/Horn_Metallic.png";
	hornInfo.scale = 0.5;
	hornInfo.position_ = glm::vec3(glm::vec3(0.0, 0.0, 4.0));
	hornInfo.vert = PHONG_VERT;
	hornInfo.frag = HORN_FRAG;


	Item horn(hornInfo);
	items_.push_back(horn);
	

	//std::vector<glm::vec3> cubePositions = {
	//	{glm::vec3(0.0f,  0.0f,  0.0f)},
	//	{glm::vec3(2.0f,  5.0f, -15.0f)},
	//	{glm::vec3(-1.5f, -2.2f, -2.5f)},
	//	{glm::vec3(-3.8f, -2.0f, -12.3f)},
	//	{glm::vec3(2.4f, -0.4f, -3.5f)},
	//	{glm::vec3(-1.7f,  3.0f, -7.5f)},
	//	{glm::vec3(1.3f, -2.0f, -2.5f)},
	//	{glm::vec3(1.5f,  2.0f, -2.5f)},
	//	{glm::vec3(1.5f,  0.2f, -1.5f)},
	//	{glm::vec3(-1.3f,  1.0f, -1.5f)}
	//};
	//
	//ItemInitializationInfo cubeInfo;
	//cubeInfo.obj_file_name = "obj/cube/cube.obj";
	//cubeInfo.colorTexName = "obj/cube/cube_BaseColor.jpg";
	//cubeInfo.position_ = glm::vec3(glm::vec3(0.0, 0.0, 4.0));
	//cubeInfo.vert = PHONG_VERT;
	//cubeInfo.frag = HORN_FRAG;
	//cubeInfo.rotation_axis = glm::vec3(1.0, 0.5, 1.0);

	//for (int i = 0; i < cubePositions.size(); ++i) {
	//	cubeInfo.position_ = cubePositions[i];
	//	cubeInfo.rotation_angle = float(i * 5);
	//	Item cube(cubeInfo);
	//	items_.push_back(cube);
	//}

	
	
}