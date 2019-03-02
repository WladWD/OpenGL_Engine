#include "ShaderSkyPlane.h"


Shader::ShaderSkyPlane::ShaderSkyPlane(void) : ShaderBase(1, 2)
{
	/*
	IDR_SHADER16            Shader                  "shader\\Atmosphere\\SkyPlane.ps"

	IDR_SHADER17            Shader                  "shader\\Atmosphere\\SkyPlane.vs"
	*/

	Shader_Resource shaders[2];
	shaders[0].resource_ID = IDR_SHADER17;
	shaders[1].resource_ID = IDR_SHADER16;

	shaders[0].type_shader = GL_VERTEX_SHADER;
	shaders[1].type_shader = GL_FRAGMENT_SHADER;

	shaders[0].define = 0;
	shaders[0].define_size = 0;

	shaders[1].define = 0;
	shaders[1].define_size = 0;

	CompileShader(shaders);

	InitProgram(this->shaders[0], this->shaders[1], program[0]);

	MInitShaderVariable();

	CloudSlot = 0;
}

Shader::ShaderSkyPlane::~ShaderSkyPlane() { }

void Shader::ShaderSkyPlane::InitVariable(GLuint program)
{
	correct_programm

	g_proj_view[current_program] = glGetUniformLocation(program, to_shader_name(g_proj_view));
	g_offset_vec[current_program] = glGetUniformLocation(program, to_shader_name(g_offset_vec));
	g_cloud_color[current_program] = glGetUniformLocation(program, to_shader_name(g_cloud_color));
	CloudCover[current_program] = glGetUniformLocation(program, to_shader_name(CloudCover));
	CloudSharpness[current_program] = glGetUniformLocation(program, to_shader_name(CloudSharpness));
	
	CloudTexture[current_program] = glGetUniformLocation(program, to_shader_name(CloudTexture));
}

GLuint Shader::ShaderSkyPlane::GetProgram(void)
{
	current_program = 0;
	return program[0];
}

void Shader::ShaderSkyPlane::SetProjView(glm::mat4 matrix) 
{
	correct_programm
	mcheck(g_proj_view) glUniformMatrix4fv(g_proj_view[current_program], 1, GL_FALSE, glm::value_ptr(matrix));
}

void Shader::ShaderSkyPlane::SetOffsetTex(glm::vec3 vector)
{
	correct_programm
	mcheck(g_offset_vec) glUniform3fv(g_offset_vec[current_program], 1, glm::value_ptr(vector));
}

void Shader::ShaderSkyPlane::SetCloudColor(glm::vec3 vector)
{
	correct_programm
	mcheck(g_cloud_color) glUniform3fv(g_cloud_color[current_program], 1, glm::value_ptr(vector));
}

void Shader::ShaderSkyPlane::SetCloudCover(float val) 
{
	correct_programm
	mcheck(CloudCover) glUniform1f(CloudCover[current_program], val);
}

void Shader::ShaderSkyPlane::SetCloudSharpness(float val)
{
	correct_programm
	mcheck(CloudSharpness) glUniform1f(CloudSharpness[current_program], val);
}

void Shader::ShaderSkyPlane::SetCloudTexture(void)
{
	correct_programm
	glActiveTexture(GL_TEXTURE0 + CloudSlot);
}

void Shader::ShaderSkyPlane::BindTextureToUnit(void)
{
	correct_programm

	mcheck(CloudTexture) glUniform1i(CloudTexture[current_program], CloudSlot);
}
