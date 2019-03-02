#include "LightCompute.h"


ComputeTileShading::LightCompute::LightCompute(LightManager *light_manager, MCamera::Camera *camera, RenderResource::RenderResources<2, 1> *render_resources,
	const DeferredRendering::GBufferData &gbuffer_data) :
	camera(camera), render_resources(render_resources), gbuffer_data(gbuffer_data), light_manager(light_manager)
{
	height = camera->GetHeight();
	width = camera->GetWeight();

	shader_tile_shading = new Shader::ShaderComputeTileShading();

	Init();

	gPointLightColor = light_manager->GetPointLightBufferColor();
	gPointLightPositionRadius = light_manager->GetPointLightBufferPositionRadius();
}

void ComputeTileShading::LightCompute::Init(void)
{
	//uint32_t thread_x, thread_y, thread_z = 1;
	thread_z = 1;
	//thread_x = width >> 4;
	//thread_x += ((width % threadx_count) ? 1 : 0);

	thread_x = (width + thready_count - 1) / thready_count;
	thread_y = (height + thready_count - 1) / thready_count;

	/*thread_y = height >> 4;
	thread_y += ((height % thready_count) ? 1 : 0);*/

	glUseProgram(shader_tile_shading->GetProgram());
	shader_tile_shading->BindTextureToUnit();
	glUseProgram(0);
}

ComputeTileShading::LightCompute::~LightCompute()
{
	delete shader_tile_shading;
}

void ComputeTileShading::LightCompute::Draw(const Atmosphere::MoonLightingParam &light_moon, const Atmosphere::GlobalLightingParam &light_sun)
{
	glUseProgram(shader_tile_shading->GetProgram());
	/*
		void SetNumPointLight(int32_t count);
		void SetNumSpotLight(int32_t count);
		void SetDimension(glm::ivec2 dim);
		void SetInvProjViewInvViewport(glm::mat4 matrix);
		void SetTileCount(glm::ivec2 tile_count);*/
	/* 2.0f / (float)BackBufferDesc->Width, 0.0f,                                 0.0f, 0.0f,
                           0.0f,                               -2.0f / (float)BackBufferDesc->Height, 0.0f, 0.0f,
                           0.0f,                                0.0f,                                 1.0f, 0.0f,
                          -1.0f,                                1.0f,                                 0.0f, 1.0f*/

	glm::mat4 gViewport(2.0f / (float)camera->GetWeight(), 0.0f, 0.0f, 0.0f,
		0.0f, 2.0f / (float)camera->GetHeight(), 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		-1.0f, -1.0f, 0.0f, 1.0f);
	//vec2 mtest = vec2(gViewport * vec4(vec2( - 0.5f,  - 0.5f), 0.0f, 1.0f));
	//camera->GetWeight()-1, camera->GetHeight()-1
	shader_tile_shading->SetProjInvMatrix(glm::inverse(camera->GetProjMatrix()));
	shader_tile_shading->SetViewMatrix(camera->GetViewMatrix());
	shader_tile_shading->SetNumPointLight(light_manager->GetPointLightCount());
	shader_tile_shading->SetNumSpotLight(light_manager->GetSpotLightCount());
	shader_tile_shading->SetInvProjViewInvViewport(glm::inverse(camera->GetProjViewMatrix()) * gViewport);
	shader_tile_shading->SetTileCount(glm::ivec2(thread_x, thread_y));
	shader_tile_shading->SetDimension(glm::ivec2(camera->GetWeight(), camera->GetHeight()));

	shader_tile_shading->SetDepthTexture();
	glBindTexture(GL_TEXTURE_2D, gbuffer_data.g_buffer_depth);

	shader_tile_shading->SetDiffuseTexture();
	glBindTexture(GL_TEXTURE_2D, gbuffer_data.g_buffer_textures[1]);

	shader_tile_shading->SetNormalTexture();
	glBindTexture(GL_TEXTURE_2D, gbuffer_data.g_buffer_textures[0]);

	shader_tile_shading->SetSpecularTexture();
	glBindTexture(GL_TEXTURE_2D, gbuffer_data.g_buffer_textures[2]);

	shader_tile_shading->SetPointLightColor();
	glBindTexture(GL_TEXTURE_BUFFER, gPointLightColor);
	shader_tile_shading->SetPointLightPositionRadius();
	glBindTexture(GL_TEXTURE_BUFFER, gPointLightPositionRadius);

	ComputeTileShading::TileBufferSpot spot_texture_buffer = light_manager->GetSpotLightTextureBuffers();
	shader_tile_shading->SetSpotLightColor();
	glBindTexture(GL_TEXTURE_BUFFER, spot_texture_buffer.gLightColor);
	shader_tile_shading->SetSpotLightBoundingSphere();
	glBindTexture(GL_TEXTURE_BUFFER, spot_texture_buffer.gBoundingSphereCenterRadius);
	shader_tile_shading->SetSpotLightData();
	glBindTexture(GL_TEXTURE_BUFFER, spot_texture_buffer.gSpotLightDirertionCosineAngle);

	ComputeTileShading::ParalelLight light_data;

	light_data.diffuse = light_sun.light_diffuse_color; //*(1.0f - light_sun.star_intensity);
	light_data.specular = light_sun.light_specurar_color;// *(1.0f - light_sun.star_intensity);
	light_data.direction = light_sun.light_dir;
	shader_tile_shading->SetDayLightParam(light_data);

	light_data.diffuse = light_moon.light_diffuse_color;// *light_sun.star_intensity;// *(1.0f - light_sun.star_intensity);
	light_data.specular = light_moon.light_specurar_color;// *light_sun.star_intensity;// *(1.0f - light_sun.star_intensity);
	light_data.direction = light_moon.light_dir;
	shader_tile_shading->SetNightLightParam(light_data);

	shader_tile_shading->SetUniformBuffersData();
	shader_tile_shading->BindUniformBuffers();

	shader_tile_shading->SetInvProjView(glm::inverse(camera->GetProjViewMatrix()));
	shader_tile_shading->SetEyePosition(camera->GetPosition());

	shader_tile_shading->SetSpotLightShadowMap();
	uint32_t smCount;
	const glm::mat4 *gmSpotMatrixes;
	glBindTexture(GL_TEXTURE_2D_ARRAY, light_manager->GetSpotLightShadowMaps(smCount, gmSpotMatrixes));
	shader_tile_shading->SetSpotLightShadowMatrixes(gmSpotMatrixes, smCount);

	const glm::vec4 *gmVectors;
	shader_tile_shading->SetPointLightShadowMap();
	glBindTexture(GL_TEXTURE_CUBE_MAP_ARRAY, light_manager->GetPointLightShadowMaps(smCount, gmVectors));
	shader_tile_shading->SetPointLightShadowVectorsParam(gmVectors, smCount);

	glBindImageTexture(shader_tile_shading->GetOutDiffSlot(), render_resources->GetDiffuseTexture(), 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA8);
	glBindImageTexture(shader_tile_shading->GetOutSpecSlot(), render_resources->GetSpecularTexture(), 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA8);

	glDispatchCompute(thread_x, thread_y, thread_z);
	glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	glUseProgram(0);
	glBindImageTexture(shader_tile_shading->GetOutDiffSlot(), 0, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA8);
	glBindImageTexture(shader_tile_shading->GetOutSpecSlot(), 0, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA8);
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	glBindTexture(GL_TEXTURE_2D, render_resources->GetDiffuseTexture());
	glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, render_resources->GetSpecularTexture());
	glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindTexture(GL_TEXTURE_BUFFER, 0);
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
}