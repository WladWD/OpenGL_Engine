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
	struct ParalelLight
	{
		glm::vec3 direction;
		float light_power;
		glm::vec4 diffuse;
		glm::vec4 specular;
	};
};
