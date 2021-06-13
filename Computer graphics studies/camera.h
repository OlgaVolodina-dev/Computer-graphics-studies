
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
	void ProcessMouse(float xoffset, float yoffset);
	std::size_t GetUBOSize() override;
	void SetData(std::size_t offset) override;
	glm::mat4 GetProjView();
	float GetPitch() { return frame_pitch; }
	float GetYaw() { return frame_yaw; }
	glm::vec3 GetPosition() { return frame_position; }
	glm::mat4 GetCustomCamera(glm::vec3 position, float yaw, float pitch);
	void Commit();

private:
	void Resolve();
	bool quatertions_ = false;
	glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
	glm::vec3 cameraFront;
	glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
	bool firstMouse = true;
	float fov = 45.0f;
	glm::mat4 view_;
	glm::mat4 projection_;
	float yaw = -90.0f;
	float pitch = 0.0f;
	glm::mat4 frame_view;
	float frame_pitch;
	float frame_yaw;
	glm::vec3 frame_position;
	

};
#endif // CAMERA_H

