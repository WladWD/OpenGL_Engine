#include "TextureManager.h"


Resources::TextureManager::TextureManager()
{
}

Resources::TextureManager::~TextureManager()
{
}

void Resources::TextureManager::initTextureDefault(GLuint texture, GLenum textureFormats, int32_t width, int32_t height)
{
	glBindTexture(GL_TEXTURE_2D, texture);
	glTextureStorage2D(texture, 1, textureFormats, width, height);

	int32_t mipMapCount = std::floor(std::log(glm::max(width, height)) / std::log(2.0)) + 1;

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, mipMapCount - 1);
}

GLuint * Resources::TextureManager::genTextures(uint32_t textureCount)
{
	GLuint * textures = new GLuint[textureCount];
	glGenTextures(textureCount, textures);


	return textures;
}

void Resources::TextureManager::initTextureDefault(uint32_t textureCount, const GLuint * textures, const GLenum * textureFormats, int32_t width, int32_t height)
{
	for (int32_t i = 0; i < textureCount; ++i) 
		initTextureDefault(textures[i], textureFormats[i], width, height);
}

void Resources::TextureManager::initTextureDepthDefault(GLuint texture, GLenum depthFormat, int32_t width, int32_t height)
{
	glBindTexture(GL_TEXTURE_2D, texture);
	glTextureStorage2D(texture, 1, depthFormat, width, height);

	int32_t mipMapCount = std::floor(std::log(glm::max(width, height)) / std::log(2.0)) + 1;

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, mipMapCount - 1);
}
