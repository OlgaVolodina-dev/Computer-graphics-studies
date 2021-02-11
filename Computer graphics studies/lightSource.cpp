#include "lightSource.h"
#include <iostream>



LightSource::LightSource(glm::vec3 position):
	IUBOListener(),
	position_(position)
{}

std::size_t LightSource::GetUBOSize()
{
	return sizeof(glm::vec3);
}

void LightSource::SetData(std::size_t offset)
{
	glBufferSubData(GL_UNIFORM_BUFFER, offset, sizeof(glm::vec3), glm::value_ptr(position_));
}