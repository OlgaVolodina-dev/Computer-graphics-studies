#include "GaussianBlur.h"
#include "utils.h"
#define SCR_WIDTH 800
#define SCR_HEIGHT 600

GaussianBlur::GaussianBlur():
	one_axis_blur(QUAD_VERT, ONE_AXIS_GAUSSIAN_FRAG)
{
	std::cout << __func__ << std::endl;
	glGenTextures(1, &cacheTex);
	glBindTexture(GL_TEXTURE_2D, cacheTex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);

	glGenFramebuffers(1, &pingFbo);
	glGenFramebuffers(1, &pongFbo);

	glGenVertexArrays(1, &emptyVAO);
}


void GaussianBlur::Blur(GLuint texture, size_t size, size_t iterations)
{
	glDisable(GL_DEPTH_TEST);

	glBindFramebuffer(GL_FRAMEBUFFER, pingFbo);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, cacheTex, 0);
	//CHECK_FRAMEBUFFER_STATUS()
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glBindFramebuffer(GL_FRAMEBUFFER, pongFbo);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);
	//CHECK_OPENGL_ERROR()
	for (int i = 0; i < iterations; ++i) {
		glBindFramebuffer(GL_FRAMEBUFFER, pingFbo);
		glUseProgram(one_axis_blur);
		glUniform1i(0, 1);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);
		glBindVertexArray(emptyVAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glUseProgram(one_axis_blur);
		glUniform1i(0, 0);
		glBindFramebuffer(GL_FRAMEBUFFER, pongFbo);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, cacheTex);
		glBindVertexArray(emptyVAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);
	}
	glBindTexture(GL_TEXTURE_2D, texture);
	glGenerateMipmap(GL_TEXTURE_2D);
	glEnable(GL_DEPTH_TEST);

}