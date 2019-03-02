#include "Shader_Struct.h"
#include "resource.h"

#pragma once
namespace Shader
{
	class Shader_Resource_Compiler
	{
		bool result;
		//////////////////////////////////////////////////////////////////////
		GLuint CompileShaderFrom(GLenum type, const wchar_t *type_resource, unsigned int resource_ID);
		GLuint CompileShaderFromWithMacros(GLenum type, const wchar_t *type_resource, char *macros, uint32_t macross_size, unsigned int resource_ID);
	public:
		Shader_Resource_Compiler(Shader_Resource *shaders, UINT count, const wchar_t *type_resource);
		~Shader_Resource_Compiler();
		//////////////////////////////////////////////////////////////////////
		bool GetResult(void);
	};
}