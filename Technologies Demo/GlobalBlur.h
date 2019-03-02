#include "DefaultFramebuffer.h"
#pragma once

namespace Resources
{
	class GlobalBlur
	{
		DefaultFramebuffer *framebuffer;

	public:
		GlobalBlur(int32_t width, int32_t height);
		~GlobalBlur();
		///////////////////////////////////////////////////////////////////////
		void blur(GLuint sourceTexture);
		GLuint getBlurTexture(void);
		///////////////////////////////////////////////////////////////////////
	};
}

