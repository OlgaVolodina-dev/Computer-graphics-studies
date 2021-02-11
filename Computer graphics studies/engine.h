
#ifndef ENGINE_H
#define ENGINE_H
#include "camera.h"
#include "cube.h"
#include "UBO.h"
#include "lightSource.h"
class Engine
{
public:
	Engine();
	void Draw();
	Camera& GetCamera() { return camera_; }
private:
	void ResolveUBO();
	Cube cube_;
	Camera camera_;
	UBO globalUBO_;
	std::vector<LightSource> lightSources_;
};

#endif

