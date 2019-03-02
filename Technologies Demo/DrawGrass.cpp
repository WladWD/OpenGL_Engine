#include "DrawGrass.h"


Atmosphere::DrawGrass::DrawGrass(MCamera::Camera *camera, Resource::MapResource *texture_map) : texture_map(texture_map), camera(camera)
{
	Resource::LoadTexture *load_texture = new Resource::LoadTexture(texture_map);

	grass_diffuse_id = load_texture->LoadResource("resource\\ground.dds");//starfield.dds"); night_sky.dds
	grass_normal_id = load_texture->LoadResource("resource\\ground.png");//starfield.dds"); night_sky.dds

	delete load_texture;

	grass_diffuse = texture_map->GetResourceByID(grass_diffuse_id).texture;
	grass_normal = texture_map->GetResourceByID(grass_normal_id).texture;

	shader_grass = new Shader::ShaderGrass();

	double scale = 2800.0f *128.0f *100.0f;
	world_matrix = glm::scale(glm::mat4(), glm::vec3(scale));

	GenerateQuad();
	InitShaderVariable();
}

Atmosphere::DrawGrass::~DrawGrass()
{
	texture_map->ReleaseResource(grass_diffuse_id);
	texture_map->ReleaseResource(grass_normal_id);

	delete shader_grass;

	glDeleteVertexArrays(1, &QUAD_VAO);
	glDeleteBuffers(1, &QUAD_VB);
	glDeleteBuffers(1, &QUAD_IB);
}

void Atmosphere::DrawGrass::Draw(const Atmosphere::GlobalLightingParam *atmosphere_parameter) 
{
	glDepthFunc(GL_LEQUAL);
	glDepthMask(GL_FALSE);////
	glBindVertexArray(QUAD_VAO);

	glUseProgram(shader_grass->GetProgram());

	shader_grass->SetWorld(world_matrix);
	shader_grass->SetProjView(camera->GetProjViewMatrix());
	shader_grass->SetCamPos(camera->GetPosition());

	shader_grass->SetLightDir(atmosphere_parameter->light_dir);
	shader_grass->SetLightDiffuse(atmosphere_parameter->light_diffuse_color);
	shader_grass->SetLightSpecular(atmosphere_parameter->light_specurar_color);

	shader_grass->SetMaterialDiffuse(vec4(0.6f, 1.0f, 0.2f, 0.8f));
	shader_grass->SetMaterialSpecular(vec4(0.2f, 1.0f, 0.1f, 10.8f));
	shader_grass->SetMaterialEmissive(vec4(0.6f, 1.0f, 0.2f, 10.8f));

	shader_grass->SetTextureDiffuse();
	glBindTexture(GL_TEXTURE_2D, grass_diffuse);

	shader_grass->SetTextureNormal();
	glBindTexture(GL_TEXTURE_2D, grass_normal);

	glDrawElementsBaseVertex(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, 0, 0);

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	glUseProgram(0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
	shader_grass->ResetProgram();
	glDepthMask(GL_TRUE);////
	glDepthFunc(GL_LESS);
}

void Atmosphere::DrawGrass::GenerateQuad(void) 
{
	float repead = 0.25f;//128.0f;//2800.0f * 128.0f;
	MeshLoad::MVertexTex verts[4] =
	{
		MeshLoad::MVertexTex{
		glm::vec3(1,0,1),
		glm::vec2(repead,repead) } ,

		MeshLoad::MVertexTex{
		glm::vec3(-1,0,1),
		glm::vec2(0,repead) },

		MeshLoad::MVertexTex{
		glm::vec3(-1,0,-1),
		glm::vec2(0,0) },

		MeshLoad::MVertexTex{
		glm::vec3(1,0,-1),
		glm::vec2(repead,0) }
	};

	uint8_t ib[6] = { 1, 0, 3, 3, 2, 1 };

	glGenVertexArrays(1, &QUAD_VAO);
	glBindVertexArray(QUAD_VAO);

	glCreateBuffers(1, &QUAD_IB);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, QUAD_IB);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(ib), ib, GL_STATIC_DRAW);

	glCreateBuffers(1, &QUAD_VB);
	glBindBuffer(GL_ARRAY_BUFFER, QUAD_VB);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);


	GLint artib1 = glGetAttribLocation(shader_grass->GetProgram(), "v_position");
	GLint artib2 = glGetAttribLocation(shader_grass->GetProgram(), "v_tex");

	uint32_t mvert_size = sizeof(MeshLoad::MVertexTex);

	glVertexAttribPointer(artib1, 3, GL_FLOAT, GL_FALSE, mvert_size, NULL);
	glVertexAttribPointer(artib2, 2, GL_FLOAT, GL_FALSE, mvert_size, (const void *)(sizeof(glm::vec3)));

	glEnableVertexAttribArray(artib1);
	glEnableVertexAttribArray(artib2);


	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Atmosphere::DrawGrass::InitShaderVariable(void) 
{
	glUseProgram(shader_grass->GetProgram());
	shader_grass->BindTextureToUnit();
	glUseProgram(0);

	shader_grass->ResetProgram();
}