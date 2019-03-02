#include "ShaderMoon.h"


Shader::ShaderMoon::ShaderMoon(void) : ShaderBase(1, 2)
{
	/*
	IDR_SHADER12            Shader                  "shader\\Atmosphere\\Moon.ps"

	IDR_SHADER13            Shader                  "shader\\Atmosphere\\Moon.vs"
	*/

	Shader_Resource shaders[2];
	shaders[0].resource_ID = IDR_SHADER13;
	shaders[1].resource_ID = IDR_SHADER12;

	shaders[0].type_shader = GL_VERTEX_SHADER;
	shaders[1].type_shader = GL_FRAGMENT_SHADER;

	shaders[0].define = 0;
	shaders[0].define_size = 0;

	shaders[1].define = 0;
	shaders[1].define_size = 0;

	CompileShader(shaders);

	InitProgram(this->shaders[0], this->shaders[1], program[0]);

	MInitShaderVariable();

	MTexture_slot = 0;
}

Shader::ShaderMoon::~ShaderMoon() { }

void Shader::ShaderMoon::InitVariable(GLuint program)
{
	correct_programm

	g_proj_view_world[current_program] = glGetUniformLocation(program, to_shader_name(g_proj_view_world));
	g_world[current_program] = glGetUniformLocation(program, to_shader_name(g_world));
	g_alpha[current_program] = glGetUniformLocation(program, to_shader_name(g_alpha));

	MTexture[current_program] = glGetUniformLocation(program, to_shader_name(MTexture));
}

GLuint Shader::ShaderMoon::GetProgram(void)
{
	current_program = 0;
	return program[0];
}

void Shader::ShaderMoon::SetProjViewWorld(glm::mat4 matrix)
{
	correct_programm
	mcheck(g_proj_view_world) glUniformMatrix4fv(g_proj_view_world[current_program], 1, GL_FALSE, glm::value_ptr(matrix));
}

void Shader::ShaderMoon::SetWorld(glm::mat4 matrix)
{
	correct_programm
	mcheck(g_world) glUniformMatrix4fv(g_world[current_program], 1, GL_FALSE, glm::value_ptr(matrix));
}

void Shader::ShaderMoon::SetAlpha(float val)
{
	correct_programm
	mcheck(g_alpha) glUniform1f(g_alpha[current_program], val);
}

void Shader::ShaderMoon::SetTexture(void)
{
	correct_programm
	glActiveTexture(GL_TEXTURE0 + MTexture_slot);
}

void Shader::ShaderMoon::BindTextureToUnit(void)
{
	correct_programm
	mcheck(MTexture) glUniform1i(MTexture[current_program], MTexture_slot);
}
