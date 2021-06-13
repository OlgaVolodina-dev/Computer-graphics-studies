#include "Item.h"
#include <iostream>
Item::Item(ItemInitializationInfo& info):
	info(info),
	colorTex_(info.colorTexName),
	metallicTex_(info.metallicTexName),
	shader_(info.vert, info.frag),
	depthPassShader_(DEPTH_PASS_VERT, DEPTH_PASS_FRAG)
{}

void Item::LoadItem()
{
	ObjReader::ReadObj(info.obj_file_name, vertices, bb);
	glGenVertexArrays(1, &VAO);
	std::cout << vertices.size() << std::endl;
	GLuint VBO;
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, std::size(vertices) * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
	glEnableVertexAttribArray(2);

	
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, info.position_);
	if (info.rotation_angle != 0.0) {
		model = glm::rotate(model, glm::radians(info.rotation_angle), glm::vec3(1.0, 0.5, 1.0));
	}
	modelMatrix_ = glm::scale(model, glm::vec3(info.scale));

	GLuint modMatBuf;
	glGenBuffers(1, &modMatBuf);
	glBindBuffer(GL_ARRAY_BUFFER, modMatBuf);
	glBufferData(GL_ARRAY_BUFFER, sizeof(modelMatrix_), &modelMatrix_, GL_STATIC_DRAW);
	for (int i = 0; i < 4; ++i) {
		glVertexAttribPointer(i + 3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (const void*)(sizeof(float) * (i * 4)));
		glEnableVertexAttribArray(i + 3);
		glVertexAttribDivisor(i + 3, 1);
	}
	//dirty for gloabal UBO
	glUniformBlockBinding(shader_, 0U, 0U);
}

void Item::DrawSimple()
{
	glUseProgram(depthPassShader_);
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, std::size(vertices));
}

void Item::Draw() {

	glUseProgram(shader_);
	glBindVertexArray(VAO);
	colorTex_.Use();
	metallicTex_.Use(1);
	glDrawArrays(GL_TRIANGLES, 0, std::size(vertices));
}