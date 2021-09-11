#include "engine.h"
#include "utils.h"
#include <iostream>

Engine::Engine() :
	camera_(),
	globalUBO_(),
	quadProgram(QUAD_VERT, QUAD_FRAG)
{
	ItemSetting();
	terrainHandler_.Init();
	// Order matters!
	globalUBO_.RegisterListener(camera_);
	globalUBO_.Setup();
	// for (auto& item : items_) {
	// 	item->LoadItem();
	// }
	glGenVertexArrays(1, &emptyVAO_);
	glGenFramebuffers(1, &msaaFBO_);
	CreateTextures();
}

void Engine::SetWindowSize(int width, int height)
{
	if (width == scr_width && height == scr_height) {
		return;
	}
	std::cout << width << height << std::endl;
	camera_.UpdateWindowSize(width, height);
	//water.UpdateWindowSize(width, height);
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
}

void Engine::Draw()
{	
	glViewport(0, 0, scr_width, scr_height);
	camera_.Commit();
	terrainHandler_.Process(camera_);
	glEnable(GL_DEPTH_TEST);

	// water.PreRender(items_, camera_);

	glBindFramebuffer(GL_FRAMEBUFFER, msaa_? msaaFBO_ : 0 );
	
	glClearColor(0.005f, 0.015f, 0.026f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	globalUBO_.UpdateUBO();
	
	// for (auto& item : items_) {
	// 	item->Draw();
	// }
	terrainHandler_.Draw();
	// water.Draw();

	if (msaa_) {
		glBindFramebuffer(GL_READ_FRAMEBUFFER, msaaFBO_);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
		glBlitFramebuffer(0, 0, scr_width, scr_height, 0, 0, scr_width, scr_height, GL_COLOR_BUFFER_BIT, GL_NEAREST);
	}

}





