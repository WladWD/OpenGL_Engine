#include "ShaderGrass.h"


Shader::ShaderGrass::ShaderGrass() : ShaderBase(1, 2)
{
	/*
	IDR_SHADER14            Shader                  "shader\\Grass\\grass.ps"

	IDR_SHADER15            Shader                  "shader\\Grass\\grass.vs"
	*/

	Shader_Resource shaders[2];
	shaders[0].resource_ID = IDR_SHADER15;
	shaders[1].resource_ID = IDR_SHADER14;

	shaders[0].type_shader = GL_VERTEX_SHADER;
	shaders[1].type_shader = GL_FRAGMENT_SHADER;

	shaders[0].define = 0;
	shaders[0].define_size = 0;

	shaders[1].define = 0;
	shaders[1].define_size = 0;

	CompileShader(shaders);

	InitProgram(this->shaders[0], this->shaders[1], program[0]);

	diff_slot = 0;
	norm_slot = 1;

	MInitShaderVariable();
}

Shader::ShaderGrass::~ShaderGrass() { }

void Shader::ShaderGrass::InitVariable(GLuint program)
{
	correct_programm

	gProjView[current_program] = glGetUniformLocation(program, to_shader_name(gProjView));
	gWorld[current_program] = glGetUniformLocation(program, to_shader_name(gWorld));
	g_cam_pos[current_program] = glGetUniformLocation(program, to_shader_name(g_cam_pos));
	g_StarIntensity[current_program] = glGetUniformLocation(program, to_shader_name(g_StarIntensity));

	m_diffuse[current_program] = glGetUniformLocation(program, to_shader_name(m_diffuse));
	m_specular[current_program] = glGetUniformLocation(program, to_shader_name(m_specular));
	m_emissive[current_program] = glGetUniformLocation(program, to_shader_name(m_emissive));

	g_diffuse[current_program] = glGetUniformLocation(program, to_shader_name(g_diffuse));
	g_normal[current_program] = glGetUniformLocation(program, to_shader_name(g_normal));

	g_lightDir[current_program] = glGetUniformLocation(program, to_shader_name(g_lightDir));
	g_lightDiffuse[current_program] = glGetUniformLocation(program, to_shader_name(g_lightDiffuse));
	g_lightSpecular[current_program] = glGetUniformLocation(program, to_shader_name(g_lightSpecular));

	mcheck(g_diffuse) glUniform1i(g_diffuse[current_program], diff_slot);
	mcheck(g_normal) glUniform1i(g_normal[current_program], norm_slot);
}

GLuint Shader::ShaderGrass::GetProgram(void)
{
	current_program = 0;
	return program[0];
}

void Shader::ShaderGrass::BindTextureToUnit(void)
{
	correct_programm

	mcheck(g_diffuse) glUniform1i(g_diffuse[current_program], diff_slot);
	mcheck(g_normal) glUniform1i(g_normal[current_program], norm_slot);
}

void Shader::ShaderGrass::SetStarIntensity(float star_intensity)
{
	correct_programm
	mcheck(g_StarIntensity) glUniform1f(g_StarIntensity[current_program], star_intensity);
}

void Shader::ShaderGrass::SetWorld(glm::mat4 matrix)
{
	correct_programm
	mcheck(gWorld) glUniformMatrix4fv(gWorld[current_program], 1, GL_FALSE, glm::value_ptr(matrix));
}

void Shader::ShaderGrass::SetProjView(glm::mat4 matrix)
{
	correct_programm
	mcheck(gProjView) glUniformMatrix4fv(gProjView[current_program], 1, GL_FALSE, glm::value_ptr(matrix));
}

void Shader::ShaderGrass::SetCamPos(glm::vec3 vector)
{
	correct_programm
	mcheck(g_cam_pos) glUniform3fv(g_cam_pos[current_program], 1, glm::value_ptr(vector));
}

void Shader::ShaderGrass::SetMaterialDiffuse(glm::vec4 color)
{
	correct_programm
	mcheck(m_diffuse) glUniform4fv(m_diffuse[current_program], 1, glm::value_ptr(color));
}

void Shader::ShaderGrass::SetMaterialSpecular(glm::vec4 color)
{
	correct_programm
	mcheck(m_specular) glUniform4fv(m_specular[current_program], 1, glm::value_ptr(color));
}

void Shader::ShaderGrass::SetMaterialEmissive(glm::vec4 color)
{
	correct_programm
	mcheck(m_emissive) glUniform4fv(m_emissive[current_program], 1, glm::value_ptr(color));
}

void Shader::ShaderGrass::SetTextureDiffuse(void)
{
	correct_programm
	glActiveTexture(GL_TEXTURE0 + diff_slot);
}

void Shader::ShaderGrass::SetTextureNormal(void)
{
	correct_programm
	glActiveTexture(GL_TEXTURE0 + norm_slot);
}

void Shader::ShaderGrass::SetLightDir(glm::vec3 vector)
{
	correct_programm
	mcheck(g_lightDir) glUniform3fv(g_lightDir[current_program], 1, glm::value_ptr(vector));
}

void Shader::ShaderGrass::SetLightDiffuse(glm::vec4 color)
{
	correct_programm
	mcheck(g_lightDiffuse) glUniform4fv(g_lightDiffuse[current_program], 1, glm::value_ptr(color));
}

void Shader::ShaderGrass::SetLightSpecular(glm::vec4 color)
{
	correct_programm
	mcheck(g_lightSpecular) glUniform4fv(g_lightSpecular[current_program], 1, glm::value_ptr(color));
}