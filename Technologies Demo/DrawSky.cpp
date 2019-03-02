#include "DrawSky.h"


Atmosphere::DrawSky::DrawSky(Atmosphere::AtmosphereParameter *mvatmosphere_parameter, Resource::MapResource *texture_map) :
	mvatmosphere_parameter(mvatmosphere_parameter), tex_width(256), tex_height(128), texture_map(texture_map)
{
	Resource::LoadTexture *load_texture = new Resource::LoadTexture(texture_map);

	star_texture_id = load_texture->LoadResource("resource\\example.dds");//starfield.dds"); night_sky.dds
	moon_texture_id = load_texture->LoadResource("resource\\moon.tga");//starfield.dds"); night_sky.dds
	moonglow_texture_id = load_texture->LoadResource("resource\\moonglow.tga");//starfield.dds"); night_sky.dds

	/*glBindTexture(GL_TEXTURE_2D, texture_map->GetResourceByID(star_texture_id).texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glBindTexture(GL_TEXTURE_2D, 0);*/

	delete load_texture;

	InitFramebuffer();

	draw_sky_dome = new DrawSkyDome(texture_map->GetResourceByID(star_texture_id).texture, RayLeighTexture, MieTexture);
	draw_sky_quad = new DrawSkyQuad(texture_map->GetResourceByID(moon_texture_id).texture, texture_map->GetResourceByID(moonglow_texture_id).texture);
}

Atmosphere::DrawSky::~DrawSky()
{
	texture_map->ReleaseResource(star_texture_id);
	texture_map->ReleaseResource(moon_texture_id);
	texture_map->ReleaseResource(moonglow_texture_id);

	glDeleteTextures(1, &RayLeighTexture);
	glDeleteTextures(1, &MieTexture);
	glDeleteFramebuffers(1, &FrameBuffer_Update);

	delete draw_sky_quad;
	delete draw_sky_dome;
}

void Atmosphere::DrawSky::InitFramebuffer(void) 
{
	glGenTextures(1, &RayLeighTexture);
	glGenTextures(1, &MieTexture);
	////////////////////////////////////////////////////////////////////////////////////////////
	glBindTexture(GL_TEXTURE_2D, RayLeighTexture);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, tex_width, tex_height, 0, GL_RGBA, GL_FLOAT, NULL);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
	////////////////////////////////////////////////////////////////////////////////////////////
	glBindTexture(GL_TEXTURE_2D, MieTexture);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, tex_width, tex_height, 0, GL_RGBA, GL_FLOAT, NULL);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
	////////////////////////////////////////////////////////////////////////////////////////////
	glBindTexture(GL_TEXTURE_2D, 0);
	////////////////////////////////////////////////////////////////////////////////////////////
	glGenFramebuffers(1, &FrameBuffer_Update);

	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, FrameBuffer_Update);
	glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, RayLeighTexture, 0);
	glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, MieTexture, 0);

	GLenum res_val;
	if ((res_val = glCheckFramebufferStatus(GL_DRAW_FRAMEBUFFER)) != GL_FRAMEBUFFER_COMPLETE)
		MessageBox(NULL, L"Framebuffer is not complette", L"", MB_OK);

	if (GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT == res_val)
		MessageBox(NULL, L"Framebuffer is not complette", L"", MB_OK);

	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
}

void Atmosphere::DrawSky::UpdateRayLeighMieTexture(const Atmosphere::GlobalLightingParam *atmosphere_parameter)
{
	GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);

	GLint current_framebuffer;
	glGetIntegerv(GL_DRAW_FRAMEBUFFER_BINDING, &current_framebuffer);

	glEnable(GL_CULL_FACE);

	glDisable(GL_DEPTH_TEST);
	glDepthMask(GL_FALSE);

	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, FrameBuffer_Update);
	glViewport(0, 0, tex_width, tex_height);

	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	GLenum draw_buffers[2] = { GL_COLOR_ATTACHMENT0 , GL_COLOR_ATTACHMENT1 };
	glDrawBuffers(2, draw_buffers);

	draw_sky_quad->Draw(atmosphere_parameter, mvatmosphere_parameter->GetScatteringParameter());
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, current_framebuffer);
	//glDrawBuffer(GL_BACK);
	glViewport(viewport[0], viewport[1], viewport[2], viewport[3]);

	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);

	glBindTexture(GL_TEXTURE_2D, MieTexture);
	glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, RayLeighTexture);
	glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, 0);
}

void Atmosphere::DrawSky::Draw(glm::mat4 pvw, glm::vec3 cam_pos, const Atmosphere::GlobalLightingParam *atmosphere_parameter)
{
	//перед вызовом этого метода текущий кадровый буффер должен быть по умолчанию
	
	//glDisable(GL_CULL_FACE);
	//glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

	//glEnable(GL_DEPTH_TEST);
	//glDepthMask(GL_FALSE);

	if (atmosphere_parameter->need_update_atmosphere) UpdateRayLeighMieTexture(atmosphere_parameter);

	draw_sky_dome->Draw(pvw, atmosphere_parameter, mvatmosphere_parameter->GetScatteringParameter());
	draw_sky_quad->DrawMoon(atmosphere_parameter, pvw, cam_pos);
}
