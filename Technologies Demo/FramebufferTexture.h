#define GLEW_STATIC
#include <glew.h>
#include <cstdint>

#pragma once
namespace SSAO {
class FramebufferTexture {
public:
	struct FramebufferTextureFormat {
		GLint internalFormat;
		GLenum format;
		GLenum type;
	};

public:
	FramebufferTexture(const FramebufferTextureFormat &textureFormat, uint32_t width, uint32_t height);
	FramebufferTexture(GLint internalFormat, GLenum format, GLenum type, uint32_t width, uint32_t height);
	~FramebufferTexture();

	void resize(uint32_t width, uint32_t height);

	GLuint getTexture() const;

protected:
	void initialize(uint32_t width, uint32_t height);
	void clear();

	FramebufferTextureFormat textureFormat;
	GLuint texture;
};
}
