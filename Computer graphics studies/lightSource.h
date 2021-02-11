#pragma once

#ifndef LIGHTSOURCE_H
#define LIGHTSOURCE_H
#include "UBO.h"

class LightSource : public IUBOListener
{
public:
	LightSource(glm::vec3 position);
	std::size_t GetUBOSize() override;
	void SetData(std::size_t) override;
private:
	glm::vec3 position_;
};

#endif // !LIGHTSOURCE_H
