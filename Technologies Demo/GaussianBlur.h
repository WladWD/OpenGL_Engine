#include "MainHeader.h"
#include <vector>
#include <memory>

#pragma once
namespace Shader {
class ShaderGaussianBlur;
}

namespace SSAO {
class Framebuffer;
class QuadMesh;

class GaussianBlur final {
public:
	GaussianBlur(uint32_t width, uint32_t height);
	~GaussianBlur();

	void makeBlur(GLuint sourceTexture, float mipBias = 0.0f);
	GLuint getBlurTexture() const;

private:
	void setupPass(GLuint sourceTexture, float mipBias, glm::vec2 texOffset);
	void initializeGaussian();

	const uint32_t width;
	const uint32_t height;
	std::vector<float> gaussianCore;

	std::unique_ptr<Shader::ShaderGaussianBlur> shader;
	std::unique_ptr<Framebuffer> framebufferPass0; 
	std::unique_ptr<Framebuffer> framebufferPass1;
	std::unique_ptr<QuadMesh> mesh;
};
}

