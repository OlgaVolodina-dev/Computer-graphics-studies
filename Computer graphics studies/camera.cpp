#include "camera.h"
#include <iostream>

Camera::Camera():
	IUBOListener()
{
	glm::vec3 front;
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	cameraFront = glm::normalize(front);
	view_ = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
	projection_ = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);
}

Camera::~Camera()
{}

//TODO matrix in stuct
void Camera::SetData(std::size_t offset)
{
	glBufferSubData(GL_UNIFORM_BUFFER, offset, sizeof(glm::mat4), glm::value_ptr(frame_view));
	glBufferSubData(GL_UNIFORM_BUFFER, offset + sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(projection_));
	glBufferSubData(GL_UNIFORM_BUFFER, offset + sizeof(glm::mat4) * 2, sizeof(glm::vec4), glm::value_ptr(frame_position));
}

glm::mat4 Camera::GetProjView()
{
	return projection_ * frame_view;
}

std::size_t Camera::GetUBOSize()
{
	return sizeof(glm::mat4) * 2 + sizeof(glm::vec4);
}

void Camera::Commit()
{
	Resolve();
	frame_view = view_;
	frame_pitch = pitch;
	frame_yaw = yaw;
	frame_position = cameraPos;
}

void Camera::Resolve()
{
	view_ = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
}

glm::mat4 Camera::GetCustomCamera(glm::vec3 position, float yaw, float pitch)
{
	glm::vec3 front;
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	glm::mat4 view_local = glm::lookAt(position, position + front, cameraUp);
	return projection_ * view_local;
	
}

//TODO take enum class
void Camera::ProcessKeyboard(Translation e)
{
	const float cameraSpeed = 0.05f;
	switch (e)
	{
	case FORWARD:
		cameraPos += cameraSpeed * cameraFront;
		break;
	case BACKWARD:
		cameraPos -= cameraSpeed * cameraFront;
		break;
	case LEFT:
		cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
		break;
	case RIGHT:
		cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
		break;
	default:
		break;
	} 
}

void Camera::ProcessMouse(float xoffset, float yoffset) {
	yaw += xoffset;
	pitch += yoffset;
	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;
	glm::vec3 front;
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	cameraFront = glm::normalize(front);
}

