#include "camera.h"
#include <iostream>
Camera::Camera(bool quatertions):
	quatertions_(quatertions),
	direction_(glm::vec3(0.0f, 0.0f, -1.0f)),
	position_(glm::vec3(0.0f, 0.0f, 3.0f))
{
	glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
	right_ = glm::normalize(glm::cross(direction_, up));
	up_ = glm::cross(direction_, right_);
	rotation_ = glm::mat4(1.0);
	translation_ = glm::translate(glm::mat4(1.0), -position_);
}

//TOOD create UBO
void Camera::BindView(const GLuint shader)
{
	view_ = rotation_ * translation_;

	int modelLoc = glGetUniformLocation(shader, "view");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(view_));
	int projectionLoc = glGetUniformLocation(shader, "projection");
	//TODO dirty
	glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);
	glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

}

//TODO take enum class
void Camera::ProcessKeyboard(Translation e)
{
	const float cameraSpeed = 0.01f;
	switch (e)
	{
	case FORWARD:
		position_ += cameraSpeed * (direction_);
		break;
	case BACKWARD:
		position_ -= cameraSpeed * (direction_);
		break;
	case LEFT:
		position_ -= right_* cameraSpeed;
		break;
	case RIGHT:
		position_ += right_ * cameraSpeed;
		break;
	default:
		break;
	} 
	translation_ = glm::translate(glm::mat4(1.0), -position_);

}

void Camera::ProcessMouse(float pitch, float yaw) {
	if (!quatertions_) {
		direction_.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
		direction_.y = sin(glm::radians(pitch));
		direction_.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
		direction_ = glm::normalize(direction_);
	} else {
		glm::quat aroundY = glm::angleAxis(glm::radians(-yaw), glm::vec3(0, 1, 0));
		glm::quat aroundX = glm::angleAxis(glm::radians(pitch), glm::vec3(1, 0, 0));
		glm::quat orientation = aroundY * aroundX;

		glm::quat qF = orientation * glm::quat(0, 0, 0, -1) * glm::conjugate(orientation);
		direction_ = { qF.x, qF.y, qF.z };
		glm::quat qFUp = orientation * glm::quat(0, 0, 1, 0) * glm::conjugate(orientation);
		up_ = { qFUp.x, qFUp.y, qFUp.z };
		right_ = glm::normalize(glm::cross(direction_, up_));
		
		glm::quat reverseOrient = glm::conjugate(orientation);
		rotation_ = glm::mat4_cast(reverseOrient);
	}
}

