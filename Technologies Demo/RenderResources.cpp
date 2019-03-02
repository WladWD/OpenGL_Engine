#include "RenderResources.h"

template RenderResource::RenderResources<2, 1>;

template<uint32_t tex_count, uint32_t framebuffer_count>
RenderResource::RenderResources<tex_count, framebuffer_count>::RenderResources(uint32_t width, uint32_t height) : width(width), height(height)
{
	InitTextures();
}

template<uint32_t tex_count, uint32_t framebuffer_count>
RenderResource::RenderResources<tex_count, framebuffer_count>::~RenderResources()
{
	glDeleteTextures(tex_count, textures);
	glDeleteFramebuffers(framebuffer_count, framebuffer);
}

template<uint32_t tex_count, uint32_t framebuffer_count>
void RenderResource::RenderResources<tex_count, framebuffer_count>::InitTextures(void)
{
	uint32_t mip_map_count = int(glm::log2<double>(glm::max<double>(width, height))) + 1;
	/////////////////////////////////////////////////////////////////////////////////////////
	glGenTextures(tex_count, textures);
	/////////////////////////////////////////////////////////////////////////////////////////
	glBindTexture(GL_TEXTURE_2D, textures[0]);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, mip_map_count - 1);

	glGenerateMipmap(GL_TEXTURE_2D);
	/////////////////////////////////////////////////////////////////////////////////////////
	glBindTexture(GL_TEXTURE_2D, textures[1]);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);//GL_REPEAT
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, mip_map_count - 1);

	glGenerateMipmap(GL_TEXTURE_2D);
	/////////////////////////////////////////////////////////////////////////////////////////
	glBindTexture(GL_TEXTURE_2D, 0);
	/////////////////////////////////////////////////////////////////////////////////////////
	glGenFramebuffers(framebuffer_count, framebuffer);

	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, framebuffer[0]);

	glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textures[0], 0);

	GLenum res_val;
	if ((res_val = glCheckFramebufferStatus(GL_DRAW_FRAMEBUFFER)) != GL_FRAMEBUFFER_COMPLETE)
		MessageBox(NULL, L"Framebuffer is not complette", L"", MB_OK);

	if (GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT == res_val)
		MessageBox(NULL, L"Framebuffer is not complette", L"", MB_OK);

	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

}

template<uint32_t tex_count, uint32_t framebuffer_count>
GLuint RenderResource::RenderResources<tex_count, framebuffer_count>::GetDiffuseTexture(void) 
{
	return textures[0];
}

template<uint32_t tex_count, uint32_t framebuffer_count>
GLuint RenderResource::RenderResources<tex_count, framebuffer_count>::GetSpecularTexture(void) 
{
	return textures[1];
}

template<uint32_t tex_count, uint32_t framebuffer_count>
GLuint RenderResource::RenderResources<tex_count, framebuffer_count>::GetFramebuffer(void)
{
	return framebuffer[0];
}