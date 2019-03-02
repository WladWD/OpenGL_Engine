#include "NoiseTexture2D.h"
#include <memory>

using namespace SSAO;
template class NoiseTexture2D<GL_RGBA16F, GL_RGBA, GL_FLOAT>;

template<GLint InternalFormat, GLenum Format, GLenum Type>
inline void NoiseTexture2D<InternalFormat, Format, Type>::initializeTexture(
	std::function<glm::vec4(uint32_t x, uint32_t y)> func) {

	std::unique_ptr<glm::vec4[]> pixels(new glm::vec4[width * height]);
	for (size_t i = 0; i < height; ++i) {
		for (size_t j = 0; j < width; ++j) {
			pixels[i * width + j] = func(j, i);
		}
	}

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, InternalFormat, width, height, 0, Format, Type, pixels.get());

	glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, 0);
}
