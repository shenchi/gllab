#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include "common.hpp"

class Texture {
	Texture(const Texture &) {}
	Texture(GLuint tex): m_tex(tex) {}
public:
	~Texture();


	static Texture* CreateTexture2D(int width, int height,
		int level = 0, GLint internalFormat = GL_RGBA, 
		GLenum format = GL_RGBA, GLenum type = GL_UNSIGNED_BYTE, const void* data = 0);

	static Texture* CreateDepthTexture(int width, int height);

	static Texture* CreateDepthStencilTexture(int width, int height);



	GLuint getTexture() const { return m_tex; }

private:
	GLuint m_tex;
};

#endif
