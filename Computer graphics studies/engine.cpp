#include "engine.h"
#include <iostream>

Engine::Engine():
	cube_(),
	camera_(),
	globalUBO_()
{
	// Order matters!
	globalUBO_.RegisterListener(camera_);
	lightSources_.push_back(LightSource(glm::vec3(0.0f, 0.0f, 1.0f)));
	// if lightSource will be deleted?
	for (auto& lightSource : lightSources_) {
		globalUBO_.RegisterListener(lightSource);
	}
	globalUBO_.Setup();

}


void Engine::Draw()
{
	globalUBO_.UpdateUBO();
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	cube_.Draw(camera_);
}


