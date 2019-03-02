#include "ShaderDeferredVNTT.h"
#include "ShaderSpotLightShadow.h"
#include "ShaderPointLightShadow.h"
#include "LoadModel.h"
#include "MCamera.h"

#pragma once
namespace DeferredRendering
{
	class DeferredDrawVNTT
	{
		/////////////////////////////////////////////////////////////
		MEngine::MResource *mresource;
		MEngine::MObjects *VNTT_object;
		MEngine::MDrawList VNTT_draw_list;
		/////////////////////////////////////////////////////////////
		MCamera::Camera *camera;
		/////////////////////////////////////////////////////////////
		Shader::ShaderDeferredVNTT *shader_deferred_vntt;
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
		DeferredDrawVNTT(MEngine::MResource *mresource, MEngine::MObjects *VNTT_object, MEngine::MDrawList VNTT_draw_list, MCamera::Camera *camera);
		~DeferredDrawVNTT();
		/////////////////////////////////////////////////////////////
		void Draw(void);
		void DrawShadow(const glm::mat4 *matrixes, uint32_t uCount, Shader::ShaderSpotLightShadow *shader);
		void DrawShadow(const glm::mat4 *matrixes, uint32_t uCount, Shader::ShaderPointLightShadow *shader);
	};
}

