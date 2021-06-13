#pragma once

#ifndef LIGHTSOURCE_H
#define LIGHTSOURCE_H
#include "UBO.h"
#include "shader.h"

class LightSource : public IUBOListener
{
public:
	LightSource(glm::vec3 position);
	std::size_t GetUBOSize() override;
	void SetData(std::size_t) override;
	void Draw();
private:
	struct Attenuation {
		float constant = 2.0;
		float linear = 0.05f;
		float quadratic = 0.022f;
	};
	glm::vec3 lightColor_ = glm::vec3(1.0, 0.0, 0.0);
	Attenuation attenuation;
	glm::vec3 position_;
	GLuint VAO_ = 0U;
	ShaderProgram shader_;
	glm::vec3 color_;

};

#endif // !LIGHTSOURCE_H
