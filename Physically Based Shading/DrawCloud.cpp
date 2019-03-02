#include "DrawCloud.h"


Atmosphere::DrawCloud::DrawCloud(void)
{
	PlaneN = 33;
	max_cover = 3.8f;
	min_cover = 0.8f;

	shader_sky_plane = new Shader::ShaderSkyPlane();
	//shader_sky_plane_update = new Shader::ShaderSkyPlaneUpdate();

	InitShaderVariable();

	GenerateSkyPlane();
	//GenerateUpdatePlane();

	InitNoiseTexture();
	//InitFramebuffer();
}

Atmosphere::DrawCloud::~DrawCloud()
{
	delete shader_sky_plane;
	//delete shader_sky_plane_update;

	glDeleteTextures(1, &NoiseTexture);

	glDeleteVertexArrays(1, &SD_VAO);
	glDeleteBuffers(1, &SD_VB);
	glDeleteBuffers(1, &SD_IB);

	/*glDeleteVertexArrays(1, &QD_VAO);
	glDeleteBuffers(1, &QD_VB);
	glDeleteBuffers(1, &QD_IB);

	glDeleteTextures(1, &CloudTexture);
	glDeleteFramebuffers(1, &CloudFrameBuffer);*/
}

void Atmosphere::DrawCloud::GenerateSkyPlane(void)
{
	uint32_t Latitude = PlaneN;
	uint32_t Longitude = PlaneN;
	int32_t nd2 = PlaneN >> 1 + 1;
	int32_t nd2n = PlaneN >> 1;
	int32_t nm1 = PlaneN - 1;
	double dnd2 = double(nd2n);

	DV_Size = Longitude * Longitude;
	DI_Size = (Longitude - 1) * (Latitude - 1) * 2 * 3;

	MeshLoad::MVertexTex *vertex_buffer = new MeshLoad::MVertexTex[DV_Size];
	uint16_t *index_buffer = new uint16_t[DI_Size];

	int DomeIndex = 0;

	double h = 2800.0f * 128.0f * 10.0f;//200.0f;
	double R = 2800.0f * 128.0f * 100.0f;//---------800.0f;// *128.0f * 100.0f;
	double a = h - R;//200.0f;// * 128.0f;
	double c = a*a - R*R;
	double aa = a*a;

	double mpLa = double(nd2n);//PlaneN - 1) * 0.5f;
	double mp_one_div_La = 1.0f / mpLa;

	double gr = sqrt(-c);
	double mpi = glm::pi<double>();
	for (int i = 0; i < Longitude; i++)
	{
		for (int j = 0; j < Latitude; j++)
		{
			double mx = double(i - nd2n) * mp_one_div_La;
			double mz = double(j - nd2n) * mp_one_div_La;//(double(j) - mpLa) * mp_one_div_La;
			mx *= gr;
			mz *= gr;

			double ix = i <= nd2n ? double(i) / dnd2 : double(nm1 - i) / dnd2;
			double jz = j <= nd2n ? double(j) / dnd2 : double(nm1 - j) / dnd2;

			//ix = pow(ix, 0.5f);
			//jz = pow(jz, 0.5f);

			double yx = a + sqrt(aa - c - mx*mx);
			double yz = a + sqrt(aa - c - mz*mz); //sqrt(2 * a * mz - mz * mz + c);
			//double lp = min(ix, jz);

			double x_cos = cos(mpi * ix);
			x_cos *= 0.5;
			x_cos += 0.5;
			x_cos = 1.0 - x_cos;

			double z_cos = cos(mpi * jz);
			z_cos *= 0.5;
			z_cos += 0.5;
			z_cos = 1.0 - z_cos;

			double my = (z_cos*yx + x_cos*yz)*0.5f;//(lp * yx + lp * yz) * 0.5;

			vertex_buffer[DomeIndex].l_pos = glm::vec3(mx, my, mz);

			vertex_buffer[DomeIndex].l_tex.x = double(i) / double(nm1);
			vertex_buffer[DomeIndex].l_tex.y = double(j) / double(nm1);

			DomeIndex++;
		}
	}

	int index = 0;
	for (short i = 0; i < Longitude - 1; i++)
	{
		for (short j = 0; j < Latitude - 1; j++)
		{
			index_buffer[index++] = (uint16_t)((i + 1) * Latitude + j + 1);
			index_buffer[index++] = (uint16_t)((i + 1) * Latitude + j);
			index_buffer[index++] = (uint16_t)(i * Latitude + j);

			index_buffer[index++] = (uint16_t)(i * Latitude + j);
			index_buffer[index++] = (uint16_t)(i * Latitude + j + 1);
			index_buffer[index++] = (uint16_t)((i + 1) * Latitude + j + 1);
		}
	}

	glGenVertexArrays(1, &SD_VAO);
	glBindVertexArray(SD_VAO);

	glCreateBuffers(1, &SD_IB);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, SD_IB);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, DI_Size * sizeof(uint16_t), index_buffer, GL_STATIC_DRAW);

	glCreateBuffers(1, &SD_VB);
	glBindBuffer(GL_ARRAY_BUFFER, SD_VB);
	glBufferData(GL_ARRAY_BUFFER, DV_Size * sizeof(MeshLoad::MVertexTex), vertex_buffer, GL_STATIC_DRAW);


	GLint artib1 = glGetAttribLocation(shader_sky_plane->GetProgram(), "v_position");
	GLint artib2 = glGetAttribLocation(shader_sky_plane->GetProgram(), "v_tex");

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
/*
void Atmosphere::DrawCloud::GenerateUpdatePlane(void)
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

	glGenVertexArrays(1, &QD_VAO);
	glBindVertexArray(QD_VAO);

	glCreateBuffers(1, &QD_IB);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, QD_IB);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(ib), ib, GL_STATIC_DRAW);

	glCreateBuffers(1, &QD_VB);
	glBindBuffer(GL_ARRAY_BUFFER, QD_VB);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);


	GLint artib1 = glGetAttribLocation(shader_sky_plane_update->GetProgram(), "v_position");
	GLint artib2 = glGetAttribLocation(shader_sky_plane_update->GetProgram(), "v_tex");

	uint32_t mvert_size = sizeof(MeshLoad::MVertexTex);

	glVertexAttribPointer(artib1, 3, GL_FLOAT, GL_FALSE, mvert_size, NULL);
	glVertexAttribPointer(artib2, 2, GL_FLOAT, GL_FALSE, mvert_size, (const void *)(sizeof(glm::vec3)));

	glEnableVertexAttribArray(artib1);
	glEnableVertexAttribArray(artib2);


	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	shader_sky_plane_update->ResetProgram();
}

void Atmosphere::DrawCloud::InitFramebuffer(void)
{
	glGenTextures(1, &CloudTexture);
	////////////////////////////////////////////////////////////////////////////////////////////
	glBindTexture(GL_TEXTURE_2D, CloudTexture);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, m_tex_dim, m_tex_dim, 0, GL_RGBA, GL_FLOAT, NULL);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
	////////////////////////////////////////////////////////////////////////////////////////////
	glBindTexture(GL_TEXTURE_2D, 0);
	////////////////////////////////////////////////////////////////////////////////////////////
	glGenFramebuffers(1, &CloudFrameBuffer);

	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, CloudFrameBuffer);
	glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, CloudTexture, 0);
	
	GLenum res_val;
	if ((res_val = glCheckFramebufferStatus(GL_DRAW_FRAMEBUFFER)) != GL_FRAMEBUFFER_COMPLETE)
		MessageBox(NULL, L"Framebuffer is not complette", L"", MB_OK);

	if (GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT == res_val)
		MessageBox(NULL, L"Framebuffer is not complette", L"", MB_OK);

	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
}
*/
void Atmosphere::DrawCloud::InitShaderVariable(void)
{
	glUseProgram(shader_sky_plane->GetProgram());
	shader_sky_plane->BindTextureToUnit();
	glUseProgram(0);
	//shader_sky_plane->ResetProgram();

	/*glUseProgram(shader_sky_plane_update->GetProgram());
	shader_sky_plane_update->BindTextureToUnit();
	glUseProgram(0);*/
	//shader_sky_plane_update->ResetProgram();
}

int Atmosphere::DrawCloud::xor128(void)
{
	static int x = 123456789;
	static int y = 362436069;
	static int z = 521288629;
	static int w = 88675123;
	int t;
	t = x ^ (x << 11);
	x = y; y = z; z = w;
	return w = w ^ (w >> 19) ^ t ^ (t >> 8);
}

double Atmosphere::DrawCloud::Noise2D (int x,int y)
 {
 int n = x + y*57;
 n = (n << 13) ^ n;
 return ( 1.0f - ( (n * (n * n * 15731 + 789221) + 1376312589) & 0x7fffffff) / 1073741824.0f);
 } 

uint8_t Atmosphere::DrawCloud::MSample(double a)
{
	a *= 0.5;
	a += 0.5;
	return a * 255;
}

void Atmosphere::DrawCloud::InitNoiseTexture(void)
{
#define dim 16
#define msize (dim * dim * 4)
	uint8_t *pixels = new uint8_t[msize];

	//for (register uint32_t i = 0; i < dim; i++)
	{
		for (register uint32_t j = 0; j < dim; j++)
		{
			for (register uint32_t k = 0; k < dim; k++)
			{
				uint32_t offset = ((j*dim) + k)*4;//((i * dim*dim) + 
				pixels[offset + 0] = MSample(Noise2D(xor128(), xor128()));//rand() % 255;
				pixels[offset + 1] = MSample(Noise2D(xor128(), xor128()));
				pixels[offset + 2] = MSample(Noise2D(xor128(), xor128()));
				pixels[offset + 3] = MSample(Noise2D(xor128(), xor128()));
	
			}
		}
	}

	//for (register uint32_t i = 1; i < dim-1; i++)
	//{
	//	for (register uint32_t j = 1; j < dim-1; j++)
	//	{
	//		for (register uint32_t k = 1; k < dim-1; k++)
	//		{
	//			
	//			uint32_t offset0 = (((i) * dim*dim) + ((j)*dim) + (k)) * 4;
	//			uint32_t offset1 = (((i) * dim*dim) + ((j)*dim) + (k+1)) * 4;
	//			uint32_t offset2 = (((i) * dim*dim) + ((j+1)*dim) + (k)) * 4;
	//			uint32_t offset3 = (((i) * dim*dim) + ((j+1)*dim) + (k+1)) * 4;
	//			uint32_t offset4 = (((i+1) * dim*dim) + ((j)*dim) + (k)) * 4;
	//			uint32_t offset5 = (((i+1) * dim*dim) + ((j)*dim) + (k+1)) * 4;
	//			uint32_t offset6 = (((i+1) * dim*dim) + ((j+1)*dim) + (k)) * 4;
	//			uint32_t offset7 = (((i+1) * dim*dim) + ((j+1)*dim) + (k+1)) * 4;
	//			int val = (int)(pixels[offset1 + 0] + pixels[offset2 + 0] + pixels[offset3 + 0] + pixels[offset4 + 0]);
	//			val += (int)(pixels[offset5 + 0] + pixels[offset6 + 0] + pixels[offset7 + 0] + pixels[offset0 + 0]);
	//			val >>= 3;
	//			pixels[offset0 + 0] = val;
	//		}
	//	}
	//}
	//for (register uint32_t i = 0; i < dim; i++)
	//{
	//	for (register uint32_t j = 1; j < dim - 1; j++)
	//	{
	//		uint32_t offset0 = (((i)* dim*dim) + ((j)*dim) + (0)) * 4;
	//		uint32_t offset1 = (((i)* dim*dim) + ((j)*dim) + (dim - 1)) * 4;
	//		uint32_t offset2 = (((i)* dim*dim) + ((0)*dim) + (j)) * 4;
	//		uint32_t offset3 = (((i)* dim*dim) + ((dim - 1)*dim) + (j)) * 4;

	//		pixels[offset0 + 0] = (int)(pixels[offset0 + 0] + pixels[offset1 + 0]) >> 1;
	//		pixels[offset1 + 0] = pixels[offset0 + 0];
	//		pixels[offset2 + 0] = (int)(pixels[offset2 + 0] + pixels[offset3 + 0]) >> 1;
	//		pixels[offset3 + 0] = pixels[offset2 + 0];

	//		/*for (register uint32_t k = 1; k < dim - 1; k++)
	//		{

	//			uint32_t offset0 = (((i)* dim*dim) + ((j)*dim) + (k)) * 4;
	//			uint32_t offset1 = (((i)* dim*dim) + ((j)*dim) + (k + 1)) * 4;
	//			uint32_t offset2 = (((i)* dim*dim) + ((j + 1)*dim) + (k)) * 4;
	//			uint32_t offset3 = (((i)* dim*dim) + ((j + 1)*dim) + (k + 1)) * 4;
	//			uint32_t offset4 = (((i + 1) * dim*dim) + ((j)*dim) + (k)) * 4;
	//			uint32_t offset5 = (((i + 1) * dim*dim) + ((j)*dim) + (k + 1)) * 4;
	//			uint32_t offset6 = (((i + 1) * dim*dim) + ((j + 1)*dim) + (k)) * 4;
	//			uint32_t offset7 = (((i + 1) * dim*dim) + ((j + 1)*dim) + (k + 1)) * 4;
	//			int val = (int)(pixels[offset1 + 0] + pixels[offset2 + 0] + pixels[offset3 + 0] + pixels[offset4 + 0]);
	//			val += (int)(pixels[offset5 + 0] + pixels[offset6 + 0] + pixels[offset7 + 0] + pixels[offset0 + 0]);
	//			val >>= 3;
	//			pixels[offset0 + 0] = val;
	//		}*/
	//	}
	//}

	glGenTextures(1, &NoiseTexture);
	glBindTexture(GL_TEXTURE_2D, NoiseTexture);
	
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, dim, dim, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);//GL_REPEAT
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);//GL_MIRRORED_REPEAT
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_REPEAT);//GL_MIRRORED_REPEAT
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);

	glGenerateMipmap(GL_TEXTURE_2D);
	
	glBindTexture(GL_TEXTURE_2D, 0);
	delete[] pixels;
}

//void Atmosphere::DrawCloud::UpdateCloud(float global_time_ms, glm::mat4 pvw)
//{
//	GLint viewport[4];
//	glGetIntegerv(GL_VIEWPORT, viewport);
//
//	glDepthFunc(GL_LEQUAL);
//
//	glBindVertexArray(SD_VAO);
//
//	glDisable(GL_CULL_FACE);
//	//glDisable(GL_BLEND);
//	glEnable(GL_BLEND);
//	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//
//	/*glEnable(GL_CULL_FACE);
//
//	glDisable(GL_DEPTH_TEST);
//	glDepthMask(GL_FALSE);*/
//
//	/*//glBindFramebuffer(GL_DRAW_FRAMEBUFFER, CloudFrameBuffer);
//	glViewport(0, 0, m_tex_dim, m_tex_dim);
//
//	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
//	glClear(GL_COLOR_BUFFER_BIT);
//
//	GLenum draw_buffers[1] = { GL_COLOR_ATTACHMENT0 };
//	glDrawBuffers(1, draw_buffers);
//	*/
//	//glBindVertexArray(SD_VAO);// QD_VAO);
//	
//	glUseProgram(shader_sky_plane_update->GetProgram());
//
//	shader_sky_plane_update->SetProjView(pvw);
//	shader_sky_plane_update->SetCloudCover(1.6f);//1.0f);
//	shader_sky_plane_update->SetCloudSharpness(0.001f);
//	shader_sky_plane_update->SetNoiseTexture();
//	glBindTexture(GL_TEXTURE_3D, NoiseTexture);
//	shader_sky_plane_update->SetOffset(global_time_ms * glm::vec3(0.00000001, 0.000002f, 0.000001f));
//
//	glDrawElementsBaseVertex(GL_TRIANGLES, DI_Size, GL_UNSIGNED_SHORT, 0, 0);// 6, GL_UNSIGNED_BYTE, 0, 0);
//
//	//shader_sky_plane_update->ResetProgram();
//	glBindVertexArray(0);
//
//	//glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
//	//glDrawBuffer(GL_BACK);
//	//glViewport(viewport[0], viewport[1], viewport[2], viewport[3]);
//	glDepthFunc(GL_LESS);
//	glEnable(GL_DEPTH_TEST);
//	glDepthMask(GL_TRUE);
//
//	glBindTexture(GL_TEXTURE_3D, 0);
//	//glBindTexture(GL_TEXTURE_2D, CloudTexture);
//	//glGenerateMipmap(GL_TEXTURE_2D);
//
//	glBindTexture(GL_TEXTURE_2D, 0);
//	glDisable(GL_BLEND);
//}

void Atmosphere::DrawCloud::Draw(float global_time_ms, glm::mat4 pvw, const Atmosphere::GlobalLightingParam *atmosphere_parameter, 
	const Atmosphere::MoonLightingParam *moon_param, float cloud_density, float cloud_gray_scale)
{
	glDepthFunc(GL_LEQUAL);
	glDepthMask(GL_FALSE);////
	glBindVertexArray(SD_VAO);

	glCullFace(GL_FRONT);
	//glDisable(GL_CULL_FACE);
	//glDisable(GL_BLEND);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glUseProgram(shader_sky_plane->GetProgram());

	//shader_sky_plane->SetNoiseTexture();
	//glBindTexture(GL_TEXTURE_2D, CloudTexture);
	float cos_angle = -atmosphere_parameter->light_dir.y;
	glm::vec4 sum_color = atmosphere_parameter->light_diffuse_color * glm::clamp(cos_angle * cos_angle * cos_angle, 0.0f, 1.0f);
	sum_color = glm::pow(sum_color, glm::vec4(0.1f));
	sum_color += moon_param->light_diffuse_color * (float)glm::clamp(glm::pow(atmosphere_parameter->star_intensity, 0.2), 0.0, 0.846) * glm::pow(glm::vec4(0.0509f, 0.0509f, 0.09803f, 1.0f), glm::vec4(0.55f));
	//346
	//0.0509 0.0509 0.09803
	shader_sky_plane->SetCloudColor(glm::vec3(sum_color.r, sum_color.g, sum_color.b) * cloud_gray_scale);
	shader_sky_plane->SetProjView(pvw);
	//float max_cover = 3.8f;
	//float min_cover = 0.8f;
	shader_sky_plane->SetCloudCover(min_cover + (max_cover- min_cover)*cloud_density);// 1.6f);//1.0f);
	shader_sky_plane->SetCloudSharpness(0.001f);
	shader_sky_plane->SetCloudTexture();
	glBindTexture(GL_TEXTURE_2D, NoiseTexture);
	shader_sky_plane->SetOffsetTex(global_time_ms * glm::vec3(0.00002, 0.00002, 0.00000000f));

	glDrawElementsBaseVertex(GL_TRIANGLES, DI_Size, GL_UNSIGNED_SHORT, 0, 0);

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	glUseProgram(0);
	glBindVertexArray(0);

	glBindTexture(GL_TEXTURE_2D, 0);

	glCullFace(GL_BACK);
	glDisable(GL_BLEND);
	//glEnable(GL_CULL_FACE);

	glDepthFunc(GL_LESS);
	glDepthMask(GL_TRUE);////
	shader_sky_plane->ResetProgram();
}