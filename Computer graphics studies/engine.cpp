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
	shadowManager_(),
	quadProgram(QUAD_VERT, QUAD_FRAG),
	bloomPreprocessingProgram_(QUAD_VERT, BLOOM_PREPROCESSING_FRAG),
	bloomPostprocessingProgram_(QUAD_VERT, BLOOM_POSTPROCESSING_FRAG), 
	item_(ItemInitializationInfo("obj/Horn/Horn.obj", "obj/Horn/Horn_BaseColor.png", "obj/Horn/Horn_Metallic.png"))
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
	item_.LoadItem();

	GLuint msaaTex = 0;
	GLint samples = 4;
	glGenFramebuffers(1, &msaaFBO_);
	glGenTextures(1, &msaaTex);
	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, msaaTex);
	glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, GL_RGBA16F, SCR_WIDTH, SCR_HEIGHT, GL_TRUE);
	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);

	GLuint msaaDepth = 0;
	glGenRenderbuffers(1, &msaaDepth);
	glBindRenderbuffer(GL_RENDERBUFFER, msaaDepth);
	glRenderbufferStorageMultisample(GL_RENDERBUFFER, samples, GL_DEPTH_COMPONENT, SCR_WIDTH, SCR_HEIGHT);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	glBindFramebuffer(GL_FRAMEBUFFER, msaaFBO_);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, msaaTex, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, msaaDepth);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	CHECK_OPENGL_ERROR();

	glGenTextures(1, &postProcessTex_);
	glBindTexture(GL_TEXTURE_2D, postProcessTex_);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);

	glGenTextures(1, &postProcessDSTex_);
	glBindTexture(GL_TEXTURE_2D, postProcessDSTex_);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SCR_WIDTH, SCR_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glBindTexture(GL_TEXTURE_2D, 0);

	glGenFramebuffers(1, &postProcessFBO_);
	glBindFramebuffer(GL_FRAMEBUFFER, postProcessFBO_);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, postProcessTex_, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, postProcessDSTex_, 0);
	GLenum err = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (err != GL_FRAMEBUFFER_COMPLETE) {
		std::cout << "Incomplete";
	}

	glGenTextures(1, &bloomTex_);
	glBindTexture(GL_TEXTURE_2D, bloomTex_);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);
	glGenerateMipmap(GL_TEXTURE_2D);

	glGenFramebuffers(1, &bloomFBO_);
	glBindFramebuffer(GL_FRAMEBUFFER, bloomFBO_);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, bloomTex_, 0);
	CHECK_FRAMEBUFFER_STATUS();
	CHECK_OPENGL_ERROR();

	glGenVertexArrays(1, &emptyVAO_);

}

void Engine::Draw()
{
	glEnable(GL_DEPTH_TEST);
	shadowManager_.SetDirectionalLight();
	std::vector<Object*> objects;
	objects.push_back(&cube_);
	objects.push_back(&terrain_);
	objects.push_back(&item_);
	shadowManager_.ShadowPrepass(objects, showDepth_);
	if (showDepth_) {
		return;
	}

	if (msaa_) {
		glBindFramebuffer(GL_FRAMEBUFFER, msaaFBO_);
	}

	glClearColor(0.005f, 0.015f, 0.026f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	globalUBO_.UpdateUBO();
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, shadowManager_.GetVSMTexture());
	cube_.Draw(camera_);
	terrain_.Draw();
	item_.Draw();
	for (auto& lightSource : lightSources_) {
		lightSource.Draw();
	}

	if (msaa_) {
		glBindFramebuffer(GL_READ_FRAMEBUFFER, msaaFBO_);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, postProcessFBO_);
		glBlitFramebuffer(0, 0, SCR_WIDTH, SCR_HEIGHT, 0, 0, SCR_WIDTH, SCR_HEIGHT, GL_COLOR_BUFFER_BIT, GL_NEAREST);
	}


	//glBindFramebuffer(GL_FRAMEBUFFER, bloomFBO_);
	//glDisable(GL_DEPTH_TEST);
	//glUseProgram(bloomPreprocessingProgram_);
	//glActiveTexture(GL_TEXTURE0);
	//glBindTexture(GL_TEXTURE_2D, postProcessTex_);
	//glBindVertexArray(emptyVAO_);
	//glDrawArrays(GL_TRIANGLES, 0, 3);

	//glBindTexture(GL_TEXTURE_2D, bloomTex_);
	//glGenerateMipmap(GL_TEXTURE_2D);


	//glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glDisable(GL_DEPTH_TEST);
	//glUseProgram(bloomPostprocessingProgram_);
	//glActiveTexture(GL_TEXTURE0);
	//glBindTexture(GL_TEXTURE_2D, postProcessTex_);
	//glActiveTexture(GL_TEXTURE1);
	//glBindTexture(GL_TEXTURE_2D, bloomTex_);
	//glUniform1f(0, 0.5);
	//glBindVertexArray(emptyVAO_);
	//glDrawArrays(GL_TRIANGLES, 0, 3);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glUseProgram(quadProgram);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, postProcessTex_);
	glBindVertexArray(emptyVAO_);
	glDrawArrays(GL_TRIANGLES, 0, 3);

}





