#include "shadowManager.h"
const unsigned int SHADOW_WIDTH = 800, SHADOW_HEIGHT = 600;

ShadowManager::ShadowManager(glm::mat4 projview) 
{
	glGenFramebuffers(1, &depthFBO_);

	glGenTextures(1, &vsmTexture_);
	glBindTexture(GL_TEXTURE_2D, vsmTexture_);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RG32F,
		SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glGenerateMipmap(GL_TEXTURE_2D);



	glGenTextures(1, &depthTexture_);
	glBindTexture(GL_TEXTURE_2D, depthTexture_);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
		SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glBindFramebuffer(GL_FRAMEBUFFER, depthFBO_);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, vsmTexture_, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTexture_, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);


	glGenBuffers(1, &UBO_);
	glBindBuffer(GL_UNIFORM_BUFFER, UBO_);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), NULL, GL_STREAM_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	
	glBindBufferRange(GL_UNIFORM_BUFFER, 0,
		UBO_, 0, sizeof(glm::mat4));


	float near_plane = 1.0f, far_plane = 10.0f;
	lightProjection_ = glm::ortho(-7.0f, 7.0f, -7.0f, 7.0f, near_plane, far_plane);
	glm::vec3 position = glm::vec3(0.0f, 4.0f, 0.0f);
	glm::vec3 direction = glm::vec3(0.0f, -1.0f, 0.0f);
	direction_ = position - direction;
	glm::vec3 up = -glm::cross(glm::vec3(1.0, 0.0, 0.0), position - direction);
	glm::mat4 lightView = glm::lookAt(position,
		position + direction,
		up);

	lightSpaceMatrix_ = lightProjection_ * lightView;

	glm::mat4 projViewInverse = glm::inverse(projview);
	glm::mat4 inverseLightView = glm::inverse(lightView);

	

}

void ShadowManager::SetDirectionalLight()
{


	glBindBuffer(GL_UNIFORM_BUFFER, UBO_);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(lightSpaceMatrix_));

}

std::size_t ShadowManager::GetUBOSize()
{
	return sizeof(glm::mat4) + sizeof(glm::vec3);
}

void ShadowManager::SetData(std::size_t offset) 
{
	glBufferSubData(GL_UNIFORM_BUFFER, offset, sizeof(glm::mat4), glm::value_ptr(lightSpaceMatrix_));
	glBufferSubData(GL_UNIFORM_BUFFER, offset + sizeof(glm::mat4), sizeof(glm::vec3), glm::value_ptr(direction_));
}

GLuint ShadowManager::GetDepthTexture()
{
	return vsmTexture_;
}

void ShadowManager::ShadowPrepass(std::vector<Object*>& objects, bool showDepth)
{
	glBindBuffer(GL_UNIFORM_BUFFER, UBO_);
	
	glBindBufferRange(GL_UNIFORM_BUFFER, 0, UBO_, 0, sizeof(glm::mat4));
	glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
	GLint fbo = showDepth ? 0 : depthFBO_;
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	for (auto object : objects) {
		object->DrawSimple();
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glGenerateTextureMipmap(vsmTexture_);
	glViewport(0, 0, 800, 600);
}


