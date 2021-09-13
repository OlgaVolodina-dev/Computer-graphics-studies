
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
	enum class Translation {
		FORWARD = 0, 
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
	void ProcessKeyboard(Translation e, float);
	void ProcessMouse(float xoffset, float yoffset);
	std::size_t GetUBOSize() override;
	void SetData(std::size_t offset) override;
	glm::mat4 GetProjView();
	float GetPitch() { return frame_pitch; }
	float GetYaw() { return frame_yaw; }
	glm::vec3 GetPosition() { return frame_position; }
	glm::vec3 GetDirection() { return frame_direction; }
	glm::vec3 GetUp() { return cameraUp; }
	glm::mat4 GetCustomCamera(glm::vec3 position, float yaw, float pitch);
	void Commit();
	void GetProjectionParams(float& fov, float& aspectRatio, float& farPlane, float& nearPlane);
	void UpdateWindowSize(int width, int height);

private:
	float fov = 45;
	float aspectRatio = 800.0 / 600.0;
	float farPlane = 1000.0f;
	float nearPlane = 0.1f;

	void Resolve();
	bool quatertions_ = false;
	glm::vec3 cameraPos = glm::vec3(64.0, 50.0, 64.0);
	glm::vec3 cameraFront;
	glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
	bool firstMouse = true;
	glm::mat4 view_;
	glm::mat4 projection_;
	float yaw = 0.0f;
	float pitch = 0.0f;
	glm::mat4 frame_view;
	float frame_pitch;
	float frame_yaw;
	glm::vec3 frame_position;
	glm::vec3 frame_direction;
	int scr_width = 800, scr_height = 600;
	

};
#endif // CAMERA_H

