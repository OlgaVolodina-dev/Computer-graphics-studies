
#ifndef UBO_H
#define UBO_H

#include<vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "GL/glew.h"

struct GlobalUboStruct
{
	glm::mat4 view;
	glm::mat4 projection;
	glm::vec4 cameraPos;
	glm::mat4 lightSpaceMatrix[3];
	glm::vec4 lightDir;	
};

class IUBOListener
{
public:
	IUBOListener() = default;
	virtual ~IUBOListener() {};
	virtual std::size_t GetUBOSize();
	virtual void SetData(std::size_t offset);
};

class UBO
{
public:
	UBO();
	void RegisterListener(IUBOListener&);
	void UpdateUBO();
	void Setup();
private:
	GLuint buffer_ = 0U;
	GLuint programIndex_ = 0U;
	GLuint bindingIndex_ = 0U;
	std::size_t size_ = 0;
	std::vector<IUBOListener*> listeners_;
};

#endif
