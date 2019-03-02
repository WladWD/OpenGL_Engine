#include "DrawParticles.h"


Particle::DrawParticles::DrawParticles(MCamera::Camera *camera, Resource::MapResource *texture_map) : camera(camera), texture_map(texture_map)
{
	meter = camera->GetMeter();
	g_heightRange = 40.0f*meter;
	g_SpriteSize = 0.8f*meter;

	particle_update = new PartilceUpdate(camera);
	shader_particle_draw = new Shader::ShaderParticleDraw();

	g_ParticlesVertexArray = particle_update->GetVAOs();
	g_ParticlesBuffers = particle_update->GetVBOs();
	g_PatricleCount = particle_update->GetParticleCount();

	cicle = 1;

	InitDropArray();
	InitShaderTexture();
}

Particle::DrawParticles::~DrawParticles()
{
	delete particle_update;
	delete shader_particle_draw;

	texture_map->ReleaseResource(drop_tex_array_id);
}

void Particle::DrawParticles::InitAttrib(GLuint program, GLuint VBO, GLuint VAO)
{
	GLint artib1 = glGetAttribLocation(program, "v_pos");
	GLint artib2 = glGetAttribLocation(program, "v_seed");
	GLint artib3 = glGetAttribLocation(program, "v_speed");
	GLint artib4 = glGetAttribLocation(program, "v_random");
	GLint artib5 = glGetAttribLocation(program, "v_Type");

	uint32_t mvert_size = sizeof(RainVertex), offset = sizeof(glm::vec3);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glVertexAttribPointer(artib1, 3, GL_FLOAT, GL_FALSE, mvert_size, NULL);
	glVertexAttribPointer(artib2, 3, GL_FLOAT, GL_FALSE, mvert_size, (const void *)(offset));
	glVertexAttribPointer(artib3, 3, GL_FLOAT, GL_FALSE, mvert_size, (const void *)(offset * 2));
	glVertexAttribPointer(artib4, 1, GL_FLOAT, GL_FALSE, mvert_size, (const void *)(offset * 3));
	glVertexAttribPointer(artib5, 1, GL_UNSIGNED_INT, GL_FALSE, mvert_size, (const void *)(offset * 3 + sizeof(float)));

	glEnableVertexAttribArray(artib1);
	glEnableVertexAttribArray(artib2);
	glEnableVertexAttribArray(artib3);
	glEnableVertexAttribArray(artib4);
	glEnableVertexAttribArray(artib5);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(VAO);
}

void Particle::DrawParticles::InitShaderTexture(void)
{
	glUseProgram(shader_particle_draw->GetProgram());
	shader_particle_draw->BindTextureToUnit();
	glUseProgram(0);
}

void Particle::DrawParticles::InitDropArray(void)
{
	Resource::LoadTexture *tex = new Resource::LoadTexture(texture_map);

	drop_tex_array_id = tex->LoadTexture2DArray(string("resource\\drop\\rainTextures\\cv0_vPositive_.dds"), 370);

	delete tex;
}

void Particle::DrawParticles::Draw(glm::vec3 wind_dir, float dt_s, const Atmosphere::GlobalLightingParam *atmosphere_parameter,
	const Atmosphere::MoonLightingParam *moon_param, float intensity)
{
	//dt_s /= 64.0f;
	particle_update->Update(wind_dir, dt_s, intensity);
	/*      glBlendFunc(GL_SRC_ALPHA,GL_ONE);
    else
        glBlendFuncSeparate(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA,GL_ZERO,GL_ONE_MINUS_SRC_ALPHA);*/
	glUseProgram(shader_particle_draw->GetProgram());
	glDisable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glDepthMask(GL_FALSE);
//	glBlendFunc(GL_ONE_MINUS_SRC_ALPHA, GL_SRC_ALPHA);//to debug
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	InitAttrib(shader_particle_draw->GetProgram(), g_ParticlesBuffers[cicle], g_ParticlesVertexArray[cicle]);
	cicle ^= 1;

	/*float cos_angle = -atmosphere_parameter->light_dir.y;
	glm::vec4 sum_color = atmosphere_parameter->light_diffuse_color * glm::clamp(cos_angle * cos_angle * cos_angle, 0.0f, 1.0f);
	sum_color = glm::pow(sum_color, glm::vec4(0.1f));
	sum_color += moon_param->light_diffuse_color * (float)glm::clamp(glm::pow(atmosphere_parameter->star_intensity, 0.2), 0.0, 0.846) * glm::pow(glm::vec4(0.0509f, 0.0509f, 0.09803f, 1.0f), glm::vec4(0.55f));

	glm::vec4 mcolor = moon_param->light_diffuse_color;
	mcolor.a = atmosphere_parameter->star_intensity;*/
	shader_particle_draw->SetMoonLight(moon_param->light_diffuse_color);
	shader_particle_draw->SetSunLight(atmosphere_parameter->light_diffuse_color);
	shader_particle_draw->SetDtSecond(dt_s);
	shader_particle_draw->SetEyePos(camera->GetPosition());
	shader_particle_draw->SetProjViewWorld(camera->GetProjViewMatrix());
	shader_particle_draw->SetSpriteSize(g_SpriteSize);
	shader_particle_draw->SetTotalVel(wind_dir);

	shader_particle_draw->SetDropTextures();
	glBindTexture(GL_TEXTURE_2D_ARRAY, texture_map->GetResourceByID(drop_tex_array_id).texture);


	glDrawArrays(GL_POINTS, 0, g_PatricleCount*intensity);
	////////////////////////////////////////////////////////////////////////////////////////////

	glDepthMask(GL_TRUE);
	glDisable(GL_BLEND);
	glUseProgram(0);
	glBindVertexArray(0);

	glEnable(GL_CULL_FACE);
}
