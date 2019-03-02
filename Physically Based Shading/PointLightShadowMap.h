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
#include <vector>
#include "ShaderPointLightShadow.h"


#pragma once
namespace ComputeTileShading
{
	class PointLightShadowMap
	{
		//////////////////////////////////////////////////////////////////////////////////////////////////
		const uint32_t smWidth, smHeight;
		GLuint smFramebuffer;
		GLuint smShadowTextureArray;
		//////////////////////////////////////////////////////////////////////////////////////////////////
		std::vector<glm::mat4> smShadowMatrices;// 6 matrix per light
		std::vector<glm::vec4> smParamData;//
		//////////////////////////////////////////////////////////////////////////////////////////////////
		Shader::ShaderPointLightShadow *smShader;
		//////////////////////////////////////////////////////////////////////////////////////////////////
		GLint sViewport[4];
		//////////////////////////////////////////////////////////////////////////////////////////////////
		glm::mat4 GetViewMatrix(glm::vec4 fPositionAndRadius, int faceID);
		//////////////////////////////////////////////////////////////////////////////////////////////////
		void Init(void);
	public:
		PointLightShadowMap(uint32_t smWidth, uint32_t smHeight);
		~PointLightShadowMap();
		//////////////////////////////////////////////////////////////////////////////////////////////////
		void AddLightShadow(glm::vec4 fPositionAndRadius);
		//////////////////////////////////////////////////////////////////////////////////////////////////
		GLuint GetMaps(void);
		uint32_t GetShadowMapCount(void);
		const glm::mat4 *GetShadowMapMatrixes(void);
		//////////////////////////////////////////////////////////////////////////////////////////////////
		Shader::ShaderPointLightShadow *GetShadowShader(void);
		//////////////////////////////////////////////////////////////////////////////////////////////////
		const glm::vec4 *GetShadowParam(void);
		//////////////////////////////////////////////////////////////////////////////////////////////////
		const glm::mat4 * Draw(uint32_t &uCount);
		void EndDraw(void);
		//////////////////////////////////////////////////////////////////////////////////////////////////
		void ReCreateShadowMaps(void);
	};
}
// http://steps3d.narod.ru/tutorials/texture-swizzle-tutorial.html
//https://www.opengl.org/discussion_boards/showthread.php/155441-Single-pass-render-to-cubemap
//http://stackoverflow.com/questions/28845375/rendering-a-dynamic-cubemap-opengl
/*layer number   cube map face
      ------------   ---------------------------
           0         TEXTURE_CUBE_MAP_POSITIVE_X
           1         TEXTURE_CUBE_MAP_NEGATIVE_X
           2         TEXTURE_CUBE_MAP_POSITIVE_Y
           3         TEXTURE_CUBE_MAP_NEGATIVE_Y
           4         TEXTURE_CUBE_MAP_POSITIVE_Z
		   5         TEXTURE_CUBE_MAP_NEGATIVE_Z*/