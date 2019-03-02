#include "DeferredDrawVNT.h"
#include "DeferredDrawVNTT.h"
#include "GBufferData.h"


#pragma once
namespace DeferredRendering
{
#define GBufferTexturesCount 3
	class DrawGBuffer
	{
		/*layout (location = 0) out vec2 out_color_0;//normal r16g16  unorm //vec4
layout (location = 1) out vec4 out_color_1;//diffuse r8g8b8a8, a - specular intensity
layout (location = 2) out vec4 out_color_2;//specular r8g8b8a8, a - specular power, but spec power = a*255.0f;*/
		//////////////////////////////////////////////////////////////////////////////////////
		MEngine::MResource *mresource;
		DeferredDrawVNT *draw_vnt;
		DeferredDrawVNTT *draw_vntt;
		//////////////////////////////////////////////////////////////////////////////////////
		MCamera::Camera *camera;
		//////////////////////////////////////////////////////////////////////////////////////
		uint32_t height, width;
		//////////////////////////////////////////////////////////////////////////////////////
		GLuint g_buffer;
		GLuint g_buffer_textures[GBufferTexturesCount];
		GLuint g_buffer_depth;
		//////////////////////////////////////////////////////////////////////////////////////
		GBufferData gbuffer_data;
		//////////////////////////////////////////////////////////////////////////////////////
		void UpdateMip(void);
		//////////////////////////////////////////////////////////////////////////////////////
		void SetGBufferDraw(void);
		void Init(void);
	public:
		DrawGBuffer(MCamera::Camera *camera, MEngine::MResource *mresource, MEngine::MObjects *VNT_object, MEngine::MDrawList VNT_draw_list,
			MEngine::MObjects *VNTT_object, MEngine::MDrawList VNTT_draw_list, uint32_t height, uint32_t width);
		~DrawGBuffer();
		//////////////////////////////////////////////////////////////////////////////////////
		void Draw(void);
		//////////////////////////////////////////////////////////////////////////////////////
		void DrawSpotShadow(const glm::mat4 *matrixes, uint32_t uCount, Shader::ShaderSpotLightShadow *shader);
		void DrawShadow(const glm::mat4 *matrixes, uint32_t uCount, Shader::ShaderPointLightShadow *shader);
		//////////////////////////////////////////////////////////////////////////////////////
		const GBufferData &GetBufferData();
	};
}

