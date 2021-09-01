
#ifndef ENGINE_H
#define ENGINE_H
#include "camera.h"
#include "UBO.h"
#include "lightSource.h"
#include "shadowManager.h"
#include "Item.h"
#include "WaterSimulation.h"
#include "GaussianBlur.h"
#include <memory>
class Engine
{
public:
	Engine();
	void Draw();
	Camera& GetCamera() { return camera_; }
	void ShowDepth(bool enable) { showDepth_ = enable; }
	void SetWindowSize(int width, int height);
private:
	void ItemSetting();
	void CreateTextures();
	bool showDepth_ = false;
	bool msaa_ = true;
	GLuint msaaFBO_ = 0U;
	GLuint postProcessTex_ = 0U;
	GLuint postProcessFBO_ = 0U;
	GLuint postProcessDSTex_ = 0U;
	GLuint emptyVAO_;
	void ResolveUBO();
	Camera camera_;
	UBO globalUBO_;
	std::vector<LightSource> lightSources_;

	GLuint bloomTex_ = 0U;
	GLuint bloomFBO_;
	std::vector<std::shared_ptr<Item>> items_;
	GaussianBlur gaussBlur;
	WaterSimulation water;
	int scr_width = 800, scr_height= 600;

	ShadowManager shadowManager_;
	ShaderProgram quadProgram;
	ShaderProgram bloomPreprocessingProgram_;
	ShaderProgram bloomPostprocessingProgram_;
};

#endif

