#include "PointLightManager.h"


ComputeTileShading::PointLightManager::PointLightManager(void) 
{
	Init();
}

ComputeTileShading::PointLightManager::~PointLightManager() 
{
	point_light_data.clear();
	///////////////////////////////////////////////////////////////////////////////////////
	glDeleteTextures(1, &texture_light_color_buffer);
	glDeleteTextures(1, &texture_light_pos_rad_buffer);

	glDeleteBuffers(1, &light_color_buffer);
	glDeleteBuffers(1, &light_pos_rad_buffer);
}

void ComputeTileShading::PointLightManager::Init(void)
{
	glGenBuffers(1, &light_color_buffer);
	glGenBuffers(1, &light_pos_rad_buffer);

	glGenTextures(1, &texture_light_color_buffer);
	glGenTextures(1, &texture_light_pos_rad_buffer);

	glBindTexture(GL_TEXTURE_BUFFER, texture_light_color_buffer);
	glBindTexture(GL_TEXTURE_BUFFER, texture_light_pos_rad_buffer);

	glBindTexture(GL_TEXTURE_BUFFER, 0);
}

void ComputeTileShading::PointLightManager::AddLight(PointLight data) 
{
	point_light_data.push_back(data);
}

void ComputeTileShading::PointLightManager::UpdateLight(void) 
{
	uint32_t size = point_light_data.size();
	glm::vec4 *data = new glm::vec4[size];
	for (register uint32_t i = 0; i < size; i++)data[i] = point_light_data[i].diffuse;

	glBindBuffer(GL_TEXTURE_BUFFER, light_color_buffer);
	glBufferSubData(GL_TEXTURE_BUFFER, 0, point_light_data.size() * sizeof(glm::vec4), data);


	for (register uint32_t i = 0; i < size; i++)data[i] = point_light_data[i].pos_radius;
	glBindBuffer(GL_TEXTURE_BUFFER, light_pos_rad_buffer);
	glBufferSubData(GL_TEXTURE_BUFFER, 0, point_light_data.size() * sizeof(glm::vec4), data);

	delete[] data;
}

void ComputeTileShading::PointLightManager::SetLightToBuffer(void) 
{
	glBindTexture(GL_TEXTURE_BUFFER, texture_light_color_buffer);
	glTexBuffer(GL_TEXTURE_BUFFER, GL_RGBA32F, 0);
	glBindTexture(GL_TEXTURE_BUFFER, texture_light_pos_rad_buffer);
	glTexBuffer(GL_TEXTURE_BUFFER, GL_RGBA32F, 0);

	glBindTexture(GL_TEXTURE_BUFFER, 0);
	////////////////////////////////////////////////////////////////////////////////////
	glDeleteBuffers(1, &light_color_buffer);
	glDeleteBuffers(1, &light_pos_rad_buffer);
	////////////////////////////////////////////////////////////////////////////////////
	glGenBuffers(1, &light_color_buffer);
	glGenBuffers(1, &light_pos_rad_buffer);
	////////////////////////////////////////////////////////////////////////////////////
	uint32_t size = point_light_data.size();
	glm::vec4 *data = new glm::vec4[size];

	for (register uint32_t i = 0; i < size; i++)
		data[i] = point_light_data[i].diffuse;
	glBindBuffer(GL_TEXTURE_BUFFER, light_color_buffer);
	glBufferData(GL_TEXTURE_BUFFER, point_light_data.size() * sizeof(glm::vec4), data, GL_STATIC_DRAW);
	
	for (register uint32_t i = 0; i < size; i++)data[i] = point_light_data[i].pos_radius;
	glBindBuffer(GL_TEXTURE_BUFFER, light_pos_rad_buffer);
	glBufferData(GL_TEXTURE_BUFFER, point_light_data.size() * sizeof(glm::vec4), data, GL_STATIC_DRAW);
	////////////////////////////////////////////////////////////////////////////////////
	glBindTexture(GL_TEXTURE_BUFFER, texture_light_color_buffer);
	glTexBuffer(GL_TEXTURE_BUFFER, GL_RGBA32F, light_color_buffer);

	glBindTexture(GL_TEXTURE_BUFFER, texture_light_pos_rad_buffer);
	glTexBuffer(GL_TEXTURE_BUFFER, GL_RGBA32F, light_pos_rad_buffer);
	////////////////////////////////////////////////////////////////////////////////////
	glBindTexture(GL_TEXTURE_BUFFER, 0);
}

int32_t ComputeTileShading::PointLightManager::GetLightCount(void) 
{
	return point_light_data.size();
}

GLuint ComputeTileShading::PointLightManager::GetPointLightBufferColor(void)
{
	return texture_light_color_buffer;
}

GLuint ComputeTileShading::PointLightManager::GetPointLightBufferPositionRadius(void)
{
	return texture_light_pos_rad_buffer;
}