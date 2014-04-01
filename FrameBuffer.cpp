#include "FrameBuffer.hpp"


FrameBuffer::~FrameBuffer() {
	if (m_fbo) glDeleteFramebuffers(1, &m_fbo);
	if (m_rbos) glDeleteRenderbuffers(m_cbCount, m_rbos);
	if (m_depthRBO) glDeleteRenderbuffers(1, &m_depthRBO);
	if (m_texs) {
		for (int i = 0; i < m_cbCount; ++i) 
			if (m_texs[i]) delete m_texs[i];
		delete [] m_texs;
	}
	if (m_depthTex) delete m_depthTex;
}


FrameBuffer* FrameBuffer::CreateFrameBuffer(int width, int height,
	int nColorBuffers, bool colorBufferUseTextures, bool hasDepthBuffer, bool depthBufferUseTexture) {

	bool failed = false;
	FrameBuffer *fb = new FrameBuffer();
	glGenFramebuffers(1, &(fb->m_fbo));
	if (!(fb->m_fbo)) {
		delete fb;
		return 0;
	}
	glBindFramebuffer(GL_FRAMEBUFFER, fb->m_fbo);

	fb->m_cbCount = nColorBuffers;
	if (nColorBuffers > 0 && colorBufferUseTextures) {
		fb->m_texs = new LPTexture[ nColorBuffers ];
		for (int i = 0; i < nColorBuffers; ++i) {
			Texture* t = Texture::CreateTexture2D(width, height);
			if (!t) {
				failed = true;
				break;
			}
			fb->m_texs[i] = t;
			glBindTexture(GL_TEXTURE_2D, t->getTexture());
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, t->getTexture(), 0);
		}
		glBindTexture(GL_TEXTURE_2D, 0);

	} else if (nColorBuffers > 0) {
		fb->m_rbos = new GLuint[ nColorBuffers ];
		glGenRenderbuffers(nColorBuffers, fb->m_rbos);
		for (int i = 0; i < nColorBuffers; ++i) {
			if (!(fb->m_rbos[i])) {
				failed = true;
				break;
			}
			glBindRenderbuffer(GL_RENDERBUFFER, fb->m_rbos[i]);
			glRenderbufferStorage(GL_RENDERBUFFER, GL_RGBA, width, height);
			glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_RENDERBUFFER, fb->m_rbos[i]);
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