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
namespace ComputeTileShading
{
	struct PointLight
	{
		glm::vec4 pos_radius;//xyz - position, w - radius
		glm::vec4 diffuse;//in a - light power, specular term = diffuse
		glm::vec4 g_Attention;
	};

	struct SpotLight
	{
		glm::vec4 g_BoundingSphereCenterRadius;//xyz - position, w - radius
		glm::vec4 g_Diffuse;//in a - light power, specular term = diffuse
		glm::vec4 g_LightDirectionAndAngle;//xyz - direction, w - exponent
		glm::vec4 g_Attention;
	};

	struct TileBufferSpot 
	{
		GLuint gLightColor;
		GLuint gBoundingSphereCenterRadius;
		GLuint gSpotLightDirertionCosineAngle;
	};

	struct LightColor 
	{
		uint8_t r, g, b, a;

		static LightColor GenFromVector4(glm::vec4 color) 
		{
			LightColor LC;
			LC.r = glm::clamp(color.r * 255.0f, 0.0f, 255.0f);
			LC.g = glm::clamp(color.g * 255.0f, 0.0f, 255.0f);
			LC.b = glm::clamp(color.b * 255.0f, 0.0f, 255.0f);
			LC.a = glm::clamp(color.a * 255.0f, 0.0f, 255.0f);

			return LC;
		}
	};

	struct LightF16
	{
		uint16_t x, y, z, w;
	};
};

