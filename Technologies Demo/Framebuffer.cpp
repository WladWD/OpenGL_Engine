#include "Framebuffer.h"
#include "FramebufferTexture.h"

using namespace SSAO;

Framebuffer::Framebuffer(uint32_t width, uint32_t height, 
	std::vector<FramebufferTexture::FramebufferTextureFormat> texturesFormats) {

	this->textures.reserve(textures.size() + 1);
	for (size_t i = 0; i < texturesFormats.size(); ++i) {
		auto& texture = std::make_shared<FramebufferTexture>(texturesFormats[i], width, height);
		this->textures.push_back(texture);
		attachment.push_back(GL_COLOR_ATTACHMENT0 + i);
	}
	initialize();
}

Framebuffer::Framebuffer(uint32_t width, uint32_t height,
	std::vector<FramebufferTexture::FramebufferTextureFormat> texturesFormats,
	FramebufferTexture::FramebufferTextureFormat depthFormat) {

	this->textures.reserve(textures.size() + 1);
	for (size_t i = 0; i < texturesFormats.size(); ++i) {
		auto& texture = std::make_shared<FramebufferTexture>(texturesFormats[i], width, height);
		this->textures.push_back(texture);
		attachment.push_back(GL_COLOR_ATTACHMENT0 + i);
	}

	auto& texture = std::make_shared<FramebufferTexture>(depthFormat, width, height);
	this->textures.push_back(texture);
	attachment.push_back(GL_DEPTH_ATTACHMENT);
	initialize();
}

Framebuffer::~Framebuffer() {
	clear();
}

void Framebuffer::bind() const {
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, framebuffer);
	glDrawBuffers(attachment.size(), attachment.data());
}

void Framebuffer::unbind() const {
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
	glDrawBuffer(GL_BACK);
}

GLuint Framebuffer::getFramebuffer() const {
	return framebuffer;
}

std::shared_ptr<FramebufferTexture> SSAO::Framebuffer::getTexture(uint32_t idx) const {
	return textures[idx];
}

void SSAO::Framebuffer::updateMip() const {
	for (auto& texture : textures) {
		texture->updateMip();
	}
}

void Framebuffer::resize(uint32_t width, uint32_t height) {
	clear();

	for (auto& texture : textures) {
		texture->resize(width, height);
	}
}

void Framebuffer::clear() {
	if (glIsFramebuffer(framebuffer) == GL_TRUE) {
		glDeleteFramebuffers(1, &framebuffer);
	}
}

void Framebuffer::initialize() {
	glGenFramebuffers(1, &framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

	for (size_t i = 0; i < attachment.size(); ++i) {
		glFramebufferTexture2D(GL_FRAMEBUFFER, attachment[i], GL_TEXTURE_2D, textures[i]->getTexture(), 0);
	}

	GLenum res_val;
	if ((res_val = glCheckFramebufferStatus(GL_DRAW_FRAMEBUFFER)) != GL_FRAMEBUFFER_COMPLETE) {
		throw std::runtime_error("Invalid framebuffer status");
	}

	if (GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT == res_val) {
		throw std::runtime_error("Incomplete framebuffer attachment");
	}

	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
}
