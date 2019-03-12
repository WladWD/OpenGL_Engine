#include "FramebufferTexture.h"
#include <vector>
#include <memory>

#pragma once
namespace SSAO {
class Framebuffer {

public:
	Framebuffer(uint32_t width, uint32_t height,
		std::vector<FramebufferTexture::FramebufferTextureFormat> texturesFormats);
	Framebuffer(uint32_t width, uint32_t height,
		std::vector<FramebufferTexture::FramebufferTextureFormat> texturesFormats, 
		FramebufferTexture::FramebufferTextureFormat depthFormat);
	~Framebuffer();

	void bind() const;
	void unbind() const;
	GLuint getFramebuffer() const;
	std::shared_ptr<FramebufferTexture> getTexture(uint32_t idx) const;
	void updateMip() const;
	void resize(uint32_t width, uint32_t height);

private:
	void initialize();
	void clear();

	GLuint framebuffer;
	std::vector<GLenum> attachment;
	std::vector<std::shared_ptr<FramebufferTexture>> textures;
};
}
