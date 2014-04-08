#include "FrameBuffer.hpp"


FrameBuffer::~FrameBuffer() {
	if (m_fbo) glDeleteFramebuffers(1, &m_fbo);
	if (m_colorBuffers) {
		for (size_t i = 0; i < m_cbCount; ++i) {
			if (m_usedTex[i]) {
				if (m_colorBuffers[i].texture) delete m_colorBuffers[i].texture;
			} else {
				if (m_colorBuffers[i].object) glDeleteRenderbuffers(1, &(m_colorBuffers[i].object));
			}
		}
		delete m_usedTex;
		delete m_colorBuffers;
	}

	if (m_depthRBO) glDeleteRenderbuffers(1, &m_depthRBO);
	if (m_depthTex) delete m_depthTex;
}

Texture* FrameBuffer::getColorTexture(size_t i) const {
	if (i >= m_cbCount) return 0;
	if (!m_usedTex[i]) return 0;
	return m_colorBuffers[i].texture;
}

Texture* FrameBuffer::getDepthTexture() const {
	return m_depthTex;
}

FrameBuffer* FrameBuffer::CreateFrameBuffer(int width, int height,
		int nColorBuffers, RenderTargetDesc *colorBuffers, 
		bool hasDepthBuffer, bool depthBufferUseTexture) {

	bool failed = false;
	FrameBuffer *fb = new FrameBuffer();
	glGenFramebuffers(1, &(fb->m_fbo));
	if (!(fb->m_fbo)) {
		delete fb;
		return 0;
	}
	glBindFramebuffer(GL_FRAMEBUFFER, fb->m_fbo);

	fb->m_cbCount = nColorBuffers;
	if (nColorBuffers > 0 && colorBuffers) {
		fb->m_colorBuffers = new ColorBuffer[ nColorBuffers ];
		fb->m_usedTex = new bool[ nColorBuffers ];

		for (int i = 0; i < nColorBuffers; ++i) {
			fb->m_usedTex[i] = colorBuffers[i].useTexture;
			if (colorBuffers[i].useTexture) {
				Texture* t = Texture::CreateTexture2D(width, height, 0, 
					colorBuffers[i].internalFormat, colorBuffers[i].format, colorBuffers[i].type);
				if (!t) {
					failed = true;
					break;
				}
				fb->m_colorBuffers[i].texture = t;
				glBindTexture(GL_TEXTURE_2D, t->getTexture());
				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, t->getTexture(), 0);
			} else {
				GLuint rbo = 0;
				glGenRenderbuffers(1, &rbo);
				if (!rbo) {
					failed = true;
					break;
				}
				fb->m_colorBuffers[i].object = rbo;
				glBindRenderbuffer(GL_RENDERBUFFER, rbo);
				glRenderbufferStorage(GL_RENDERBUFFER, colorBuffers[i].internalFormat, width, height);
				glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_RENDERBUFFER, rbo);
			}
		}

		glBindTexture(GL_TEXTURE_2D, 0);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);

	} else if (nColorBuffers > 0) {
		fb->m_colorBuffers = new ColorBuffer[ nColorBuffers ];
		fb->m_usedTex = new bool[ nColorBuffers ];

		for (int i = 0; i < nColorBuffers; ++i) {
			GLuint rbo = 0;
			fb->m_usedTex[i] = false;
			glGenRenderbuffers(1, &rbo);
			if (!rbo) {
				failed = true;
				break;
			}
			fb->m_colorBuffers[i].object = rbo;
			glBindRenderbuffer(GL_RENDERBUFFER, rbo);
			glRenderbufferStorage(GL_RENDERBUFFER, GL_RGBA, width, height);
			glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_RENDERBUFFER, rbo);
		}
		glBindRenderbuffer(GL_RENDERBUFFER, 0);
	}

	if (hasDepthBuffer && depthBufferUseTexture) {
		fb->m_depthTex = Texture::CreateDepthTexture(width, height);
		if (fb->m_depthTex) {
			glBindTexture(GL_TEXTURE_2D, fb->m_depthTex->getTexture());
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, fb->m_depthTex->getTexture(), 0);
			glBindTexture(GL_TEXTURE_2D, 0);
		} else {
			failed = true;
		}
	} else if (hasDepthBuffer) {
		glGenRenderbuffers(1, &fb->m_depthRBO);
		if (fb->m_depthRBO) {
			glBindRenderbuffer(GL_RENDERBUFFER, fb->m_depthRBO);
			glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
			glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, fb->m_depthRBO);
			glBindRenderbuffer(GL_RENDERBUFFER, 0);
		} else {
			failed = true;
		}
	}

	failed = (GL_FRAMEBUFFER_COMPLETE != glCheckFramebufferStatus(GL_FRAMEBUFFER));
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	if (failed) {
		delete fb;
		return 0;
	}
	return fb;
}