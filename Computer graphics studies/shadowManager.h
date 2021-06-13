

#ifndef SHADOW_MANAGER_H
#define SHADOW_MANAGER_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "GL/glew.h"
#include <vector>
#include "UBO.h"
#include "Item.h"

class ShadowManager final : public IUBOListener
{
public:
	ShadowManager(glm::mat4 projview);
	void SetDirectionalLight();
	void ShadowPrepass(std::vector<Item *> &, bool);
	GLuint GetDepthTexture();
	GLuint GetVSMTexture() { return vsmTexture_; }
	std::size_t GetUBOSize() override;
	void SetData(std::size_t offset) override;
private:
	glm::mat4 lightProjection_;
	GLuint UBO_;
	glm::mat4 lightSpaceMatrix_;
	GLuint depthTexture_;
	GLuint vsmTexture_;
	GLuint depthFBO_;
	glm::vec3 direction_;
};

#endif
