#include "ShaderSkyPlaneUpdate.h"


Shader::ShaderSkyPlaneUpdate::ShaderSkyPlaneUpdate(void) : ShaderBase(1, 2)
{
	/*
	IDR_SHADER18            Shader                  "shader\\Atmosphere\\SkyPlaneUpdate.ps"

	IDR_SHADER19            Shader                  "shader\\Atmosphere\\SkyPlaneUpdate.vs"
	*/

	Shader_Resource shaders[2];
	shaders[0].resource_ID = IDR_SHADER19;
	shaders[1].resource_ID = IDR_SHADER18;

	shaders[0].type_shader = GL_VERTEX_SHADER;
	shaders[1].type_shader = GL_FRAGMENT_SHADER;

	shaders[0].define = 0;
	shaders[0].define_size = 0;

	shaders[1].define = 0;
	shaders[1].define_size = 0;

	CompileShader(shaders);

	InitProgram(this->shaders[0], this->shaders[1], program[0]);

	MInitShaderVariable();

	Noise_slot = 0;
}

Shader::ShaderSkyPlaneUpdate::~ShaderSkyPlaneUpdate() { }

void Shader::ShaderSkyPlaneUpdate::InitVariable(GLuint program)
{
	correct_programm

	g_proj_view[current_program] = glGetUniformLocation(program, to_shader_name(g_proj_view));
	m_offset[current_program] = glGetUniformLocation(program, to_shader_name(m_offset));
	CloudCover[current_program] = glGetUniformLocation(program, to_shader_name(CloudCover));
	CloudSharpness[current_program] = glGetUniformLocation(program, to_shader_name(CloudSharpness));

	NoiseTexture[current_program] = glGetUniformLocation(program, to_shader_name(NoiseTexture));
}

GLuint Shader::ShaderSkyPlaneUpdate::GetProgram(void)
{
	current_program = 0;
	return program[0];
}

void Shader::ShaderSkyPlaneUpdate::SetOffset(glm::vec3 offset)
{
	correct_programm
	mcheck(m_offset) glUniform3fv(m_offset[current_program], 1, glm::value_ptr(offset));
}

void Shader::ShaderSkyPlaneUpdate::SetCloudCover(float val)
{
	correct_programm
	mcheck(CloudCover) glUniform1f(CloudCover[current_program], val);
}

void Shader::ShaderSkyPlaneUpdate::SetCloudSharpness(float val)
{
	correct_programm
	mcheck(CloudSharpness) glUniform1f(CloudSharpness[current_program], val);
}

void Shader::ShaderSkyPlaneUpdate::SetNoiseTexture(void)
{
	correct_programm
	glActiveTexture(GL_TEXTURE0 + Noise_slot);
}

void Shader::ShaderSkyPlaneUpdate::BindTextureToUnit(void)
{
	correct_programm

	mcheck(NoiseTexture) glUniform1i(NoiseTexture[current_program], Noise_slot);
}

void Shader::ShaderSkyPlaneUpdate::SetProjView(glm::mat4 m_mat)
{
	correct_programm

	mcheck(g_proj_view) glUniformMatrix4fv(g_proj_view[current_program], 1, GL_FALSE, glm::value_ptr(m_mat));
}