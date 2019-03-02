#include "ShaderComputeTileShading.h"
#include "RenderResources.h"
#include "GBufferData.h"
#include "AtmosphereParameter.h"
#include "LightManager.h"
#include "LightInit.h"
#include "MCamera.h"

#pragma once
namespace ComputeTileShading
{
	class LightCompute
	{
		//////////////////////////////////////////////////////////////////////////////////////
		MCamera::Camera *camera;
		RenderResource::RenderResources<2, 1> *render_resources;
		const DeferredRendering::GBufferData &gbuffer_data;

		LightManager *light_manager;
		//////////////////////////////////////////////////////////////////////////////////////
		GLuint gPointLightColor;
		GLuint gPointLightPositionRadius;
		//////////////////////////////////////////////////////////////////////////////////////
		uint32_t height, width;
		const uint32_t threadx_count = 32, thready_count = 32;
		uint32_t thread_x, thread_y, thread_z;
		//////////////////////////////////////////////////////////////////////////////////////
		Shader::ShaderComputeTileShading *shader_tile_shading;
		//////////////////////////////////////////////////////////////////////////////////////
		void Init(void);
	public:
		LightCompute(LightManager *light_manager, MCamera::Camera *camera, RenderResource::RenderResources<2, 1> *render_resources, const DeferredRendering::GBufferData &gbuffer_data);
		~LightCompute();

		void Draw(const Atmosphere::MoonLightingParam &light_moon, const Atmosphere::GlobalLightingParam &light_sun);
	};
}
//http://stackoverflow.com/questions/21384972/sample-depth-buffer-in-opengl-compute-shader
//https://johanmedestrom.wordpress.com/2016/03/30/tiled-deferred-shading-in-opengl-using-compute-shaders/
//https://github.com/bcrusco/Forward-Plus-Renderer
/*fragDataOut0.xyz =
		(diffuseLight + iblDiffuse) * diffuseTex.xyz
		+ specularLight * specularTex.xyz
		;
	fragDataOut0.w = 1.0;*/
//3.402823466e+38F;//0x7f7fffff
//-3.402823466e+38F;//0xff7fffff
