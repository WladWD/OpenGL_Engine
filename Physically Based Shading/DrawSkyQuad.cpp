#include "DrawSkyQuad.h"


Atmosphere::DrawSkyQuad::DrawSkyQuad(GLuint MoonTexture, GLuint MoonGlowTexture) : MoonTexture(MoonTexture), MoonGlowTexture(MoonGlowTexture)
{
	shader_update_sky = new Shader::ShaderUpdateSky();
	shader_moon = new Shader::ShaderMoon();

	GenerateQuad();

	InitShaderVariable();
}

Atmosphere::DrawSkyQuad::~DrawSkyQuad()
{
	delete shader_update_sky;
	delete shader_moon;

	glDeleteVertexArrays(1, &QUAD_VAO);
	glDeleteBuffers(1, &QUAD_VB);
	glDeleteBuffers(1, &QUAD_IB);
}

void Atmosphere::DrawSkyQuad::GenerateQuad(void) 
{
	MeshLoad::MVertexTex verts[4] =
	{
		MeshLoad::MVertexTex{
		glm::vec3(1,1,0),
		glm::vec2(1,1) } ,

		MeshLoad::MVertexTex{
		glm::vec3(-1,1,0),
		glm::vec2(0,1) },

		MeshLoad::MVertexTex{
		glm::vec3(-1,-1,0),
		glm::vec2(0,0) },

		MeshLoad::MVertexTex{
		glm::vec3(1,-1,0),
		glm::vec2(1,0) }
	};

	uint8_t ib[6] = { 0, 1, 2, 2, 3, 0 };

	glGenVertexArrays(1, &QUAD_VAO);
	glBindVertexArray(QUAD_VAO);

	glCreateBuffers(1, &QUAD_IB);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, QUAD_IB);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(ib), ib, GL_STATIC_DRAW);

	glCreateBuffers(1, &QUAD_VB);
	glBindBuffer(GL_ARRAY_BUFFER, QUAD_VB);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);


	GLint artib1 = glGetAttribLocation(shader_update_sky->GetProgram(), "v_position");
	GLint artib2 = glGetAttribLocation(shader_update_sky->GetProgram(), "v_tex");

	uint32_t mvert_size = sizeof(MeshLoad::MVertexTex);

	glVertexAttribPointer(artib1, 3, GL_FLOAT, GL_FALSE, mvert_size, NULL);
	glVertexAttribPointer(artib2, 2, GL_FLOAT, GL_FALSE, mvert_size, (const void *)(sizeof(glm::vec3)));

	glEnableVertexAttribArray(artib1);
	glEnableVertexAttribArray(artib2);


	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Atmosphere::DrawSkyQuad::InitShaderVariable(void)
{
	glUseProgram(shader_update_sky->GetProgram());
	shader_update_sky->SetDefValue();
	glUseProgram(0);

	glUseProgram(shader_moon->GetProgram());
	shader_moon->BindTextureToUnit();
	glUseProgram(0);
}

glm::mat4 Atmosphere::DrawSkyQuad::BildWorldFormAngles(float fPhi, float fThetta, glm::vec3 light_dir, glm::vec3 cam_pos, float scale)
{
	glm::mat4 world;

	float a = -fPhi + glm::pi<float>() * 0.5f;
	float b = fThetta + glm::pi<float>() * 0.5f;

	glm::mat4 rotation_matrix = glm::mat4_cast(glm::quat(cos(a * 0.5f), glm::vec3(0.0f, sin(a * 0.5f), 0.0f))) *
		glm::mat4_cast(glm::quat(cos(b * 0.5f), glm::vec3(sin(b * 0.5f), 0.0f, 0.0f)));

	glm::mat4 translation_matrix = glm::translate(glm::mat4(), cam_pos) * glm::translate(glm::mat4(), light_dir * scale);
	//для отладки движения солнца glm::translate(glm::mat4(), -light_dir * 15.0f);
	//world = translation_matrix;//для отладки движения солнца

	world = translation_matrix * rotation_matrix;

	/*Matrix.CreateRotationX(this.Theta + (float)Math.PI / 2.0f) *
                Matrix.CreateRotationY(-this.Phi + (float)Math.PI / 2.0f) *
                Matrix.CreateTranslation(parameters.LightDirection.X * 15,
                parameters.LightDirection.Y * 15,
                parameters.LightDirection.Z * 15) *
                Matrix.CreateTranslation(camera.Position.X,
                camera.Position.Y,
                camera.Position.Z))*/

	return world;
}

void Atmosphere::DrawSkyQuad::Draw(const Atmosphere::GlobalLightingParam *atmosphere_parameter, const ScatteringParam *scattering_param)
{
	glBindVertexArray(QUAD_VAO);

	glUseProgram(shader_update_sky->GetProgram());

	shader_update_sky->SetWavelengthMie(scattering_param->waveLengthsMie);
	shader_update_sky->SetInvWavelength(scattering_param->invWaveLengths);
	shader_update_sky->Setv3SunDir(-atmosphere_parameter->light_dir);

	glDrawElementsBaseVertex(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, 0, 0);

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	glBindVertexArray(0);
}

void Atmosphere::DrawSkyQuad::DrawMoon(const Atmosphere::GlobalLightingParam *atmosphere_parameter, glm::mat4 proj_view, glm::vec3 cam_pos)
{
	//перед вызовом этого метоода 
	//CullFace - enable
	//glBlendEquation - default
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glDepthMask(GL_FALSE);////
	glDepthFunc(GL_LEQUAL);
	glCullFace(GL_FRONT);

	glBindVertexArray(QUAD_VAO);

	glUseProgram(shader_moon->GetProgram());

	glm::mat4 world = BildWorldFormAngles(atmosphere_parameter->Phi, atmosphere_parameter->Thetta, atmosphere_parameter->light_dir, cam_pos, 15.0f);
	glm::mat4 proj_view_world = proj_view * world;

	shader_moon->SetAlpha(glm::clamp(atmosphere_parameter->star_intensity * 16.0f, 0.0f, 1.0f));
	shader_moon->SetProjViewWorld(proj_view_world);
	shader_moon->SetWorld(world);
	shader_moon->SetTexture();
	glBindTexture(GL_TEXTURE_2D, MoonTexture);

	glDrawElementsBaseVertex(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, 0, 0);

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	DrawGlow(proj_view, cam_pos, atmosphere_parameter);

	//glDepthMask(GL_FALSE);////

	/*glBindVertexArray(0);

	glBindTexture(GL_TEXTURE_2D, 0);

	glDepthFunc(GL_LESS);
	glDisable(GL_BLEND);
	glCullFace(GL_BACK); */
}

void Atmosphere::DrawSkyQuad::DrawGlow(glm::mat4 proj_view, glm::vec3 cam_pos, const Atmosphere::GlobalLightingParam *atmosphere_parameter)
{
	glUseProgram(shader_moon->GetProgram());

	glm::mat4 world = BildWorldFormAngles(atmosphere_parameter->Phi, atmosphere_parameter->Thetta, atmosphere_parameter->light_dir, cam_pos, 5.0f);
	glm::mat4 proj_view_world = proj_view * world;

	shader_moon->SetAlpha(atmosphere_parameter->star_intensity);
	shader_moon->SetProjViewWorld(proj_view_world);
	shader_moon->SetWorld(world);
	shader_moon->SetTexture();
	glBindTexture(GL_TEXTURE_2D, MoonGlowTexture);

	glDrawElementsBaseVertex(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, 0, 0);

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	glBindVertexArray(0);

	glUseProgram(0);

	glBindTexture(GL_TEXTURE_2D, 0);

	glDepthFunc(GL_LESS);
	glDisable(GL_BLEND);
	glCullFace(GL_BACK);
	glDepthMask(GL_TRUE);////
}