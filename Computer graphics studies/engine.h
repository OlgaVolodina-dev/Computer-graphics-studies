
#ifndef ENGINE_H
#define ENGINE_H
#include "camera.h"
#include "cube.h"
#include "UBO.h"
#include "lightSource.h"
#include "shadowManager.h"
#include "terrain.h"
#include "Item.h"

class Engine
{
public:
	Engine();
	void Draw();
	Camera& GetCamera() { return camera_; }
	void ShowDepth(bool enable) { showDepth_ = enable; }
private:
	void ItemSetting();

	bool showDepth_ = false;
	bool msaa_ = true;
	GLuint msaaFBO_ = 0U;
	GLuint postProcessTex_ = 0U;
	GLuint postProcessFBO_ = 0U;
	GLuint postProcessDSTex_ = 0U;
	GLuint emptyVAO_;
	void ResolveUBO();
	Cube cube_;
	Terrain terrain_;
	Camera camera_;
	UBO globalUBO_;
	std::vector<LightSource> lightSources_;
	ShadowManager shadowManager_;
	ShaderProgram quadProgram;
	ShaderProgram bloomPreprocessingProgram_;
	ShaderProgram bloomPostprocessingProgram_;
	GLuint bloomTex_;
	GLuint bloomFBO_;
	std::vector<Item> items_;
};

#endif

