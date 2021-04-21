
#ifndef ENGINE_H
#define ENGINE_H
#include "camera.h"
#include "cube.h"
#include "UBO.h"
#include "lightSource.h"
#include "shadowManager.h"
#include "terrain.h"

class Engine
{
public:
	Engine();
	void Draw();
	Camera& GetCamera() { return camera_; }
	void ShowDepth(bool enable) { showDepth_ = enable; }
private:
	bool showDepth_ = false;
	bool msaa_ = true;
	GLuint msaaFBO_ = 0;
	void ResolveUBO();
	Cube cube_;
	Terrain terrain_;
	Camera camera_;
	UBO globalUBO_;
	std::vector<LightSource> lightSources_;
	ShadowManager shadowManager_;
};

#endif

