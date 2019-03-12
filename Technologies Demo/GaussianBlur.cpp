#include "GaussianBlur.h"
#include "ShaderGaussianBlur.h"
#include "Framebuffer.h"
#include "QuadMesh.h"

using namespace SSAO;

GaussianBlur::GaussianBlur(uint32_t width, uint32_t height): width(width), height(height) {
	framebufferPass0 = std::make_unique<Framebuffer>(width, height, 
		std::vector<FramebufferTexture::FramebufferTextureFormat>{ {GL_RGB8, GL_RGB, GL_UNSIGNED_BYTE} });
	framebufferPass1 = std::make_unique<Framebuffer>(width, height,
		std::vector<FramebufferTexture::FramebufferTextureFormat>{ {GL_RGB8, GL_RGB, GL_UNSIGNED_BYTE} });
	shader = std::make_unique<Shader::ShaderGaussianBlur>();
	mesh = std::make_unique<QuadMesh>();
	initializeGaussian();
}

GaussianBlur::~GaussianBlur() { }

void GaussianBlur::makeBlur(GLuint sourceTexture, float mipBias) {
	mesh->bindVAO();

	framebufferPass0->bind();
	setupPass(sourceTexture, mipBias, glm::vec2(1.0f / float(width), 0.0f));
	framebufferPass0->unbind();
	framebufferPass0->updateMip();

	framebufferPass1->bind();
	setupPass(framebufferPass0->getTexture(0)->getTexture(), 0.0f, glm::vec2(0.0f, 1.0f / float(height)));
	framebufferPass1->unbind();
	framebufferPass1->updateMip();

	glBindVertexArray(0);
}

GLuint GaussianBlur::getBlurTexture() const {
	return framebufferPass1->getTexture(0)->getTexture();
}

void SSAO::GaussianBlur::setupPass(GLuint sourceTexture, float mipBias, glm::vec2 texOffset) {
	glUseProgram(shader->getProgram());

	shader->bindSourceTexture(sourceTexture);
	shader->setGaussian(gaussianCore);
	shader->setMipBias(mipBias);
	shader->setTexOffset(texOffset);

	mesh->draw();
}

void GaussianBlur::initializeGaussian() {
	constexpr float q = 1.0f;
	constexpr float q2 = 2.0 * q * q;

	float sum = 0.0f;
	gaussianCore.resize(Shader::ShaderGaussianBlur::kernelSize);
	for (size_t i = 0; i < Shader::ShaderGaussianBlur::kernelSize; ++i) {
		float x = static_cast<float>(i);
		gaussianCore[i] = glm::exp(-x * x / q2);
		sum += gaussianCore[i] * (i? 2.0f: 1.0f);
	}
	for (size_t i = 0; i < Shader::ShaderGaussianBlur::kernelSize; ++i) {
		gaussianCore[i] /= sum;
	}
}
