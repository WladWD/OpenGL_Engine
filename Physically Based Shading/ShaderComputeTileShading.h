#include "Shader_Resource_Compiler.h"
#include "ShaderBase.h"
#include "ParalelLight.h"

#pragma once
namespace Shader
{
	class ShaderComputeTileShading : public ShaderBase
	{
#define mprog_count 1
#define mbuffer_count 1
		////////////////////////////////////////////////////
		//static const int mprog_count = 1;//количество программ для текущего шейдера
		////////////////////////////////////////////////////
		GLuint uniform_buffers[mbuffer_count];
		////////////////////////////////////////////////////
		/*g_SpotColor;
layout(location = 11, binding = 7) uniform samplerBuffer g_SpotBoundingSpherePositionRadius;
layout(location = 12, binding = 8) uniform samplerBuffer g_SpotDirectionRadiusCosineAngle;*/
		GLint day_night[mprog_count]; 
		GLint g_inv_proj_view[mprog_count];
		GLint g_vCameraPos[mprog_count];
		////////////////////////////////////////////////////
		//in
		GLint g_depth[mprog_count];
		GLint g_normal[mprog_count];
		GLint g_diffuse[mprog_count];
		GLint g_specular[mprog_count];
		GLint g_PBR[mprog_count];
		////////////////////////////////////////////////////
		GLint g_PointColor[mprog_count];
		GLint g_PointPositionRadius[mprog_count];
		////////////////////////////////////////////////////
		GLint g_SpotColor[mprog_count];
		GLint g_SpotBoundingSpherePositionRadius[mprog_count];
		GLint g_SpotDirectionRadiusCosineAngle[mprog_count];
		////////////////////////////////////////////////////
		GLint gSpotLightShadowMap[mprog_count];
		GLint g_mProjViewSpot[mprog_count];
		////////////////////////////////////////////////////
		GLint gEnvMap[mprog_count];
		////////////////////////////////////////////////////
		//out
		GLint g_albedro[mprog_count];
		GLint g_spec_albedro[mprog_count];
		////////////////////////////////////////////////////
		GLint g_mProjectionInv[mprog_count];
		GLint g_mView[mprog_count];
		GLint g_uNumLights[mprog_count];
		GLint g_sNumSpotLights[mprog_count];
		GLint g_sTexDim[mprog_count];
		GLint g_mInvProjViewInvViewport[mprog_count];
		GLint gTileCount[mprog_count];
		////////////////////////////////////////////////////
		GLint gPointLightShadowMap[mprog_count];
		GLint g_mNF[mprog_count];
		////////////////////////////////////////////////////
		GLint g_depth_slot, g_normal_slot, g_diffuse_slot, g_specular_slot, g_PBR_slot;
		GLint g_pointlight_color_slot, g_pointlight_pos_radius_slot;
		GLint g_spotlight_color_slot, g_spotlight_bounding_sphere_pos_radius_slot, g_spotlight_data;
		GLint gout_diff_slot, gout_spec_slot;
		GLint g_smSpotShadow, g_smPointShadow, gEnvMapSlot;
		////////////////////////////////////////////////////
#undef mprog_count
#undef mbuffer_count
		////////////////////////////////////////////////////
		ComputeTileShading::ParalelLight light_data[2];
		////////////////////////////////////////////////////
		void InitUniformBuffers(void);
		////////////////////////////////////////////////////
		void InitVariable(GLuint program);
	public:
		ShaderComputeTileShading(void);
		~ShaderComputeTileShading();
		////////////////////////////////////////////////////
		GLuint GetProgram(void);
		////////////////////////////////////////////////////
		void BindUniformBuffers(void);
		void SetUniformBuffersData(void);
		void SetDayLightParam(ComputeTileShading::ParalelLight data);
		void SetNightLightParam(ComputeTileShading::ParalelLight data);
		////////////////////////////////////////////////////
		void SetSpotLightShadowMatrixes(const glm::mat4 matrix[], uint32_t count);
		void SetSpotLightShadowMap(void);
		////////////////////////////////////////////////////
		void SetEnvCubeMap(void);
		////////////////////////////////////////////////////
		void SetDepthTexture(void);
		void SetNormalTexture(void);
		void SetDiffuseTexture(void);
		void SetSpecularTexture(void);
		void SetPBSTexture(void);
		////////////////////////////////////////////////////
		void SetPointLightColor(void);
		void SetPointLightPositionRadius(void);
		////////////////////////////////////////////////////
		void SetPointLightShadowMap(void);
		void SetPointLightShadowVectorsParam(const glm::vec4 mvec[], uint32_t count);
		////////////////////////////////////////////////////
		void SetSpotLightColor(void);
		void SetSpotLightBoundingSphere(void);
		void SetSpotLightData(void);
		////////////////////////////////////////////////////
		void SetProjInvMatrix(glm::mat4 matrix);
		void SetViewMatrix(glm::mat4 view);
		void SetNumPointLight(int32_t count);
		void SetNumSpotLight(int32_t count);
		void SetDimension(glm::ivec2 dim);
		void SetInvProjViewInvViewport(glm::mat4 matrix);
		void SetTileCount(glm::ivec2 tile_count);
		////////////////////////////////////////////////////
		void SetInvProjView(glm::mat4 matrix);
		void SetEyePosition(glm::vec3 pos);
		////////////////////////////////////////////////////
		GLint GetOutDiffSlot(void);
		GLint GetOutSpecSlot(void);
		////////////////////////////////////////////////////
		void BindTextureToUnit(void);
	};
}

