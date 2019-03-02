#include "PartilceUpdate.h"
#include "ShaderParticleDraw.h"
#include "AtmosphereParameter.h"
#include "LoadTexture.h"

#pragma once
namespace Particle
{
	class DrawParticles
	{
		Resource::MapResource *texture_map;
		////////////////////////////////////////////////////////////////////////////////
		float g_heightRange;
		float g_SpriteSize;
		float meter;
		uint32_t g_PatricleCount;
		////////////////////////////////////////////////////////////////////////////////
		bool first_run;
		uint8_t cicle;//индекс буффера в который будут записаны новые данные
		////////////////////////////////////////////////////////////////////////////////
		GLuint drop_tex_array_id;
		////////////////////////////////////////////////////////////////////////////////
		GLuint *g_ParticlesVertexArray;
		GLuint *g_ParticlesBuffers;
		////////////////////////////////////////////////////////////////////////////////
		MCamera::Camera *camera;
		PartilceUpdate *particle_update;
		////////////////////////////////////////////////////////////////////////////////
		Shader::ShaderParticleDraw *shader_particle_draw;
		////////////////////////////////////////////////////////////////////////////////
		void InitAttrib(GLuint program, GLuint VBO, GLuint VAO);
		////////////////////////////////////////////////////////////////////////////////
		void InitDropArray(void);
		void InitShaderTexture(void);
	public:
		DrawParticles(MCamera::Camera *camera, Resource::MapResource *texture_map);
		~DrawParticles();
		////////////////////////////////////////////////////////////////////////////////
		void Draw(glm::vec3 wind_dir, float dt_s, const Atmosphere::GlobalLightingParam *atmosphere_parameter,
			const Atmosphere::MoonLightingParam *moon_param, float intensity);
	};
}
//1)Добавить симуляцию ветра
//2)Попробовать загрузить текстуры
//3)Выполнить общуюю оптимизацию и оформить код

