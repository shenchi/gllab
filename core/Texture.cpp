#include "Texture.hpp"


Texture::~Texture() {
	if (m_tex) glDeleteTextures(1, &m_tex);
}

Texture* Texture::CreateTexture2D(int width, int height,
	int level, GLint internalFormat, GLenum format, GLenum type, const void* data) {

	GLuint tex = 0;
	glGenTextures(1, &tex);
	if (!tex) return 0;

	glBindTexture(GL_TEXTURE_2D, tex);
	glTexImage2D(GL_TEXTURE_2D, level, internalFormat, width, height, 0, format, type, data);
	glBindTexture(GL_TEXTURE_2D, 0);

	return new Texture(tex, GL_TEXTURE_2D);
}

Texture* Texture::CreateDepthTexture(int width, int height) {
	return CreateTexture2D(width, height, 0, GL_DEPTH_COMPONENT, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
}

Texture* Texture::CreateDepthStencilTexture(int width, int height) {
	return CreateTexture2D(width, height, 0, GL_DEPTH_STENCIL, GL_DEPTH_STENCIL, GL_DEPTH24_STENCIL8, 0);
}