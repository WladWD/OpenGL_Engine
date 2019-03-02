#define GLEW_STATIC
#include <glew.h>
#include <Windows.h>
#include <stdint.h>

#include "LoadStructure.h"
#include "ShaderSkyPlane.h"
#include "ShaderSkyPlaneUpdate.h"
#include "LoadTexture.h"
#include "AtmosphereParameter.h"


#pragma once
namespace Atmosphere
{
	class DrawCloud
	{
		////////////////////////////////////////////////////////////////////////
		//uint32_t m_tex_dim;
		////////////////////////////////////////////////////////////////////////
		GLuint NoiseTexture;
		////////////////////////////////////////////////////////////////////////
		uint32_t PlaneN;
		uint32_t DV_Size, DI_Size;
		/*////////////////////////////////////////////////////////////////////////
		GLuint CloudFrameBuffer;
		GLuint CloudTexture;*/
		////////////////////////////////////////////////////////////////////////
		GLuint SD_VAO, SD_VB;
		GLuint SD_IB;
		////////////////////////////////////////////////////////////////////////
		float max_cover;
		float min_cover;
		/*////////////////////////////////////////////////////////////////////////
		GLuint QD_VAO, QD_VB;
		GLuint QD_IB;
		////////////////////////////////////////////////////////////////////////*/
		Shader::ShaderSkyPlane *shader_sky_plane;
		//Shader::ShaderSkyPlaneUpdate *shader_sky_plane_update;
		////////////////////////////////////////////////////////////////////////
		int xor128(void);
		double Noise2D(int x, int y);
		uint8_t MSample(double a);
		////////////////////////////////////////////////////////////////////////
		void GenerateSkyPlane(void);
		//void GenerateUpdatePlane(void);
		void InitShaderVariable(void);
		void InitNoiseTexture(void);
		//void InitFramebuffer(void);
		//void UpdateCloud(float global_time_ms, glm::mat4 pvw);
	public:
		DrawCloud(void);
		~DrawCloud();
		////////////////////////////////////////////////////////////////////////
		void Draw(float global_time_ms, glm::mat4 pvw,
			const Atmosphere::GlobalLightingParam *atmosphere_parameter, const Atmosphere::MoonLightingParam *moon_param,
			float cloud_density, float cloud_gray_scale);
	};
}
