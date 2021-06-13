#include "WaterSimulation.h"
#include <iostream>

#define SCR_WIDTH 800
#define SCR_HEIGHT 600
#define WAVE_SPEED  0.03f
WaterSimulation::WaterSimulation():
	dudv("obj/water/dudv.png"),
	normalTex("obj/water/water_Normal.png"),
	simpleColorShader(SIMPLE_COLOR_VERT, SIMPLE_COLOR_FRAG)
{
	glGenFramebuffers(1, &fbo);

	glGenTextures(1, &reflectionTex);
	glBindTexture(GL_TEXTURE_2D, reflectionTex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);

	glGenTextures(1, &refractionTex);
	glBindTexture(GL_TEXTURE_2D, refractionTex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);


	glGenTextures(1, &depthTexture);
	glBindTexture(GL_TEXTURE_2D, depthTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
		SCR_WIDTH, SCR_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);


	ItemInitializationInfo waterInfo;
	waterInfo.obj_file_name = "obj/water/water.obj";
	waterInfo.colorTexName = "obj/water/water_BaseColor.png";
	waterInfo.vert = WATER_FINAL_VERT;
	waterInfo.frag = WATER_FINAL_FRAG;
	waterInfo.shadowCaster = false;

	water = new Item(waterInfo);

	ItemModelPositionInfo& waterModelInfo = water->GetWriteableModelInfo();
	waterModelInfo.scale_ = { 50.0 };
	waterModelInfo.position_ = { glm::vec3(0.0, -10.0, 0.0) };

	water->LoadItem();

	//glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	

}

WaterSimulation::~WaterSimulation()
{
	std::cout << "~WaterSimulation" << std::endl;
	free(water);
}

void WaterSimulation::PreRender(std::vector<Item*> items, Camera& camera)
{
	glEnable(GL_CLIP_DISTANCE0);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, reflectionTex, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTexture, 0);
	glClearColor(0.005f, 0.015f, 0.026f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glm::mat4 projView = camera.GetProjView();
	glUseProgram(simpleColorShader);
	glUniformMatrix4fv(0, 1, GL_FALSE, glm::value_ptr(projView));
	glm::vec4 reflectionClip = glm::vec4(0.0, -1.0, 0.0, -10.0);
	glUniform4fv(1, 1, glm::value_ptr(reflectionClip));
	for (auto& item : items) {
		item->DrawSimpleColor();
	}

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, refractionTex, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTexture, 0);
	glClearColor(0.005f, 0.015f, 0.026f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	float y_diff = 2 * (camera.GetPosition()[1] - (-10));
	projView = camera.GetCustomCamera(
		camera.GetPosition() - glm::vec3(0.0, y_diff, 0.0),
		camera.GetYaw(),
		-camera.GetPitch()
	);
	glUseProgram(simpleColorShader);
	glUniformMatrix4fv(0, 1, GL_FALSE, glm::value_ptr(projView));
	glm::vec4 refractionClip = glm::vec4(0.0, 1.0, 0.0, 10.0);
	glUniform4fv(1, 1, glm::value_ptr(refractionClip));
	for (auto& item : items) {
		item->DrawSimpleColor();
	}
	glDisable(GL_CLIP_DISTANCE0);
}

void WaterSimulation::Draw() {
	auto timePoint = std::chrono::steady_clock::now();
	auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(timePoint - start);
	start = timePoint;
	moveFactor += (WAVE_SPEED * elapsed.count() / 1000.0) ;
	moveFactor  = moveFactor > 1.0 ? moveFactor - 1.0 : moveFactor;
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, reflectionTex);
	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, refractionTex);
	glProgramUniform1f(water->GetMainProgram(), 0, moveFactor);
	dudv.Use(5);
	normalTex.Use(6);
	water->Draw();

}