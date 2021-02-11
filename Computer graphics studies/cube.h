
#ifndef CUBE_H
#define CUBE_H
#include "GL/glew.h"
#include "texture.h"
#include "shader.h"
#include "camera.h"
class Cube
{
public:
	Cube();
	void Draw(const Camera& camera);
private:
	GLuint VAO = 0U;
	ShaderProgram shader_;
	Texture texture_;
};


#endif // !CUBE_H