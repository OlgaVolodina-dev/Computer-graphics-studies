#include "engine.h"
#include "utils.h"
#include <iostream>
#define SCR_WIDTH  800
#define SCR_HEIGHT  600


Engine::Engine() :
	cube_(),
	terrain_(),
	camera_(),
	globalUBO_(),
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


	GLuint msaaTex = 0;
	GLint samples = 4;
	glGenFramebuffers(1, &msaaFBO_);
	glGenTextures(1, &msaaTex);
	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, msaaTex);
	glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, GL_RGB, SCR_WIDTH, SCR_HEIGHT, GL_TRUE);
	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);

	GLuint msaaDepth = 0;
	glGenRenderbuffers(1, &msaaDepth);
	glBindRenderbuffer(GL_RENDERBUFFER, msaaDepth);
	glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4, GL_DEPTH_COMPONENT16, SCR_WIDTH, SCR_HEIGHT);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	glBindFramebuffer(GL_FRAMEBUFFER, msaaFBO_);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, msaaTex, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, msaaDepth);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	CHECK_OPENGL_ERROR();
}

void Engine::Draw()
{
	shadowManager_.SetDirectionalLight();
	std::vector<Object*> objects;
	objects.push_back(&cube_);
	objects.push_back(&terrain_);
	shadowManager_.ShadowPrepass(objects, showDepth_);
	if (showDepth_) {
		return;
	}

	if (msaa_) {
		glBindFramebuffer(GL_FRAMEBUFFER, msaaFBO_);
	}
	glEnable(GL_DEPTH_TEST);

	glClearColor(0.005f, 0.015f, 0.026f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	globalUBO_.UpdateUBO();
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, shadowManager_.GetDepthTexture());
	cube_.Draw(camera_);
	terrain_.Draw();
	for (auto& lightSource : lightSources_) {
		lightSource.Draw();
	}

	if (msaa_) {
		glBindFramebuffer(GL_READ_FRAMEBUFFER, msaaFBO_);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
		glBlitFramebuffer(0, 0, SCR_WIDTH, SCR_HEIGHT, 0, 0, SCR_WIDTH, SCR_HEIGHT, GL_COLOR_BUFFER_BIT, GL_NEAREST);
	}
}





