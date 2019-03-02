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
namespace RenderResource
{
	template<uint32_t tex_count, uint32_t framebuffer_count>
	class RenderResources
	{
		//////////////////////////////////////////////////////////////////////////////////////
		const uint32_t width;
		const uint32_t height;
		//////////////////////////////////////////////////////////////////////////////////////
		GLuint textures[tex_count];
		GLuint framebuffer[framebuffer_count];
		//////////////////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////////////////
		void InitTextures(void);
	public:
		RenderResources(uint32_t width, uint32_t height);
		~RenderResources();

		GLuint GetDiffuseTexture(void);
		GLuint GetSpecularTexture(void);
		GLuint GetFramebuffer(void);
	};
}

