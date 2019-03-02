#include "Shader_VNTT.h"


Shader::Shader_VNTT::Shader_VNTT() : ShaderBase(2, 3)
{
/*
IDR_SHADER5             Shader                  "shader\\Forward Rendering\\Triangle\\VNTT\\TriangleNT_Tang.vs"

IDR_SHADER6             Shader                  "shader\\Forward Rendering\\Triangle\\VNTT\\TriangleNT_Tang_Diff_Reflaction.ps"

IDR_SHADER7             Shader                  "shader\\Forward Rendering\\Triangle\\VNTT\\TriangleNT_Tang_Diffuse.ps"
*/

	Shader_Resource shaders[3];
	shaders[0].resource_ID = IDR_SHADER5;
	shaders[1].resource_ID = IDR_SHADER6;
	shaders[2].resource_ID = IDR_SHADER7;

	shaders[0].type_shader = GL_VERTEX_SHADER;
	shaders[1].type_shader = GL_FRAGMENT_SHADER;
	shaders[2].type_shader = GL_FRAGMENT_SHADER;

	shaders[0].define = 0;
	shaders[0].define_size = 0;

	shaders[1].define = 0;
	shaders[1].define_size = 0;

	shaders[2].define = 0;
	shaders[2].define_size = 0;

	CompileShader(shaders);

	InitProgram(this->shaders[0], this->shaders[1], program[0]);//diff reflect
	InitProgram(this->shaders[0], this->shaders[2], program[1]);//diff

	diff_slot = 0;
	norm_slot = 1;
	spec_slot = 2;
	reflect_slot = 3;

	MInitShaderVariable();
}

Shader::Shader_VNTT::~Shader_VNTT() { }

void Shader::Shader_VNTT::InitVariable(GLuint program)
{
	correct_programm

	g_proj_view[current_program] = glGetUniformLocation(program, to_shader_name(g_proj_view));
	g_cam_pos[current_program] = glGetUniformLocation(program, to_shader_name(g_cam_pos));

	m_diffuse[current_program] = glGetUniformLocation(program, to_shader_name(m_diffuse));
	m_specular[current_program] = glGetUniformLocation(program, to_shader_name(m_specular));
	m_emissive[current_program] = glGetUniformLocation(program, to_shader_name(m_emissive));

	g_diffuse[current_program] = glGetUniformLocation(program, to_shader_name(g_diffuse));
	g_normal[current_program] = glGetUniformLocation(program, to_shader_name(g_normal));
	g_specular[current_program] = glGetUniformLocation(program, to_shader_name(g_specular));
	g_reflaction[current_program] = glGetUniformLocation(program, to_shader_name(g_reflaction));

	g_lightDir[current_program] = glGetUniformLocation(program, to_shader_name(g_lightDir));
	g_lightDiffuse[current_program] = glGetUniformLocation(program, to_shader_name(g_lightDiffuse));
	g_lightSpecular[current_program] = glGetUniformLocation(program, to_shader_name(g_lightSpecular));

	mcheck(g_diffuse) glUniform1i(g_diffuse[current_program], diff_slot);
	mcheck(g_normal) glUniform1i(g_normal[current_program], norm_slot);
	mcheck(g_specular) glUniform1i(g_specular[current_program], spec_slot);
	mcheck(g_reflaction) glUniform1i(g_reflaction[current_program], reflect_slot);
}

GLuint Shader::Shader_VNTT::GetProgramDiff(void)
{
	current_program = 1;
	return program[1];
}

GLuint Shader::Shader_VNTT::GetProgramDiffReflect(void)
{
	current_program = 0;
	return program[0];
}

void Shader::Shader_VNTT::BindTextureToUnit(void)
{
	correct_programm

	mcheck(g_diffuse) glUniform1i(g_diffuse[current_program], diff_slot);
	mcheck(g_normal) glUniform1i(g_normal[current_program], norm_slot);
	mcheck(g_specular) glUniform1i(g_specular[current_program], spec_slot);
	mcheck(g_reflaction) glUniform1i(g_reflaction[current_program], reflect_slot);
}

void Shader::Shader_VNTT::SetProjView(glm::mat4 matrix)
{
	correct_programm
	mcheck(g_proj_view) glUniformMatrix4fv(g_proj_view[current_program], 1, GL_FALSE, glm::value_ptr(matrix));
}

void Shader::Shader_VNTT::SetCamPos(glm::vec3 vector)
{
	correct_programm
	mcheck(g_cam_pos) glUniform3fv(g_cam_pos[current_program], 1, glm::value_ptr(vector));
}

void Shader::Shader_VNTT::SetMaterialDiffuse(glm::vec4 color)
{
	correct_programm
	mcheck(m_diffuse) glUniform4fv(m_diffuse[current_program], 1, glm::value_ptr(color));
}

void Shader::Shader_VNTT::SetMaterialSpecular(glm::vec4 color)
{
	correct_programm
	mcheck(m_specular) glUniform4fv(m_specular[current_program], 1, glm::value_ptr(color));
}

void Shader::Shader_VNTT::SetMaterialEmissive(glm::vec4 color)
{
	correct_programm
	mcheck(m_emissive) glUniform4fv(m_emissive[current_program], 1, glm::value_ptr(color));
}

void Shader::Shader_VNTT::SetTextureDiffuse(void)
{
	correct_programm
	glActiveTexture(GL_TEXTURE0 + diff_slot);
}

void Shader::Shader_VNTT::SetTextureNormal(void)
{
	correct_programm
	glActiveTexture(GL_TEXTURE0 + norm_slot);
}

void Shader::Shader_VNTT::SetTextureSpecular(void)
{
	correct_programm
	glActiveTexture(GL_TEXTURE0 + spec_slot);
}

void Shader::Shader_VNTT::SetTextureReflaction(void)
{
	correct_programm
	glActiveTexture(GL_TEXTURE0 + reflect_slot);
}

void Shader::Shader_VNTT::SetLightDir(glm::vec3 vector)
{
	correct_programm
	mcheck(g_lightDir) glUniform3fv(g_lightDir[current_program], 1, glm::value_ptr(vector));
}

void Shader::Shader_VNTT::SetLightDiffuse(glm::vec4 color)
{
	correct_programm
	mcheck(g_lightDiffuse) glUniform4fv(g_lightDiffuse[current_program], 1, glm::value_ptr(color));
}

void Shader::Shader_VNTT::SetLightSpecular(glm::vec4 color)
{
	correct_programm
	mcheck(g_lightSpecular) glUniform4fv(g_lightSpecular[current_program], 1, glm::value_ptr(color));
}