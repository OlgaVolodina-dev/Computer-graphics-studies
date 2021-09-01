#include "shadowManager.h"
#include <algorithm>
#include <iostream>
#define SPLIT_NUMBER 3


ShadowManager::ShadowManager(glm::mat4 projview) 
{
	glGenFramebuffers(1, &depthFBO_);

	CreateTextures();

	glGenBuffers(1, &UBO_);
	glBindBuffer(GL_UNIFORM_BUFFER, UBO_);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), NULL, GL_STREAM_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	
	glBindBufferRange(GL_UNIFORM_BUFFER, 0,
		UBO_, 0, sizeof(glm::mat4));


	float near_plane = 1.0f, far_plane = 13.0f;
	const float boundary = 9.5;
	lightProjection_ = glm::ortho(-boundary, boundary, -boundary, boundary, near_plane, far_plane);
	glm::vec3 position = glm::vec3(0.0f, 4.0f, 0.0f);
	glm::vec3 direction = glm::vec3(0.0f, -1.0f, 0.0f);
	direction_ = position - direction;
	glm::vec3 up = -glm::cross(glm::vec3(1.0, 0.0, 0.0), position - direction);
	lightView = glm::lookAt(position,
		position + direction,
		up);

	lightSpaceMatrix_ = lightProjection_ * lightView;
}

void ShadowManager::CascadeMatrixes(Camera& camera, std::vector<std::shared_ptr<Item>>& objects)
{
	float z_fars[SPLIT_NUMBER+1] = { 0.0, 0.99036, 0.99678, 1.0 };
	
	glm::mat4 projView = camera.GetProjView();
	glm::mat4 iPV = glm::inverse(projView);
	lightProjMatrixes.clear();
	for (int i = 1; i < SPLIT_NUMBER+1; ++i) {
		std::vector<glm::vec4> corners{
			iPV * glm::vec4(-1.2, 1.2,  0.0, 1.0),
			iPV * glm::vec4(1.2, 1.2,   0.0, 1.0),
			iPV * glm::vec4(-1.2, -1.2, 0.0, 1.0),
			iPV * glm::vec4(1.2, -1.2,  0.0, 1.0),

			iPV * glm::vec4(-1.2,  1.2,  z_fars[i], 1.0),
			iPV * glm::vec4(1.2,  1.2,  z_fars[i], 1.0),
			iPV * glm::vec4(-1.2, -1.2,  z_fars[i], 1.0),
			iPV * glm::vec4(1.2, -1.2,  z_fars[i], 1.0)
		};

		float minX = FLT_MAX, maxX = -FLT_MAX, minY = FLT_MAX, maxY = -FLT_MAX, minZ = FLT_MAX, maxZ = -FLT_MAX;
		for (auto& corner : corners) {
			corner = corner / glm::vec4(corner.w);
			glm::vec4 pos = corner;
			maxX = std::max(pos.x, maxX);
			maxY = std::max(pos.y, maxY);
			maxZ = std::max(pos.z, maxZ);
			minX = std::min(pos.x, minX);
			minY = std::min(pos.y, minY);
			minZ = std::min(pos.z, minZ);
		}
		//std::cout << maxX << " " << maxZ << " " << minX << " " << minZ << std::endl;
		//std::cout << maxY << std::endl;
		maxY = 5.0F;
		glm::vec3 position = glm::vec3((maxX + minX) / 2,  maxY, (minZ + maxZ) / 2);
		glm::vec3 direction = glm::vec3(0.0, -1.0, 0.0);
		glm::mat4 lightView_local = glm::lookAt(position, position + direction, glm::vec3(1.0, 0.0, 0.0));

		float w = fabs(maxX - minX) / 2.0;
		float h = fabs(maxZ - minZ) / 2.0;
		w = std::max(w, h);

		glm::mat4 proj = glm::ortho(-w, w, -w, w, 1.0F, abs(maxY - minY));
		//std::cout << w << " " << maxY - minY << " " << position.y << std::endl ;

		lightProjMatrixes.push_back(proj * lightView_local);
		//lightSpaceMatrix_ = proj * lightView_local;
	}
}


void ShadowManager::CreateTextures()
{
	
	for (int i = 0; i < SPLIT_NUMBER; ++i) {
		if (vsmTexture_[i]) {
			glDeleteTextures(1, &(vsmTexture_[i]));
		}
		glGenTextures(1, &(vsmTexture_[i]));
		glBindTexture(GL_TEXTURE_2D, vsmTexture_[i]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RG32F,
			scr_width, scr_height, 0, GL_RGBA, GL_FLOAT, NULL);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		if (i != 0) {
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		else {
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		}
	}

	if (depthTexture_) {
		glDeleteTextures(1, &depthTexture_);
	}
	glGenTextures(1, &depthTexture_);
	glBindTexture(GL_TEXTURE_2D, depthTexture_);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
		scr_width, scr_height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}

void ShadowManager::SetDirectionalLight()
{
	glBindBuffer(GL_UNIFORM_BUFFER, UBO_);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(lightSpaceMatrix_));
	
}

std::size_t ShadowManager::GetUBOSize()
{
	return sizeof(glm::mat4) * 3 + sizeof(glm::vec3);
}

void ShadowManager::SetData(std::size_t offset) 
{
	glBufferSubData(GL_UNIFORM_BUFFER, offset, sizeof(glm::mat4) * 3, lightProjMatrixes.data() );
	glBufferSubData(GL_UNIFORM_BUFFER, offset + sizeof(glm::mat4) * 3, sizeof(glm::vec3), glm::value_ptr(direction_));
}

GLuint ShadowManager::GetDepthTexture()
{
	return depthTexture_;
}

void ShadowManager::UpdateWindowSize(int width, int height)
{
	scr_width = width;
	scr_height = height;
	CreateTextures();
}

void ShadowManager::ShadowPrepass(std::vector<std::shared_ptr<Item>>& objects, bool showDepth)
{
	for (int i = 0; i < SPLIT_NUMBER; ++i) {
		glBindFramebuffer(GL_FRAMEBUFFER, depthFBO_);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, vsmTexture_[i], 0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTexture_, 0);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);


		glBindBuffer(GL_UNIFORM_BUFFER, UBO_);
		glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(lightProjMatrixes[i]));
		glBindBufferRange(GL_UNIFORM_BUFFER, 0, UBO_, 0, sizeof(glm::mat4));
		glBindBufferBase(GL_UNIFORM_BUFFER, 0, UBO_);


		glViewport(0, 0, scr_width, scr_height);
		GLint fbo = showDepth ? 0 : depthFBO_;
		glBindFramebuffer(GL_FRAMEBUFFER, fbo);
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		for (auto object : objects) {
			object->DrawSimple();
		}
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glViewport(0, 0, scr_width, scr_height);
		if (showDepth) {
			return;
		}
		if (i != 0) {
			glBindTexture(GL_TEXTURE_2D, vsmTexture_[i]);
			glGenerateMipmap(GL_TEXTURE_2D);

		}
	}
}


