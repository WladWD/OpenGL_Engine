#include "DrawSkyDome.h"

	
Atmosphere::DrawSkyDome::DrawSkyDome(GLuint StarTexture, GLuint RayLeighTexture, GLuint MieTexture) :
	RayLeighTexture(RayLeighTexture), MieTexture(MieTexture), StarTexture(StarTexture)
{
	DomeN = 32;

	shader_sky_dome = new Shader::ShaderSkyDome();

	GenerateDome();

	InitShaderVariable();
}

Atmosphere::DrawSkyDome::~DrawSkyDome()
{
	delete shader_sky_dome;

	glDeleteVertexArrays(1, &SD_VAO);
	glDeleteBuffers(1, &SD_VB);
	glDeleteBuffers(1, &SD_IB);
}

void Atmosphere::DrawSkyDome::GenerateDome(void)
{
	uint32_t Latitude = DomeN / 2;
	uint32_t Longitude = DomeN;

	DV_Size = Longitude * Latitude;
	DI_Size = (Longitude - 1) * (Latitude - 1) * 2;
	DV_Size *= 2;
	DI_Size *= 2;

	MeshLoad::MVertexTex *vertex_buffer = new MeshLoad::MVertexTex[DV_Size];
	uint16_t *index_buffer = new uint16_t[DI_Size * 3];

	int DomeIndex = 0;

	double scale = 2800.0f * 128.0f * 100.0f;

	double PI = glm::pi<double>();
	for (int i = 0; i < Longitude; i++)
	{
		double MoveXZ = 100.0f * (i / ((float)Longitude - 1.0f)) * PI / 180.0;

		for (int j = 0; j < Latitude; j++)
		{
			double MoveY = PI * j / (Latitude - 1);

			vertex_buffer[DomeIndex].l_pos.x = (float)(sin(MoveXZ) * cos(MoveY));
			vertex_buffer[DomeIndex].l_pos.y = (float)cos(MoveXZ);
			vertex_buffer[DomeIndex].l_pos.z = (float)(sin(MoveXZ) * sin(MoveY));

			vertex_buffer[DomeIndex].l_pos *= scale;

			vertex_buffer[DomeIndex].l_tex.x = 0.5f / (float)Longitude + i / (float)Longitude;
			vertex_buffer[DomeIndex].l_tex.y = 0.5f / (float)Latitude + j / (float)Latitude;

			DomeIndex++;
		}
	}
	for (int i = 0; i < Longitude; i++)
	{
		double MoveXZ = 100.0 * (i / (float)(Longitude - 1)) * PI / 180.0;

		for (int j = 0; j < Latitude; j++)
		{
			double MoveY = (PI * 2.0) - (PI * j / (Latitude - 1));

			vertex_buffer[DomeIndex].l_pos.x = (float)(sin(MoveXZ) * cos(MoveY));
			vertex_buffer[DomeIndex].l_pos.y = (float)cos(MoveXZ);
			vertex_buffer[DomeIndex].l_pos.z = (float)(sin(MoveXZ) * sin(MoveY));

			vertex_buffer[DomeIndex].l_pos *= scale;

			vertex_buffer[DomeIndex].l_tex.x = 0.5f / (float)Longitude + i / (float)Longitude;
			vertex_buffer[DomeIndex].l_tex.y = 0.5f / (float)Latitude + j / (float)Latitude;

			DomeIndex++;
		}
	}

	int index = 0;
	for (short i = 0; i < Longitude - 1; i++)
	{
		for (short j = 0; j < Latitude - 1; j++)
		{
			index_buffer[index++] = (uint16_t)(i * Latitude + j);
			index_buffer[index++] = (uint16_t)((i + 1) * Latitude + j);
			index_buffer[index++] = (uint16_t)((i + 1) * Latitude + j + 1);

			index_buffer[index++] = (uint16_t)((i + 1) * Latitude + j + 1);
			index_buffer[index++] = (uint16_t)(i * Latitude + j + 1);
			index_buffer[index++] = (uint16_t)(i * Latitude + j);
		}
	}
	uint16_t Offset = (uint16_t)(Latitude * Longitude);
	for (short i = 0; i < Longitude - 1; i++)
	{
		for (short j = 0; j < Latitude - 1; j++)
		{
			index_buffer[index++] = (uint16_t)(Offset + i * Latitude + j);
			index_buffer[index++] = (uint16_t)(Offset + (i + 1) * Latitude + j + 1);
			index_buffer[index++] = (uint16_t)(Offset + (i + 1) * Latitude + j);

			index_buffer[index++] = (uint16_t)(Offset + i * Latitude + j + 1);
			index_buffer[index++] = (uint16_t)(Offset + (i + 1) * Latitude + j + 1);
			index_buffer[index++] = (uint16_t)(Offset + i * Latitude + j);
		}
	}

	glGenVertexArrays(1, &SD_VAO);
	glBindVertexArray(SD_VAO);

	glCreateBuffers(1, &SD_IB);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, SD_IB);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, DI_Size * 3 * sizeof(uint16_t), index_buffer, GL_STATIC_DRAW);

	glCreateBuffers(1, &SD_VB);
	glBindBuffer(GL_ARRAY_BUFFER, SD_VB);
	glBufferData(GL_ARRAY_BUFFER, DV_Size * sizeof(MeshLoad::MVertexTex), vertex_buffer, GL_STATIC_DRAW);


	GLint artib1 = glGetAttribLocation(shader_sky_dome->GetProgram(), "v_position");
	GLint artib2 = glGetAttribLocation(shader_sky_dome->GetProgram(), "v_tex");

	uint32_t mvert_size = sizeof(MeshLoad::MVertexTex);

	glVertexAttribPointer(artib1, 3, GL_FLOAT, GL_FALSE, mvert_size, NULL);
	glVertexAttribPointer(artib2, 2, GL_FLOAT, GL_FALSE, mvert_size, (const void *)(sizeof(glm::vec3)));

	glEnableVertexAttribArray(artib1);
	glEnableVertexAttribArray(artib2);


	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	delete[] index_buffer;
	delete[] vertex_buffer;
}

void Atmosphere::DrawSkyDome::InitShaderVariable(void)
{
	glUseProgram(shader_sky_dome->GetProgram());
	shader_sky_dome->SetDefValue();
	shader_sky_dome->BindTextureToUnit();
	glUseProgram(0);
}

void Atmosphere::DrawSkyDome::Draw(glm::mat4 pvw, const Atmosphere::GlobalLightingParam *atmosphere_parameter, const ScatteringParam *scattering_param)
{
	glDepthMask(GL_FALSE);////

	glDepthFunc(GL_LEQUAL);

	glBindVertexArray(SD_VAO);

	glUseProgram(shader_sky_dome->GetProgram());

	shader_sky_dome->SetWavelengthMie(scattering_param->waveLengthsMie);
	shader_sky_dome->SetInvWavelength(scattering_param->invWaveLengths);
	shader_sky_dome->Setv3SunDir(-atmosphere_parameter->light_dir);

	shader_sky_dome->SetRayLeighTexture();
	glBindTexture(GL_TEXTURE_2D, RayLeighTexture);

	shader_sky_dome->SetMieTexture();
	glBindTexture(GL_TEXTURE_2D, MieTexture);

	shader_sky_dome->SetStarTexture();
	glBindTexture(GL_TEXTURE_2D, StarTexture);//GL_TEXTURE_2D GL_TEXTURE_CUBE_MAP

	shader_sky_dome->SetProjViewWorld(pvw);

	shader_sky_dome->SetNumSamples(10.0f);
	shader_sky_dome->SetExposure(-1.5f);//-1.5f);//-2.0f
	shader_sky_dome->SetStarIntensity(atmosphere_parameter->star_intensity);

	glDrawElementsBaseVertex(GL_TRIANGLES, DI_Size * 3, GL_UNSIGNED_SHORT, 0, 0);
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////
	glUseProgram(0);

	glBindVertexArray(0);

	glBindTexture(GL_TEXTURE_2D, 0);

	glDepthFunc(GL_LESS);
	glDepthMask(GL_TRUE);////
}