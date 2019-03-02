#include "ShaderParticleDraw.h"


Shader::ShaderParticleDraw::ShaderParticleDraw() : ShaderBase(1, 3)
{
	/*
	IDR_SHADER20            Shader                  "shader\\Rain\\ParticleDraw.gs"

	IDR_SHADER21            Shader                  "shader\\Rain\\ParticleDraw.ps"

	IDR_SHADER22            Shader                  "shader\\Rain\\ParticleDraw.vs"
	*/

	Shader_Resource shaders[3];
	shaders[0].resource_ID = IDR_SHADER22;
	shaders[1].resource_ID = IDR_SHADER20;
	shaders[2].resource_ID = IDR_SHADER21;

	shaders[0].type_shader = GL_VERTEX_SHADER;
	shaders[1].type_shader = GL_GEOMETRY_SHADER;
	shaders[2].type_shader = GL_FRAGMENT_SHADER;

	shaders[0].define = 0;
	shaders[0].define_size = 0;

	shaders[1].define = 0;
	shaders[1].define_size = 0;

	shaders[2].define = 0;
	shaders[2].define_size = 0;

	CompileShader(shaders);

	InitProgram(this->shaders[0], this->shaders[1], this->shaders[2], program[0]);

	g_drop_tex_slot = 0;

	MInitShaderVariable();
}

Shader::ShaderParticleDraw::~ShaderParticleDraw() { }

void Shader::ShaderParticleDraw::InitVariable(GLuint program)
{
	correct_programm

	g_SpriteSize[current_program] = glGetUniformLocation(program, to_shader_name(g_SpriteSize));
	g_dt_s[current_program] = glGetUniformLocation(program, to_shader_name(g_dt_s));
	g_TotalVel[current_program] = glGetUniformLocation(program, to_shader_name(g_TotalVel));
	g_eyePos[current_program] = glGetUniformLocation(program, to_shader_name(g_eyePos));
	g_mProjViewWorld[current_program] = glGetUniformLocation(program, to_shader_name(g_mProjViewWorld));
	g_SunLight[current_program] = glGetUniformLocation(program, to_shader_name(g_SunLight));
	g_MoonLight[current_program] = glGetUniformLocation(program, to_shader_name(g_MoonLight));
	g_drop_array[current_program] = glGetUniformLocation(program, to_shader_name(g_drop_array));
}

void Shader::ShaderParticleDraw::BindTextureToUnit(void)
{
	correct_programm
	mcheck(g_drop_array) glUniform1i(g_drop_array[current_program], g_drop_tex_slot);
}

void Shader::ShaderParticleDraw::SetDropTextures(void)
{
	glActiveTexture(GL_TEXTURE0 + g_drop_tex_slot);
}

GLuint Shader::ShaderParticleDraw::GetProgram(void)
{
	current_program = 0;
	return program[0];
}

void Shader::ShaderParticleDraw::SetProjViewWorld(glm::mat4 matrix) 
{
	correct_programm
	mcheck(g_mProjViewWorld) glUniformMatrix4fv(g_mProjViewWorld[current_program], 1, GL_FALSE, glm::value_ptr(matrix));
}

void Shader::ShaderParticleDraw::SetTotalVel(glm::vec3 vector) 
{
	correct_programm
	mcheck(g_TotalVel) glUniform3fv(g_TotalVel[current_program], 1, glm::value_ptr(vector));
}

void Shader::ShaderParticleDraw::SetEyePos(glm::vec3 vector) 
{
	correct_programm
	mcheck(g_eyePos) glUniform3fv(g_eyePos[current_program], 1, glm::value_ptr(vector));
}

void Shader::ShaderParticleDraw::SetDtSecond(float val)
{
	correct_programm
	mcheck(g_dt_s) glUniform1f(g_dt_s[current_program], val);
}

void Shader::ShaderParticleDraw::SetSunLight(glm::vec4 color) 
{
	correct_programm
	mcheck(g_SunLight) glUniform4fv(g_SunLight[current_program], 1, glm::value_ptr(color));
}

void Shader::ShaderParticleDraw::SetMoonLight(glm::vec4 color) 
{
	correct_programm
	mcheck(g_dt_s) glUniform4fv(g_MoonLight[current_program], 1, glm::value_ptr(color));
}

void Shader::ShaderParticleDraw::SetSpriteSize(float val) 
{
	correct_programm
	mcheck(g_SpriteSize) glUniform1f(g_SpriteSize[current_program], val);
}