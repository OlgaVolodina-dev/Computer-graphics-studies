
#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "GL/glew.h"


class Camera
{
public:
	enum Translation {
	FORWARD, 
	BACKWARD, 
	LEFT,
	RIGHT
	};

	Camera(bool quaternions = false);
	void BindView(GLuint shader);
	void ProcessKeyboard(Translation e);
	void ProcessMouse(float pitch, float yaw);
private:
	bool quatertions_ = false;
	glm::vec3 position_;
	glm::vec3 direction_;
	glm::mat4 view_;
	glm::mat4 rotation_;
	glm::mat4 translation_;
	glm::vec3 right_;
	glm::vec3 up_;

};
#endif // CAMERA_H

