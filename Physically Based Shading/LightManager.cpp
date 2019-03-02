#include "LightManager.h"

	
ComputeTileShading::LightManager::LightManager(void) 
{
	Init();
}

ComputeTileShading::LightManager::~LightManager() 
{
	delete point_lights;
	delete vSpotLight;

	delete vSpotLightShadowMap;
	delete vPointLightShadowMap;
}

void ComputeTileShading::LightManager::Init(void)
{
	point_lights = new PointLightManager();
	vSpotLight = new SpotLightManager();

	vSpotLightShadowMap = new SpotLightShadowMap(512, 512);
	vPointLightShadowMap = new PointLightShadowMap(512, 512);
}

void ComputeTileShading::LightManager::AddPointLight(PointLight data) 
{
	point_lights->AddLight(data);
}

void ComputeTileShading::LightManager::AddPointLight(glm::vec4 pos_radius, glm::vec4 diffuse) 
{
	diffuse = glm::clamp(diffuse, glm::vec4(0.0f, 0.0f, 0.0f, 0.0f), glm::vec4(1.0f, 1.0f, 1.0f, 256.0f));
	point_lights->AddLight({ pos_radius, diffuse });
	vPointLightShadowMap->AddLightShadow(pos_radius);
}

void ComputeTileShading::LightManager::AddSpotLight(glm::vec4 vlightPositionRadius, glm::vec3 vlightLookAtPosition, float vConeAngle, glm::vec4 vlightColor) 
{
	vlightColor = glm::clamp(vlightColor, glm::vec4(0.0f, 0.0f, 0.0f, 0.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
	vSpotLight->AddLight(vlightPositionRadius, vlightLookAtPosition, vConeAngle, vlightColor);
	vSpotLightShadowMap->AddLightShadow(vlightPositionRadius, glm::vec3(vlightPositionRadius), vlightLookAtPosition, vConeAngle * 2.0f);
}

void ComputeTileShading::LightManager::DrawSpotLightShadow( const glm::mat4 *&matrixes,  Shader::ShaderSpotLightShadow *&shader, uint32_t &uCount)
{
	matrixes = vSpotLightShadowMap->Draw(uCount);
	shader = vSpotLightShadowMap->GetShadowShader();
}

void ComputeTileShading::LightManager::DrawPointLightShadow(const glm::mat4 *&matrixes, Shader::ShaderPointLightShadow *&shader, uint32_t &uCount)
{
	matrixes = vPointLightShadowMap->Draw(uCount);
	shader = vPointLightShadowMap->GetShadowShader();
}

const glm::vec4 * ComputeTileShading::LightManager::GetPointShadowParam(void)
{
	return vPointLightShadowMap->GetShadowParam();
}

void ComputeTileShading::LightManager::EndDrawShadowPoint(void)
{
	vPointLightShadowMap->EndDraw();
}

void ComputeTileShading::LightManager::EndDrawShadowSpot(void)
{
	vSpotLightShadowMap->EndDraw();
}

GLuint ComputeTileShading::LightManager::GetSpotLightShadowMaps(uint32_t &uCount, const glm::mat4 *&matrixes)
{
	uCount = vSpotLightShadowMap->GetShadowMapCount();
	matrixes = vSpotLightShadowMap->GetShadowMapMatrixes();
	return vSpotLightShadowMap->GetMaps();
}

GLuint ComputeTileShading::LightManager::GetPointLightShadowMaps(uint32_t &uCount, const glm::vec4 *&mVectors)
{
	uCount = vPointLightShadowMap->GetShadowMapCount();
	mVectors = vPointLightShadowMap->GetShadowParam();
	return vPointLightShadowMap->GetMaps();
}

void ComputeTileShading::LightManager::UpdatePointLightParamList(void) 
{
	point_lights->UpdateLight();
}

void ComputeTileShading::LightManager::SetPointLightToBuffer(void) 
{
	point_lights->SetLightToBuffer();
	vPointLightShadowMap->ReCreateShadowMaps();
}

void ComputeTileShading::LightManager::SetSpotLightToBuffer(void)
{
	vSpotLight->SetLightToBuffer();
	vSpotLightShadowMap->ReCreateShadowMaps();
}

GLuint ComputeTileShading::LightManager::GetPointLightBufferColor(void)
{
	return point_lights->GetPointLightBufferColor();
}

int32_t ComputeTileShading::LightManager::GetPointLightCount(void) 
{
	return point_lights->GetLightCount();
}

int32_t ComputeTileShading::LightManager::GetSpotLightCount(void) 
{
	return vSpotLight->GetLightCount();
}

GLuint ComputeTileShading::LightManager::GetPointLightBufferPositionRadius(void) 
{
	return point_lights->GetPointLightBufferPositionRadius();
}

const ComputeTileShading::TileBufferSpot &ComputeTileShading::LightManager::GetSpotLightTextureBuffers(void)
{
	return vSpotLight->GetTextureBuffer();
}