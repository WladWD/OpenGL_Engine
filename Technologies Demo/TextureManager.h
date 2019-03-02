#include "MainHeader.h"
#pragma once

namespace Resources 
{
	class TextureManager
	{
		//////////////////////////////////////////////////////////////////////////////
		void initTextureDefault(GLuint texture, GLenum textureFormats, int32_t width, int32_t height);
		//////////////////////////////////////////////////////////////////////////////
	public:
		TextureManager();
		~TextureManager();
		//////////////////////////////////////////////////////////////////////////////
		GLuint *genTextures(uint32_t textureCount);
		void initTextureDefault(uint32_t textureCount, const GLuint *textures, const GLenum * textureFormats, int32_t width, int32_t height);
		void initTextureDepthDefault(GLuint texture, GLenum depthFormat, int32_t width, int32_t height);
		//////////////////////////////////////////////////////////////////////////////
	};
}
