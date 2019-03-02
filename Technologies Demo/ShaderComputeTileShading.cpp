#include "ShaderComputeTileShading.h"


Shader::ShaderComputeTileShading::ShaderComputeTileShading(void) : ShaderBase(1, 1)
{
	/*
	IDR_SHADER34            SHADER                  "shader\\Shading\\shading.compute.glsl"

	IDR_SHADER35            SHADER                  "shader\\Shading\\shd.glsl"
	*/

	Shader_Resource shaders[1];
	shaders[0].resource_ID = IDR_SHADER35;

	shaders[0].type_shader = GL_COMPUTE_SHADER;

	shaders[0].define = 0;
	shaders[0].define_size = 0;

	CompileShader(shaders);
	InitProgram(this->shaders[0], program[0]);

	MInitShaderVariable();

	g_depth_slot = 0;
	g_normal_slot = 1;
	g_diffuse_slot = 2;
	g_specular_slot = 3;
	
	g_pointlight_color_slot = 4;
	g_pointlight_pos_radius_slot = 5;

	g_spotlight_color_slot = 6;
	g_spotlight_bounding_sphere_pos_radius_slot = 7;
	g_spotlight_data = 8;

	g_smSpotShadow = 9;
	g_smPointShadow = 10;

	gout_diff_slot = 0;
	gout_spec_slot = 1;

	InitUniformBuffers();
}

Shader::ShaderComputeTileShading::~ShaderComputeTileShading() 
{
	glDeleteBuffers(1, uniform_buffers);
}

void Shader::ShaderComputeTileShading::InitVariable(GLuint program)
{
	correct_programm

	g_spec_albedro[current_program] = glGetUniformLocation(program, to_shader_name(g_spec_albedro));
	g_albedro[current_program] = glGetUniformLocation(program, to_shader_name(g_albedro));

	g_depth[current_program] = glGetUniformLocation(program, to_shader_name(g_depth));
	g_diffuse[current_program] = glGetUniformLocation(program, to_shader_name(g_diffuse));
	g_specular[current_program] = glGetUniformLocation(program, to_shader_name(g_specular));
	g_normal[current_program] = glGetUniformLocation(program, to_shader_name(g_normal));
	
	day_night[current_program] = glGetUniformBlockIndex(program, to_shader_name(day_night));
	g_inv_proj_view[current_program] = glGetUniformLocation(program, to_shader_name(g_inv_proj_view));
	g_vCameraPos[current_program] = glGetUniformLocation(program, to_shader_name(g_vCameraPos));
	
	g_PointColor[current_program] = glGetUniformLocation(program, to_shader_name(g_PointColor));
	g_PointPositionRadius[current_program] = glGetUniformLocation(program, to_shader_name(g_PointPositionRadius));

	g_SpotColor[current_program] = glGetUniformLocation(program, to_shader_name(g_SpotColor));
	g_SpotBoundingSpherePositionRadius[current_program] = glGetUniformLocation(program, to_shader_name(g_SpotBoundingSpherePositionRadius));
	g_SpotDirectionRadiusCosineAngle[current_program] = glGetUniformLocation(program, to_shader_name(g_SpotDirectionRadiusCosineAngle));


	g_mProjectionInv[current_program] = glGetUniformLocation(program, to_shader_name(g_mProjectionInv));
	g_mView[current_program] = glGetUniformLocation(program, to_shader_name(g_mView));
	g_uNumLights[current_program] = glGetUniformLocation(program, to_shader_name(g_uNumLights));
	g_sNumSpotLights[current_program] = glGetUniformLocation(program, to_shader_name(g_sNumSpotLights));
	g_sTexDim[current_program] = glGetUniformLocation(program, to_shader_name(g_sTexDim));
	g_mInvProjViewInvViewport[current_program] = glGetUniformLocation(program, to_shader_name(g_mInvProjViewInvViewport));
	gTileCount[current_program] = glGetUniformLocation(program, to_shader_name(gTileCount));

	gSpotLightShadowMap[current_program] = glGetUniformLocation(program, to_shader_name(gSpotLightShadowMap));
	g_mProjViewSpot[current_program] = glGetUniformLocation(program, to_shader_name(g_mProjViewSpot));

	gPointLightShadowMap[current_program] = glGetUniformLocation(program, to_shader_name(gPointLightShadowMap));
	g_mNF[current_program] = glGetUniformLocation(program, to_shader_name(g_mNF));
	/*vec3 direction;
	vec4 diffuse;
	vec4 specular;
	const int NUM = 4;
	const char * names[NUM] =
	{
		"light_data[0].direction",
		"light_data[0].diffuse",
		"light_data[0].specular",
		"light_data[1].direction"
	};

	GLuint	index[NUM];			// index for every variable
	int		offset[NUM], size[NUM];

	glGetUniformIndices(program, NUM, names, index);
	glGetActiveUniformsiv(program, NUM, index, GL_UNIFORM_OFFSET, offset);
	glGetActiveUniformsiv(program, NUM, index, GL_UNIFORM_SIZE, size);
	int debug = 0;*/
}

void Shader::ShaderComputeTileShading::InitUniformBuffers(void)
{
	glCreateBuffers(1, uniform_buffers);
	/////////////////////////////////////////////////////////////////////////////
	glBindBuffer(GL_UNIFORM_BUFFER, uniform_buffers[0]);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(light_data), light_data, GL_STATIC_DRAW);
	/////////////////////////////////////////////////////////////////////////////
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void Shader::ShaderComputeTileShading::BindUniformBuffers(void)
{
	correct_programm

	glBindBufferBase(GL_UNIFORM_BUFFER, day_night[current_program], uniform_buffers[0]);
}

void Shader::ShaderComputeTileShading::SetUniformBuffersData(void)
{
	correct_programm
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(light_data), light_data);
}

void Shader::ShaderComputeTileShading::SetPointLightShadowMap(void)
{
	correct_programm
		glActiveTexture(GL_TEXTURE0 + g_smPointShadow);
}

void Shader::ShaderComputeTileShading::SetPointLightShadowVectorsParam(const glm::vec4 mvec[], uint32_t count)
{
	correct_programm
		mcheck(g_mNF) glUniform4fv(g_mNF[current_program], count, glm::value_ptr(mvec[0]));
}

void Shader::ShaderComputeTileShading::SetEyePosition(glm::vec3 pos)
{
	correct_programm
	mcheck(g_vCameraPos) glUniform3fv(g_vCameraPos[current_program], 1, glm::value_ptr(pos));
}
/*	
		GLint g_sTexDim[mprog_count];
		GLint g_mInvProjViewInvViewport[mprog_count];
		GLint gTileCount[mprog_count];*/
void Shader::ShaderComputeTileShading::SetProjInvMatrix(glm::mat4 matrix) 
{
	correct_programm
	mcheck(g_mProjectionInv) glUniformMatrix4fv(g_mProjectionInv[current_program], 1, GL_FALSE, glm::value_ptr(matrix));
}

void Shader::ShaderComputeTileShading::SetViewMatrix(glm::mat4 view) 
{
	correct_programm
	mcheck(g_mView) glUniformMatrix4fv(g_mView[current_program], 1, GL_FALSE, glm::value_ptr(view));
}

void Shader::ShaderComputeTileShading::SetNumPointLight(int32_t count) 
{
	correct_programm
	mcheck(g_uNumLights) glUniform1i(g_uNumLights[current_program], count);
}

void Shader::ShaderComputeTileShading::SetNumSpotLight(int32_t count) 
{
	correct_programm
	mcheck(g_sNumSpotLights) glUniform1i(g_sNumSpotLights[current_program], count);
}

void Shader::ShaderComputeTileShading::SetDimension(glm::ivec2 dim) 
{
	correct_programm
	mcheck(g_sTexDim) glUniform2iv(g_sTexDim[current_program], 1, glm::value_ptr(dim));
}

void Shader::ShaderComputeTileShading::SetSpotLightShadowMatrixes(const glm::mat4 matrix[], uint32_t count)
{
	correct_programm
		mcheck(g_mProjViewSpot) glUniformMatrix4fv(g_mProjViewSpot[current_program], count, GL_FALSE, glm::value_ptr(matrix[0]));
}

void Shader::ShaderComputeTileShading::SetSpotLightShadowMap(void)
{
	glActiveTexture(GL_TEXTURE0 + g_smSpotShadow);
}

void Shader::ShaderComputeTileShading::SetInvProjViewInvViewport(glm::mat4 matrix)
{
	correct_programm
	mcheck(g_mInvProjViewInvViewport) glUniformMatrix4fv(g_mInvProjViewInvViewport[current_program], 1, GL_FALSE, glm::value_ptr(matrix));
}

void Shader::ShaderComputeTileShading::SetTileCount(glm::ivec2 tile_count)
{
	correct_programm
	mcheck(gTileCount) glUniform2iv(gTileCount[current_program], 1, glm::value_ptr(tile_count));
}

void Shader::ShaderComputeTileShading::SetDayLightParam(ComputeTileShading::ParalelLight data) 
{
	light_data[0] = data;
}

void Shader::ShaderComputeTileShading::SetNightLightParam(ComputeTileShading::ParalelLight data) 
{
	light_data[1] = data;
}

GLuint Shader::ShaderComputeTileShading::GetProgram(void)
{
	current_program = 0;
	return program[0];
}

void Shader::ShaderComputeTileShading::SetDepthTexture(void)
{
	correct_programm
	glActiveTexture(GL_TEXTURE0 + g_depth_slot);
}

void Shader::ShaderComputeTileShading::SetPointLightColor(void)
{
	correct_programm
	glActiveTexture(GL_TEXTURE0 + g_pointlight_color_slot);
}

void Shader::ShaderComputeTileShading::SetPointLightPositionRadius(void)
{
	correct_programm
	glActiveTexture(GL_TEXTURE0 + g_pointlight_pos_radius_slot);
}

void Shader::ShaderComputeTileShading::SetSpotLightColor(void)
{
	correct_programm
		glActiveTexture(GL_TEXTURE0 + g_spotlight_color_slot);
}

void Shader::ShaderComputeTileShading::SetSpotLightBoundingSphere(void)
{
	correct_programm
		glActiveTexture(GL_TEXTURE0 + g_spotlight_bounding_sphere_pos_radius_slot);
}

void Shader::ShaderComputeTileShading::SetSpotLightData(void) 
{
	correct_programm
		glActiveTexture(GL_TEXTURE0 + g_spotlight_data);
}

void Shader::ShaderComputeTileShading::SetNormalTexture(void) 
{
	correct_programm
	glActiveTexture(GL_TEXTURE0 + g_normal_slot);
}

void Shader::ShaderComputeTileShading::SetDiffuseTexture(void) 
{
	correct_programm
	glActiveTexture(GL_TEXTURE0 + g_diffuse_slot);
}

void Shader::ShaderComputeTileShading::SetSpecularTexture(void) 
{
	correct_programm
	glActiveTexture(GL_TEXTURE0 + g_specular_slot);
}

void Shader::ShaderComputeTileShading::SetInvProjView(glm::mat4 matrix)
{
	correct_programm
	mcheck(g_inv_proj_view) glUniformMatrix4fv(g_inv_proj_view[current_program], 1, GL_FALSE, glm::value_ptr(matrix));
}

GLint Shader::ShaderComputeTileShading::GetOutDiffSlot(void)
{
	return gout_diff_slot;
}

GLint Shader::ShaderComputeTileShading::GetOutSpecSlot(void) 
{
	return gout_spec_slot;
}

void Shader::ShaderComputeTileShading::BindTextureToUnit(void)
{
	correct_programm

	mcheck(g_depth) glUniform1i(g_depth[current_program], g_depth_slot);
	mcheck(g_normal) glUniform1i(g_normal[current_program], g_normal_slot);
	mcheck(g_diffuse) glUniform1i(g_diffuse[current_program], g_diffuse_slot);
	mcheck(g_specular) glUniform1i(g_specular[current_program], g_specular_slot);
	
	mcheck(g_PointColor) glUniform1i(g_PointColor[current_program], g_pointlight_color_slot);
	mcheck(g_PointPositionRadius) glUniform1i(g_PointPositionRadius[current_program], g_pointlight_pos_radius_slot);

	mcheck(g_SpotColor) glUniform1i(g_SpotColor[current_program], g_spotlight_color_slot);
	mcheck(g_SpotBoundingSpherePositionRadius) glUniform1i(g_SpotBoundingSpherePositionRadius[current_program], g_spotlight_bounding_sphere_pos_radius_slot);
	mcheck(g_SpotDirectionRadiusCosineAngle) glUniform1i(g_SpotDirectionRadiusCosineAngle[current_program], g_spotlight_data);

	mcheck(g_albedro) glUniform1i(g_albedro[current_program], gout_diff_slot);
	mcheck(g_spec_albedro) glUniform1i(g_spec_albedro[current_program], gout_spec_slot);

	mcheck(gSpotLightShadowMap) glUniform1i(gSpotLightShadowMap[current_program], g_smSpotShadow);
	mcheck(gPointLightShadowMap) glUniform1i(gPointLightShadowMap[current_program], g_smPointShadow);
}
