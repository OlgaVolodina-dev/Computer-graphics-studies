#include "engine.h"



void Engine::ItemSetting()
{
	ItemInitializationInfo terrainInfo;
	terrainInfo.obj_file_name = "obj/Landscape/terrain.obj";
	terrainInfo.colorTexName = "obj/Landscape/terrain_BaseColor.png";
	terrainInfo.vert = PHONG_VERT;
	terrainInfo.frag = PHONG_FRAG;
	terrainInfo.shadowCaster = true;

	Item terrain(terrainInfo);

	ItemModelPositionInfo& terrainModelInfo = terrain.GetWriteableModelInfo();
	terrainModelInfo.scale_ = { 0.01 };
	terrainModelInfo.position_ = { glm::vec3(0.0, -11.0, 0.0) };

	items_.push_back(terrain);

	ItemInitializationInfo hornInfo;
	hornInfo.obj_file_name = "obj/Horn/Horn.obj";
	hornInfo.colorTexName = "obj/Horn/Horn_BaseColor.png";
	hornInfo.metallicTexName = "obj/Horn/Horn_Metallic.png";
	hornInfo.vert = PHONG_VERT;
	hornInfo.frag = HORN_FRAG;
	
	Item horn(hornInfo);

	ItemModelPositionInfo& hornModelInfo = horn.GetWriteableModelInfo();
	hornModelInfo.position_ = { glm::vec3(0.0, 0.0, 4.0) };
	hornModelInfo.scale_ = { 0.5 };

	items_.push_back(horn);
	
	ItemInitializationInfo cubeInfo;
	cubeInfo.obj_file_name = "obj/cube/cube.obj";
	cubeInfo.colorTexName = "obj/cube/cube_BaseColor.jpg";
	cubeInfo.vert = PHONG_VERT;
	cubeInfo.frag = PHONG_FRAG;
	cubeInfo.n_indices = 10;
	cubeInfo.colorTexGenerateMipmap = true;

	Item cube(cubeInfo);

	ItemModelPositionInfo& cubeModelInfo = cube.GetWriteableModelInfo();
	cubeModelInfo.position_ = {
		{glm::vec3(0.0f,  0.0f,  0.0f)},
		{glm::vec3(2.0f,  5.0f, -15.0f)},
		{glm::vec3(-1.5f, -2.2f, -2.5f)},
		{glm::vec3(-3.8f, -2.0f, -12.3f)},
		{glm::vec3(2.4f, -0.4f, -3.5f)},
		{glm::vec3(-1.7f,  3.0f, -7.5f)},
		{glm::vec3(1.3f, -2.0f, -2.5f)},
		{glm::vec3(1.5f,  2.0f, -2.5f)},
		{glm::vec3(1.5f,  0.2f, -1.5f)},
		{glm::vec3(-1.3f,  1.0f, -1.5f)}
	};
	cubeModelInfo.rotation_axis_.clear();
	cubeModelInfo.rotation_angle_.clear();
	cubeModelInfo.scale_.clear();
	for (int i = 0; i < cubeModelInfo.position_.size(); ++i) {
		cubeModelInfo.rotation_axis_.push_back(glm::vec3(1.0, 0.5, 1.0));
		cubeModelInfo.rotation_angle_.push_back(float(i * 5));
		cubeModelInfo.scale_.push_back(1.0);
	}

	items_.push_back(cube);






	
	
}