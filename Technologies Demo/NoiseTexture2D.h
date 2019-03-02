#include "MainHeader.h"
#include <functional>

#pragma once
namespace SSAO {
template<
	GLint InternalFormat,
	GLenum Format,
	GLenum Type>
class NoiseTexture2D {
public:
	NoiseTexture2D(uint32_t width, uint32_t height, 
		std::function<glm::vec4(uint32_t x, uint32_t y)> func): width(width), height(height) {
		initializeTexture(func);
		initializeProperties();
	}

	~NoiseTexture2D() {
		if (glIsTexture(texture) == GL_TRUE) {
			glDeleteTextures(1, &texture);
		}
	}

	uint32_t getWidth() const {
		return width;
	}

	uint32_t getHeight() const {
		return height;
	}

	GLuint getTexture() const {
		return texture;
	}

private:
	void initializeProperties() {
		glBindTexture(GL_TEXTURE_2D, texture);

		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);

		glBindTexture(GL_TEXTURE_2D, 0);
	}

	void initializeTexture(std::function<glm::vec4(uint32_t x, uint32_t y)> func);

	uint32_t width;
	uint32_t height;
	GLuint texture;
};
}

