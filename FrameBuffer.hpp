#ifndef FRAMEBUFFER_HPP
#define FRAMEBUFFER_HPP

#include "common.hpp"
#include "Texture.hpp"

struct RenderTargetDesc {
	bool useTexture;
	GLuint internalFormat;
	GLenum format;
	GLenum type;

	// default
	RenderTargetDesc() : useTexture(false), internalFormat(GL_RGBA), format(GL_RGBA), type(GL_UNSIGNED_BYTE) {}

	// default format
	RenderTargetDesc(bool _useTexture) : useTexture(_useTexture), internalFormat(GL_RGBA), format(GL_RGBA), type(GL_UNSIGNED_BYTE) {}

	// render buffer
	RenderTargetDesc(GLuint _internalFormat) : useTexture(false), internalFormat(_internalFormat), format(GL_RGBA), type(GL_UNSIGNED_BYTE) {}

	// texture
	RenderTargetDesc(GLuint _internalFormat, GLenum _format, GLenum _type): 
		useTexture(true), internalFormat(_internalFormat), format(_format), type(_type) {}
};

class FrameBuffer {

	FrameBuffer(const FrameBuffer &) {}
	FrameBuffer(): m_fbo(0), m_colorBuffers(0), m_usedTex(0), m_cbCount(0), m_depthRBO(0), m_depthTex(0) {}
public:
	~FrameBuffer();

	static FrameBuffer* CreateFrameBuffer(int width, int height,
		int nColorBuffers = 1, RenderTargetDesc *colorBuffers = 0, 
		bool hasDepthBuffer = true, bool depthBufferUseTexture = false);

	GLuint getFrameBuffer() const { return m_fbo; }

private:
	GLuint m_fbo;

	union ColorBuffer {
		GLuint object;
		Texture* texture;
	};
	int m_cbCount;
	ColorBuffer* m_colorBuffers;
	bool* m_usedTex;

	GLuint m_depthRBO;
	Texture *m_depthTex;
};

#endif

