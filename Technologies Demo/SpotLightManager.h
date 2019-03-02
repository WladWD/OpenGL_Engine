#include "LightStructs.h"
#include "SpotLightShadowMap.h"
#include <vector>

#pragma once
namespace ComputeTileShading
{
	class SpotLightManager
	{
		std::vector<SpotLight> vSpotLightData;//в релизной конфигурации можно удалить это
		///////////////////////////////////////////////////////////////////////////////////////
		std::vector<glm::vec4> g_BoundingSphereCenterRadius;
		std::vector<LightColor> g_LightColor;
		//std::vector<LightF16> g_LightDirectionAndAngleCos;
		std::vector<glm::vec4> g_LightDirectionRadiusAndAngleCos;
		///////////////////////////////////////////////////////////////////////////////////////
		GLuint gLightColor;
		GLuint gBoundingSphereCenterRadius;
		GLuint gSpotLightDirertionCosineAngle;
		///////////////////////////////////////////////////////////////////////////////////////
		GLuint gLightColorBuffer;
		GLuint gBoundingSphereCenterRadiusBuffer;
		GLuint gSpotLightDirertionCosineAngleBuffer;
		///////////////////////////////////////////////////////////////////////////////////////
		TileBufferSpot tbs;
		///////////////////////////////////////////////////////////////////////////////////////
		uint16_t ConvertF32ToF16(float fValueToConvert);
		void Init(void);
	public:
		SpotLightManager(void);
		~SpotLightManager();
		///////////////////////////////////////////////////////////////////////////////////////
		void AddLight(glm::vec4 vlightPositionRadius, glm::vec3 vlightLookAtPosition, float vConeAngle, glm::vec4 vlightColor);
		void UpdateLight(const SpotLight &vlight, uint32_t i);
		void SetLightToBuffer(void);
		///////////////////////////////////////////////////////////////////////////////////////
		GLuint GetSpotLightColorTexture(void);
		GLuint GetSpotLightBoundingSpherePositionRadius(void);
		GLuint GetSpotLightDirectionCosineAngleRadius();
		///////////////////////////////////////////////////////////////////////////////////////
		int32_t GetLightCount(void);
		///////////////////////////////////////////////////////////////////////////////////////
		const TileBufferSpot &GetTextureBuffer(void);
	};
}

