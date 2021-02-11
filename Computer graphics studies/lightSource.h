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
		float constant = 1.0;
		float linear = 0.09f;
		float quadratic = 0.032f;
	};
	Attenuation attenuation;
	glm::vec3 position_;
	GLuint VAO_ = 0U;
	ShaderProgram shader_;
	glm::vec3 color_;

};

#endif // !LIGHTSOURCE_H
