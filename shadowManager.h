

#ifndef SHADOW_MANAGER_H
#define SHADOW_MANAGER_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "GL/glew.h"
#include <vector>
#include "UBO.h"
#include "Item.h"
#include "camera.h"
#include <memory>

class ShadowManager final : public IUBOListener
{
public:
	ShadowManager(glm::mat4 projview);
	void SetDirectionalLight();
	void ShadowPrepass(std::vector<std::unique_ptr<Item>>& objs, bool);
	GLuint GetDepthTexture();
	GLuint* GetVSMTexture() { return vsmTexture_; }
	std::size_t GetUBOSize() override;
	void SetData(std::size_t offset) override;
	void CascadeMatrixes(Camera& camera, std::vector<std::unique_ptr<Item>>& objects);
	constexpr static const int SPLIT_NUMBER = 3;
	void UpdateWindowSize(int width, int height);
private:
	void CreateTextures();
	glm::mat4 lightProjection_;
	GLuint UBO_;
	glm::mat4 lightSpaceMatrix_;
	GLuint depthTexture_ = 0;
	GLuint vsmTexture_[SPLIT_NUMBER] = { 0, 0, 0 };
	GLuint depthFBO_;
	glm::vec3 direction_;
	glm::mat4 lightView;
	std::vector<glm::mat4> lightProjMatrixes;
	bool cascade_shadows = true;
	int scr_width = 800, scr_height=600;
};

#endif
