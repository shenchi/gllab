#include "Texture.hpp"
#include <FreeImage.h>

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
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glBindTexture(GL_TEXTURE_2D, 0);

	return new Texture(tex, GL_TEXTURE_2D);
}

Texture* Texture::CreateDepthTexture(int width, int height) {
	return CreateTexture2D(width, height, 0, GL_DEPTH_COMPONENT, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
}

Texture* Texture::CreateDepthStencilTexture(int width, int height) {
	return CreateTexture2D(width, height, 0, GL_DEPTH_STENCIL, GL_DEPTH_STENCIL, GL_DEPTH24_STENCIL8, 0);
}


Texture* Texture::CreateFromFile(const char* filename) {
	FreeImage_Initialise(FALSE);
	FREE_IMAGE_FORMAT fif = FreeImage_GetFileType(filename, 0);
	if (fif == FIF_UNKNOWN)
		fif = FreeImage_GetFIFFromFilename(filename);
	if (fif == FIF_UNKNOWN) {
		FreeImage_DeInitialise();
		return 0;
	}

	FIBITMAP *image = FreeImage_Load(fif, filename);
	if (!image) {
		FreeImage_DeInitialise();
		return 0;
	}
	unsigned bpp = FreeImage_GetBPP(image);
	FREE_IMAGE_COLOR_TYPE colorType = FreeImage_GetColorType(image);
	FREE_IMAGE_TYPE imageType = FreeImage_GetImageType(image);
	unsigned width = FreeImage_GetWidth(image);
	unsigned height = FreeImage_GetHeight(image);
	unsigned char* data = FreeImage_GetBits(image);
	if (!data) {
		FreeImage_DeInitialise();
		return 0;
	}

	GLint internalFormat = GL_RGB;
	GLenum format = GL_RGB;
	GLenum type = GL_UNSIGNED_BYTE;

	if (bpp == 8) {

		internalFormat = GL_RED;
		format = GL_RED;
		type = GL_UNSIGNED_BYTE;

	} else if (colorType == FIC_RGB && (bpp == 24 || bpp == 32)) {

		internalFormat = GL_RGB;
		format = GL_RGB;
		type = GL_UNSIGNED_BYTE;

	} else if (imageType == FIT_RGB16 || (colorType == FIC_RGB && bpp == 48)) {

		internalFormat = GL_RGB16UI;
		format = GL_RGB;
		type = GL_UNSIGNED_SHORT;

	} else if (imageType == FIT_RGBF || (colorType == FIC_RGB && bpp == 96)) {

		internalFormat = GL_RGB32F; // F? UI ?
		format = GL_RGB;
		type = GL_FLOAT; // UNSIGNED_INT ?

	} else if (colorType == FIC_RGBALPHA && bpp == 32) {

		internalFormat = GL_RGBA;
		format = GL_RGBA;
		type = GL_UNSIGNED_BYTE;

	} else if (imageType == FIT_RGBA16 || (colorType == FIC_RGBALPHA && bpp == 64)) {

		internalFormat = GL_RGBA16UI;
		format = GL_RGBA;
		type = GL_UNSIGNED_SHORT;

	} else if (imageType == FIT_RGBAF || (colorType == FIC_RGBALPHA && bpp == 128)) {

		internalFormat = GL_RGBA32F; // F? UI ?
		format = GL_RGBA;
		type = GL_FLOAT; // UNSIGNED_INT ?

	} else {
		FreeImage_Unload(image);
		FreeImage_DeInitialise();
		return 0;
	}

	Texture *tex = CreateTexture2D(width, height, 0, internalFormat, format, type, data);

	FreeImage_Unload(image);
	FreeImage_DeInitialise();

	return tex;
}

void Texture::bind(GLuint unit) {
	glActiveTexture(GL_TEXTURE0 + unit);
	glBindTexture(m_target, m_tex);
}