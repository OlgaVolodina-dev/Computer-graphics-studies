

#ifndef SHADOW_MANAGER_H
#define SHADOW_MANAGER_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "GL/glew.h"
#include <vector>
#include "UBO.h"


class Object {
public:
	// here will be rules of interface for simple shader drawings
	virtual void DrawSimple() = 0;
};

class ShadowManager final : public IUBOListener
{
public:
	ShadowManager();
	void SetDirectionalLight();
	void ShadowPrepass(std::vector<Object *> &, bool);
	GLuint GetDepthTexture();
	std::size_t GetUBOSize() override;
	void SetData(std::size_t offset) override;
private:
	GLuint UBO_;
	glm::mat4 lightSpaceMatrix_;
	GLuint depthTexture_;
	GLuint depthFBO_;
	glm::vec3 direction_;
};

#endif
