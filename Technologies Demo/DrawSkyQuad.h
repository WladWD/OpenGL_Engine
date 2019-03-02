#define GLEW_STATIC
#include <glew.h>
#include <Windows.h>
#include <stdint.h>

#include "LoadStructure.h"
#include "ShaderUpdateSky.h"
#include "AtmosphereParameter.h"
#include "ShaderMoon.h"


#pragma once
namespace Atmosphere
{
	class DrawSkyQuad
	{
		////////////////////////////////////////////////////////////////////////
		GLuint MoonTexture;
		GLuint MoonGlowTexture;
		////////////////////////////////////////////////////////////////////////
		GLuint QUAD_VAO, QUAD_VB;
		GLuint QUAD_IB;
		////////////////////////////////////////////////////////////////////////
		Shader::ShaderUpdateSky *shader_update_sky;
		Shader::ShaderMoon *shader_moon;
		////////////////////////////////////////////////////////////////////////
		void GenerateQuad(void);
		void InitShaderVariable(void);
		////////////////////////////////////////////////////////////////////////
		void DrawGlow(glm::mat4 proj_view, glm::vec3 cam_pos, const Atmosphere::GlobalLightingParam *atmosphere_parameter);
		glm::mat4 BildWorldFormAngles(float fPhi, float fThetta, glm::vec3 light_dir, glm::vec3 cam_pos, float scale);
	public:
		DrawSkyQuad(GLuint MoonTexture, GLuint MoonGlowTexture);
		~DrawSkyQuad();
		////////////////////////////////////////////////////////////////////////
		void Draw(const Atmosphere::GlobalLightingParam *atmosphere_parameter, const ScatteringParam *scattering_param);
		void DrawMoon(const Atmosphere::GlobalLightingParam *atmosphere_parameter, glm::mat4 proj_view, glm::vec3 cam_pos);
	};
}