#pragma once

#ifndef WATER_SIMULATION_H
#define WATER_SIMULATION_H

#include "camera.h"
#include "Item.h"
#include "texture.h"
#include "shader.h"
#include <chrono>

class WaterSimulation
{
public:
	WaterSimulation();
	~WaterSimulation();
	void PreRender(std::vector<Item*> items, Camera& camera);
	void Draw();
	void UpdateWindowSize(int, int);
private:
	GLuint reflectionTex = 0;
	GLuint refractionTex = 0;
	void CreateTextures();
	Texture dudv;
	Texture normalTex;
	GLuint depthTexture = 0;
	GLuint fbo;
	ShaderProgram simpleColorShader;
	Item* water = nullptr;
	float moveFactor = 0.0;
	std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
	bool firstStartTime = true;
	int scr_width = 800, scr_height = 600;
};

#endif// WATER_SIMULATION