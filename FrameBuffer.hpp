#ifndef FRAMEBUFFER_HPP
#define FRAMEBUFFER_HPP

#include "common.hpp"
#include "Texture.hpp"

class FrameBuffer {
	typedef Texture *LPTexture;

	FrameBuffer(const FrameBuffer &) {}
	FrameBuffer(): m_fbo(0), m_rbos(0), m_texs(0), m_cbCount(0), m_depthRBO(0), m_depthTex(0) {}
public:
	~FrameBuffer();

	static FrameBuffer* CreateFrameBuffer(int width, int height,
		int nColorBuffers = 1, bool colorBufferUseTextures = false, bool hasDepthBuffer = true, bool depthBufferUseTexture = false);

	GLuint getFrameBuffer() const { return m_fbo; }

private:
	GLuint m_fbo;

	GLuint *m_rbos;
	Texture **m_texs;
	int m_cbCount;

	GLuint m_depthRBO;
	Texture *m_depthTex;
};

#endif

