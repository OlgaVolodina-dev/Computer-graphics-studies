
#ifndef ENGINE_H
#define ENGINE_H
#include "camera.h"
#include "UBO.h"
#include "Item.h"
#include "WaterSimulation.h"
#include <memory>
class Engine
{
public:
	Engine();
	void Draw();
	Camera& GetCamera() { return camera_; }
	void SetWindowSize(int width, int height);
private:
	void ItemSetting();
	void CreateTextures();
	void ResolveUBO();
	bool msaa_ = true;
	GLuint msaaFBO_ = 0U;

	GLuint emptyVAO_;
	Camera camera_;
	UBO globalUBO_;

	std::vector<std::unique_ptr<Item>> items_;
	WaterSimulation water;
	int scr_width = 800, scr_height= 600;

	ShaderProgram quadProgram;
};

#endif

