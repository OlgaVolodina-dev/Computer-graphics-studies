
#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "GL/glew.h"
#include "UBO.h"

class Camera final: public IUBOListener
{
public:
	enum Translation {
	FORWARD, 
	BACKWARD, 
	LEFT,
	RIGHT
	};

	Camera();
	Camera(Camera&) = delete;
	Camera(Camera&&) = delete;
	Camera& operator=(const Camera&) = delete;
	Camera& operator=(const Camera&&) = delete;
	~Camera() override;
	void ProcessKeyboard(Translation e);
	void ProcessMouse(float pitch, float yaw);
	std::size_t GetUBOSize() override;
	void SetData(std::size_t offset) override;
private:
	void Resolve();
	bool quatertions_ = false;
	glm::vec3 position_;
	glm::vec3 direction_;
	glm::mat4 view_;
	glm::mat4 rotation_;
	glm::mat4 translation_;
	glm::mat4 projection_;
	glm::vec3 right_;
	glm::vec3 up_;

};
#endif // CAMERA_H

