#include "PointLightManager.h"
#include "SpotLightManager.h"

#pragma once
namespace ComputeTileShading
{
	class LightManager
	{
		////////////////////////////////////////////////////////////////////////////////////////
		PointLightManager *point_lights;
		PointLightShadowMap *vPointLightShadowMap;
		////////////////////////////////////////////////////////////////////////////////////////
		SpotLightManager *vSpotLight;
		SpotLightShadowMap *vSpotLightShadowMap;
		////////////////////////////////////////////////////////////////////////////////////////
		void Init(void);
	public:
		LightManager(void);
		~LightManager();
		////////////////////////////////////////////////////////////////////////////////////////
		void AddPointLight(PointLight data);
		void AddPointLight(glm::vec4 pos_radius, glm::vec4 diffuse);
		////////////////////////////////////////////////////////////////////////////////////////
		void AddSpotLight(glm::vec4 vlightPositionRadius, glm::vec3 vlightLookAtPosition, float vConeAngle, glm::vec4 vlightColor);
		////////////////////////////////////////////////////////////////////////////////////////
		void UpdatePointLightParamList(void);
		void SetPointLightToBuffer(void);
		////////////////////////////////////////////////////////////////////////////////////////
		GLuint GetSpotLightShadowMaps(uint32_t &uCount, const glm::mat4 *&matrixes);
		GLuint GetPointLightShadowMaps(uint32_t &uCount, const glm::vec4 *&mVectors);
		////////////////////////////////////////////////////////////////////////////////////////
		void SetSpotLightToBuffer(void);
		////////////////////////////////////////////////////////////////////////////////////////
		void DrawSpotLightShadow(const glm::mat4 *&matrixes, Shader::ShaderSpotLightShadow *&shader, uint32_t &uCount);
		void DrawPointLightShadow(const glm::mat4 *&matrixes, Shader::ShaderPointLightShadow *&shader, uint32_t &uCount);
		////////////////////////////////////////////////////////////////////////////////////////
		const glm::vec4 *GetPointShadowParam(void);
		////////////////////////////////////////////////////////////////////////////////////////
		void EndDrawShadowPoint(void);
		void EndDrawShadowSpot(void);
		////////////////////////////////////////////////////////////////////////////////////////
		int32_t GetPointLightCount(void);
		int32_t GetSpotLightCount(void);
		////////////////////////////////////////////////////////////////////////////////////////
		GLuint GetPointLightBufferColor(void);
		GLuint GetPointLightBufferPositionRadius(void);
		////////////////////////////////////////////////////////////////////////////////////////
		const TileBufferSpot &GetSpotLightTextureBuffers(void);
	};
}
/*glGenTextures(1, &color_tbo);
glBindTexture(GL_TEXTURE_BUFFER, color_tbo);
glTexBuffer(GL_TEXTURE_BUFFER, GL_RGBA32F, color_buffer);
// Now do the same thing with a TBO for the model matrices. The
// buffer object (model_matrix_buffer) has been created and sized
// to store one mat4 per-instance.
glGenTextures(1, &model_matrix_tbo);
glActiveTexture(GL_TEXTURE1);
glBindTexture(GL_TEXTURE_BUFFER, model_matrix_tbo);
glTexBuffer(GL_TEXTURE_BUFFER, GL_RGBA32F, model_matrix_buffer);*/

/*// Buffer to be used as the data store
GLuint buf;
// Texture to be used as a buffer texture
GLuint tex;
// Data is located somewhere else in this program
extern const GLvoid* data;
// Generate, bind, and initialize a buffer object
// using the GL_TEXTURE_BUFFER binding. Assume we’re
// going to use one megabyte of data here.
glGenBuffers(1, &buf);
glBindBuffer(GL_TEXTURE_BUFFER, buf);
glBufferData(GL_TEXTURE_BUFFER, 1024 * 1024,
data, GL_STATIC_DRAW);
// Now create the buffer texture and associate it
// with the buffer object.
glGenTextures(1, &tex);
glBindTexture(GL_TEXTURE_BUFFER, tex);
glTexBuffer(GL_TEXTURE_BUFFER, GL_R32F, buf);*/
/*#version 330 core
uniform samplerBuffer buf
in int buf_tex_coord;
layout (location = 0) out vec4 color;
void main(void)
{
color = texelFetch(buf, tex_coord);*/
