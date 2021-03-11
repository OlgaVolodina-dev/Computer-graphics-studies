
#ifndef CUBE_H
#define CUBE_H
#include "GL/glew.h"
#include "texture.h"
#include "shader.h"
#include "camera.h"
#include "shadowManager.h"
class Cube : public Object
{
public:
	Cube();
	void Draw(const Camera& camera);
	void DrawSimple();
private:
	GLuint VAO = 0U;
	ShaderProgram shader_;
	ShaderProgram depthPassShader_;
	Texture texture_;
};


#endif // !CUBE_H