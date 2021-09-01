#pragma once

#ifndef LIGHTSOURCE_H
#define LIGHTSOURCE_H
#include "UBO.h"
#include "shader.h"
#include <chrono>

class LightSource : public IUBOListener
{
public:
	LightSource(glm::vec3 position);
	std::size_t GetUBOSize() override;
	void SetData(std::size_t) override;
	void Draw();
private:
	void ResolvePosition();
	struct Attenuation {
		float constant = 1.8;
		float linear = 0.89f;
		float quadratic = 0.83f;
	};
	glm::vec3 lightColor_ = glm::vec3(5.0, 0.0, 0.0);
	Attenuation attenuation;
	glm::vec3 position_;
	GLuint VAO_ = 0U;
	ShaderProgram shader_;
	glm::vec3 color_;
	std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
	bool firstStartTime = true;
	float moveFactor =  (float)(rand()) / ((float)(RAND_MAX));
	float initial_y = 0.0;


};

#endif // !LIGHTSOURCE_H
