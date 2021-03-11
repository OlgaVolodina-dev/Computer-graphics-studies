#include "terrain.h"

#include <iostream>
Terrain::Terrain():
	color_(glm::vec3(0.25, 0.47, 0.29)),
	position_(glm::vec3(0.0, -5.0, 0.0)),
	shader_(TESSELATION_VERT, TESSELATION_TC, TESSELATION_TES, TESSELATION_FRAG), 
	depthPassShader_(DEPTH_PASS_VERT, DEPTH_PASS_FRAG),
	heightmap_("heightmap.png"),
	colormap_("Terrain-Colormap.png")
	
{
	const float vertices[] = {
		1.0, 0.0, -1.0, 1.0, 0.0, 
		1.0, 0.0, 1.0, 1.0, 1.0,
		-1.0, 0.0, -1.0, 0.0, 0.0,

		1.0, 0.0, 1.0, 1.0, 1.0,
		-1.0, 0.0, -1.0, 0.0, 0.0,
		-1.0, 0.0, 1.0, 0.0, 1.0
	};
	glGenVertexArrays(1, &VAO_);
	GLuint VBO;
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO_);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3*sizeof(float)));
	glEnableVertexAttribArray(1);

	glm::mat4 modMat;
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, position_);
	modMat = glm::scale(model, glm::vec3(20));

	GLuint modMatBuf;
	glGenBuffers(1, &modMatBuf);
	glBindBuffer(GL_ARRAY_BUFFER, modMatBuf);
	glBufferData(GL_ARRAY_BUFFER, sizeof(modMat), &modMat, GL_STATIC_DRAW);
	for (int i = 0; i < 4; ++i) {
		glVertexAttribPointer(i + 3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (const void*)(sizeof(float) * (i * 4)));
		glEnableVertexAttribArray(i + 3);
		glVertexAttribDivisor(i + 3, 1);
	}
	glUniformBlockBinding(shader_, 0U, 0U);
	glUniformBlockBinding(depthPassShader_, 0U, 0U);

}

void Terrain::DrawSimple()
{
	glUseProgram(depthPassShader_);
	glBindVertexArray(VAO_);
	glDrawArrays(GL_TRIANGLES, 0, 6);
}

void Terrain::Draw()
{
	glUseProgram(shader_);
	heightmap_.Use();
	colormap_.Use(1);
	glBindVertexArray(VAO_);
	glUniform3fv(glGetUniformLocation(shader_, "aColor"), 1, glm::value_ptr(color_));
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glPatchParameteri(GL_PATCH_VERTICES, 3);
	glDrawArrays(GL_PATCHES, 0, 6);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}