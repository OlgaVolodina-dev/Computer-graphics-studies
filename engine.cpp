#include "engine.h"
#include "utils.h"
#include <iostream>


Engine::Engine() :
	camera_(),
	globalUBO_(),
	shadowManager_(camera_.GetProjView()),
	quadProgram(QUAD_VERT, QUAD_FRAG),
	bloomPreprocessingProgram_(QUAD_VERT, BLOOM_PREPROCESSING_FRAG),
	bloomPostprocessingProgram_(QUAD_VERT, BLOOM_POSTPROCESSING_FRAG),
	gaussBlur(), 
	lightSources_{ LightSource(glm::vec3(0.0f, 0.0f, 5.0f)) , LightSource(glm::vec3(-3.0f, 1.0f, -0.0f))}
{
	ItemSetting();

	// Order matters!
	globalUBO_.RegisterListener(camera_);
	// if lightSource will be deleted?
	for (auto& lightSource : lightSources_) {
		globalUBO_.RegisterListener(lightSource);
	}
	globalUBO_.RegisterListener(shadowManager_);

	globalUBO_.Setup();
	for (auto& item : items_) {
		item->LoadItem();
	}
	glGenFramebuffers(1, &postProcessFBO_);
	glGenFramebuffers(1, &bloomFBO_);
	glGenVertexArrays(1, &emptyVAO_);
	glGenFramebuffers(1, &msaaFBO_);
	CreateTextures();
}

void Engine::SetWindowSize(int width, int height)
{
	if (width == scr_width && height == scr_height) {
		return;
	}

	camera_.UpdateWindowSize(width, height);
	shadowManager_.UpdateWindowSize(width, height);
	water.UpdateWindowSize(width, height);
	gaussBlur.UpdateWindowSize(width, height);
	scr_width = width;
	scr_height = height;
	CreateTextures();
}

void Engine::CreateTextures()
{
	GLuint msaaTex = 0;
	GLint samples = 4;

	glGenTextures(1, &msaaTex);
	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, msaaTex);
	glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, GL_RGBA16F, scr_width, scr_height, GL_TRUE);
	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);

	GLuint msaaDepth = 0;
	glGenRenderbuffers(1, &msaaDepth);
	glBindRenderbuffer(GL_RENDERBUFFER, msaaDepth);
	glRenderbufferStorageMultisample(GL_RENDERBUFFER, samples, GL_DEPTH_COMPONENT, scr_width, scr_height);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	glBindFramebuffer(GL_FRAMEBUFFER, msaaFBO_);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, msaaTex, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, msaaDepth);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	CHECK_OPENGL_ERROR();

	if (postProcessTex_) {
		glDeleteTextures(1, &postProcessTex_);
	}
	glGenTextures(1, &postProcessTex_);
	glBindTexture(GL_TEXTURE_2D, postProcessTex_);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, scr_width, scr_height, 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glBindTexture(GL_TEXTURE_2D, 0);

	if (postProcessDSTex_) {
		glDeleteTextures(1, &postProcessDSTex_);
	}
	glGenTextures(1, &postProcessDSTex_);
	glBindTexture(GL_TEXTURE_2D, postProcessDSTex_);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, scr_width, scr_height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glBindTexture(GL_TEXTURE_2D, 0);


	glBindFramebuffer(GL_FRAMEBUFFER, postProcessFBO_);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, postProcessTex_, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, postProcessDSTex_, 0);
	CHECK_FRAMEBUFFER_STATUS();

	if (bloomTex_) {
		glDeleteTextures(1, &bloomTex_);
	}
	glGenTextures(1, &bloomTex_);
	glBindTexture(GL_TEXTURE_2D, bloomTex_);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, scr_width, scr_height, 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glBindTexture(GL_TEXTURE_2D, 0);
	glGenerateMipmap(GL_TEXTURE_2D);


	glBindFramebuffer(GL_FRAMEBUFFER, bloomFBO_);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, bloomTex_, 0);
	CHECK_FRAMEBUFFER_STATUS();
	CHECK_OPENGL_ERROR();
}

void Engine::Draw()
{	
	glViewport(0, 0, scr_width, scr_height);
	camera_.Commit();
	shadowManager_.CascadeMatrixes(camera_, items_);
	glEnable(GL_DEPTH_TEST);


	shadowManager_.ShadowPrepass(items_, showDepth_);
	if (showDepth_) {
		return;
	}
	gaussBlur.Blur(shadowManager_.GetVSMTexture()[0], 1, 1, false);
	gaussBlur.Blur(shadowManager_.GetVSMTexture()[1], 1, 1, false);
	//gaussBlur.Blur(shadowManager_.GetVSMTexture()[2], 1, 1, false);

	water.PreRender(items_, camera_);

	if (msaa_) {
		glBindFramebuffer(GL_FRAMEBUFFER, msaaFBO_);
	}
	

	glClearColor(0.005f, 0.015f, 0.026f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	globalUBO_.UpdateUBO();
	
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, shadowManager_.GetVSMTexture()[0]);
	glActiveTexture(GL_TEXTURE7);
	glBindTexture(GL_TEXTURE_2D, shadowManager_.GetVSMTexture()[1]);
	glActiveTexture(GL_TEXTURE8);
	glBindTexture(GL_TEXTURE_2D, shadowManager_.GetVSMTexture()[2]);
	for (auto& item : items_) {
		item->Draw();
	}
	water.Draw();
	for (auto& lightSource : lightSources_) {
		lightSource.Draw();
	}

	if (msaa_) {
		glBindFramebuffer(GL_READ_FRAMEBUFFER, msaaFBO_);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, postProcessFBO_);
		glBlitFramebuffer(0, 0, scr_width, scr_height, 0, 0, scr_width, scr_height, GL_COLOR_BUFFER_BIT, GL_NEAREST);
	}

	glBindFramebuffer(GL_FRAMEBUFFER, bloomFBO_);
	glDisable(GL_DEPTH_TEST);
	glUseProgram(bloomPreprocessingProgram_);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, postProcessTex_);
	glBindVertexArray(emptyVAO_);
	glDrawArrays(GL_TRIANGLES, 0, 3);

	gaussBlur.Blur(bloomTex_, 0, 5, false);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glDisable(GL_DEPTH_TEST);
	glUseProgram(bloomPostprocessingProgram_);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, postProcessTex_);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, bloomTex_);
	glUniform1f(0, 0.5);
	glBindVertexArray(emptyVAO_);
	glDrawArrays(GL_TRIANGLES, 0, 3);

	//glBindFramebuffer(GL_FRAMEBUFFER, 0);
	//glUseProgram(quadProgram);
	//glActiveTexture(GL_TEXTURE0);
	//glBindTexture(GL_TEXTURE_2D, shadowManager_.GetVSMTexture()[2]);
	//glBindVertexArray(emptyVAO_);
	//glDrawArrays(GL_TRIANGLES, 0, 3);

}





