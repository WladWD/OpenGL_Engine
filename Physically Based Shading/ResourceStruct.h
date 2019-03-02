#define GLEW_STATIC
#include <glew.h>
#include <Windows.h>
#include <stdint.h>

#pragma once
namespace Resource
{
	struct GLTextureResource
	{
		GLuint texture;
		GLenum texture_target;
	};

	struct ResourceName
	{
		char resource_name[MAX_PATH];
	};
}
