#include "engine.h"
#include <iostream>

Engine::Engine():
	cube_(),
	camera_(),
	globalUBO_(), 
	terrain_(), 
	shadowManager_()
{
	// Order matters!
	globalUBO_.RegisterListener(camera_);
	lightSources_.push_back(LightSource(glm::vec3(0.0f, 0.0f, 5.0f)));
	lightSources_.push_back(LightSource(glm::vec3(0.0f, 0.0f, -5.0f)));
	// if lightSource will be deleted?
	for (auto& lightSource : lightSources_) {
		globalUBO_.RegisterListener(lightSource);
	}
	globalUBO_.RegisterListener(shadowManager_);

	globalUBO_.Setup();

}


void Engine::Draw()
{

	glEnable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.05f, 0.15f, 0.26f, 1.0f);

	shadowManager_.SetDirectionalLight();
	std::vector<Object*> objects;
	objects.push_back(&terrain_);
	objects.push_back(&cube_);
	shadowManager_.ShadowPrepass(objects);

	globalUBO_.UpdateUBO();
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, shadowManager_.GetDepthTexture());
	cube_.Draw(camera_);
	terrain_.Draw();
	for (auto& lightSource : lightSources_) {
		lightSource.Draw();
	}
}


