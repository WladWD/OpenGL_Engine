#include "FramebufferManager.h"


Resources::FramebufferManager::FramebufferManager(void) : framebuffer(0), attachDepth(0) {
	glGenFramebuffers(1, &framebuffer);
}

Resources::FramebufferManager::~FramebufferManager()
{
	glDeleteFramebuffers(1, &framebuffer);
}

void Resources::FramebufferManager::initFramebuffer(uint32_t textureCount, const GLuint * textures)
{
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

	attachTexture.clear();
	attachTexture.resize(textureCount);
	for (int32_t i = 0; i < textureCount; ++i) {
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, textures[i], 0);
		attachTexture[i] = textures[i];
	}

	GLenum res_val;
	if ((res_val = glCheckFramebufferStatus(GL_FRAMEBUFFER)) != GL_FRAMEBUFFER_COMPLETE)
		MessageBox(NULL, L"Framebuffer is not complette", L"", MB_OK);

	if (GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT == res_val)
		MessageBox(NULL, L"Framebuffer is not complette", L"", MB_OK);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Resources::FramebufferManager::initDepthTexture(GLuint depth)
{
	attachDepth = depth;
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depth, 0);

	GLenum res_val;
	if ((res_val = glCheckFramebufferStatus(GL_FRAMEBUFFER)) != GL_FRAMEBUFFER_COMPLETE)
		MessageBox(NULL, L"Framebuffer is not complette", L"", MB_OK);

	if (GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT == res_val)
		MessageBox(NULL, L"Framebuffer is not complette", L"", MB_OK);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

GLuint Resources::FramebufferManager::getFramebuffer(void)
{
	return framebuffer;
}

const GLuint * Resources::FramebufferManager::getFramebufferTextures(void)
{
	return attachTexture.data();
}

GLuint Resources::FramebufferManager::getFramebufferTexture(uint32_t idx)
{
	return attachTexture[idx];
}

GLuint Resources::FramebufferManager::getFramebufferDepth(void)
{
	return attachDepth;
}
