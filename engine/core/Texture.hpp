#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include "common.hpp"

class Texture {
	Texture(const Texture &) {}
	Texture(GLuint tex, GLenum target): m_tex(tex), m_target(target) {}
public:
	~Texture();


	static Texture* CreateTexture2D(int width, int height,
		int level = 0, GLint internalFormat = GL_RGBA, 
		GLenum format = GL_RGBA, GLenum type = GL_UNSIGNED_BYTE, const void* data = 0);

	static Texture* CreateDepthTexture(int width, int height);

	static Texture* CreateDepthStencilTexture(int width, int height);

	static Texture* CreateFromFile(const char* filename);

	GLenum getTarget() const { return m_target; }

	GLuint getTexture() const { return m_tex; }

	void bind(GLuint unit);

private:
	GLuint m_tex;
	GLenum m_target;
};

#endif
