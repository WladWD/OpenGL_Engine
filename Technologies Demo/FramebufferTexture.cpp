#include "FramebufferTexture.h"

using namespace SSAO;

FramebufferTexture::FramebufferTexture(const FramebufferTextureFormat &textureFormat, uint32_t width, uint32_t height) : 
	textureFormat(textureFormat) {
	initialize(width, height);
}

FramebufferTexture::FramebufferTexture(GLint internalFormat, GLenum format, GLenum type, uint32_t width, uint32_t height) : 
	FramebufferTexture({ internalFormat, format, type }, width, height) { }

FramebufferTexture::~FramebufferTexture() {
	clear();
}

void FramebufferTexture::clear() {
	if (glIsTexture(texture) == GL_TRUE) {
		glDeleteTextures(1, &texture);
	}
}

void FramebufferTexture::resize(uint32_t width, uint32_t height) {
	clear();
	initialize(width, height);
}

void SSAO::FramebufferTexture::updateMip() const {
	glBindTexture(GL_TEXTURE_2D, texture);
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
}

GLuint FramebufferTexture::getTexture() const {
	return texture;
}

void FramebufferTexture::initialize(uint32_t width, uint32_t height) {
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexImage2D(GL_TEXTURE_2D, 0, textureFormat.internalFormat, width, height, 0, textureFormat.format, textureFormat.type, nullptr);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);

	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
}

