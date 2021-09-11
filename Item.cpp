#include "Item.h"
#include <iostream>
Item::Item(ItemInitializationInfo& info):
	info(info),
	colorTex_(info.colorTexName, info.colorTexGenerateMipmap),
	metallicTex_(info.metallicTexName),
	shader_(info.vert, info.frag),
	depthPassShader_(DEPTH_PASS_VERT, DEPTH_PASS_FRAG),
	simpleColorShader_(SIMPLE_COLOR_VERT, SIMPLE_COLOR_FRAG)
{}

Item::~Item()
{
	Reset();
}

void Item::Reset() {
	if (VAO) {
		glDeleteVertexArrays(1, &VAO);
		VAO = 0U;
	}
	if (VBO) {
		glDeleteBuffers(1, &VBO);
		VBO = 0U;
	}
}

void Item::LoadItem()
{
	ObjReader::ReadObj(info.obj_file_name, vertices, bb_local);
	//CalculateBoundingBoxes();
}

void Item::LoadBuffers()
{
	glGenVertexArrays(1, &VAO);
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

	modelMatrix_.clear();
	for (int i = 0; i < posInfo.n_indices; ++i) {
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, posInfo.position_[i]);
		if (posInfo.rotation_angle_[i] != 0.0) {
			model = glm::rotate(model, glm::radians(posInfo.rotation_angle_[i]), glm::vec3(1.0, 0.5, 1.0));
		}
		modelMatrix_.push_back(glm::scale(model, glm::vec3(posInfo.scale_[i])));
	}

	GLuint modMatBuf;
	glGenBuffers(1, &modMatBuf);
	glBindBuffer(GL_ARRAY_BUFFER, modMatBuf);
	glBufferData(GL_ARRAY_BUFFER, modelMatrix_.size() * sizeof(glm::mat4), modelMatrix_.data(), GL_STATIC_DRAW);
	for (int i = 0; i < 4; ++i) {
		glVertexAttribPointer(i + 3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (const void*)(sizeof(float) * (i * 4)));
		glEnableVertexAttribArray(i + 3);
		glVertexAttribDivisor(i + 3, 1);
	}
	glUniformBlockBinding(shader_, 0U, 0U);
}


void Item::DrawSimpleColor()
{
	if (!VAO) { 
		return;
	}
	glBindVertexArray(VAO);
	colorTex_.Use(0);
	if (posInfo.n_indices == 1) {
		glDrawArrays(GL_TRIANGLES, 0, std::size(vertices));
	} else {
		glDrawArraysInstanced(GL_TRIANGLES, 0, std::size(vertices), posInfo.n_indices);
	}
}

void Item::DrawSimple()
{
	if (!VAO) { 
		return;
	}
	glBindVertexArray(VAO);
	glUseProgram(depthPassShader_);
	if (posInfo.n_indices == 1) {
		glDrawArrays(GL_TRIANGLES, 0, std::size(vertices));
	} else {
		glDrawArraysInstanced(GL_TRIANGLES, 0, std::size(vertices), posInfo.n_indices);
	}
}

void Item::Draw() {
	if (!VAO) { 
		return;
	}
	glUseProgram(shader_);
	glBindVertexArray(VAO);
	colorTex_.Use();
	metallicTex_.Use(1);

	if (posInfo.n_indices == 1) {
		glDrawArrays(GL_TRIANGLES, 0, std::size(vertices));
	}
	else {
		glDrawArraysInstanced(GL_TRIANGLES, 0, std::size(vertices), posInfo.n_indices);
	}
}

void Item::CalculateBoundingBoxes()
{
	BoundingBox bb_world_item;
	glm::vec4 min, max;
	for (auto& modelMatrix : modelMatrix_) {
		min = glm::vec4(bb_local.minX, bb_local.minY, bb_local.minZ, 1.0);
		max = glm::vec4(bb_local.maxX, bb_local.maxY, bb_local.maxZ, 1.0);
		min = modelMatrix * min;
		max = modelMatrix * max;
		bb_world_item.maxX = max.x;
		bb_world_item.maxY = max.y;
		bb_world_item.maxZ = max.z;
		bb_world_item.minX = min.x;
		bb_world_item.minY = min.y;
		bb_world_item.minZ = min.z;
		bb_world.push_back(bb_world_item);
	}
}