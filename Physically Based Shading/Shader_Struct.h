#define GLEW_STATIC
#include <glew.h>
#include <Windows.h>
#include <stdint.h>

#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/constants.hpp>
#include <stdint.h>

#pragma once
namespace Shader
{
	struct Shader_File
	{
		GLenum type_shader;
		wchar_t *file_name;
		GLuint shader;

		char *define;
		uint32_t define_size;

		Shader_File()
		{
			file_name = 0;
			define = 0;
			define_size = 0;
			type_shader = 0;
			shader = 0;
		}
	};

	struct Shader_Resource
	{
		GLenum type_shader;
		unsigned int resource_ID;
		GLuint shader;

		char *define;
		uint32_t define_size;
		Shader_Resource()
		{
			type_shader = 0;
			define = 0;
			define_size = 0;
			resource_ID = 0;
			shader = 0;
		}
	};
}