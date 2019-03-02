#include "ShaderDeferredVNT.h"
#include "ShaderSpotLightShadow.h"
#include "ShaderPointLightShadow.h"
#include "LoadModel.h"
#include "MCamera.h"

#pragma once
namespace DeferredRendering
{
	class DeferredDrawVNT
	{
		/////////////////////////////////////////////////////////////
		MEngine::MResource *mresource;
		MEngine::MObjects *VNT_object;
		MEngine::MDrawList VNT_draw_list;
		/////////////////////////////////////////////////////////////
		MCamera::Camera *camera;
		/////////////////////////////////////////////////////////////
		Shader::ShaderDeferredVNT *shader_deferred_vnt;
		/////////////////////////////////////////////////////////////
		void InitBuffer(GLuint program);
		void InitPrograms(void);
		/////////////////////////////////////////////////////////////
		void DrawEarlyDepthTest(void);
		void DrawClipTex(void);
		/////////////////////////////////////////////////////////////
		void DrawEarlyDepthTestShadow(const glm::mat4 *matrixes, uint32_t uCount, Shader::ShaderSpotLightShadow *shader);
		void DrawClipTexShadow(const glm::mat4 *matrixes, uint32_t uCount, Shader::ShaderSpotLightShadow *shader);
		/////////////////////////////////////////////////////////////
		void DrawEarlyDepthTestShadow(const glm::mat4 *matrixes, uint32_t uCount, Shader::ShaderPointLightShadow *shader);
		void DrawClipTexShadow(const glm::mat4 *matrixes, uint32_t uCount, Shader::ShaderPointLightShadow *shader);
	public:
		DeferredDrawVNT(MEngine::MResource *mresource, MEngine::MObjects *VNT_object, MEngine::MDrawList VNT_draw_list, MCamera::Camera *camera);
		~DeferredDrawVNT();
		/////////////////////////////////////////////////////////////
		void Draw(void);
		/////////////////////////////////////////////////////////////
		void DrawShadow(const glm::mat4 *matrixes, uint32_t uCount, Shader::ShaderSpotLightShadow *shader);
		void DrawShadow(const glm::mat4 *matrixes, uint32_t uCount, Shader::ShaderPointLightShadow *shader);
	};
}

