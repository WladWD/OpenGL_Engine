#include "ShaderDeferredVNTT.h"


Shader::ShaderDeferredVNTT::ShaderDeferredVNTT(void) : ShaderBase(6, 7)
{
	/*
IDR_SHADER30            SHADER                  "shader\\Deferred Rendering\\Triangle\\VNTT\\TriangleNT_Tang.vs"

IDR_SHADER31            SHADER                  "shader\\Deferred Rendering\\Triangle\\VNTT\\TriangleNT_Tang_Diff_Reflaction.ps"

IDR_SHADER32            SHADER                  "shader\\Deferred Rendering\\Triangle\\VNTT\\TriangleNT_Tang_Diff_Spec.ps"

IDR_SHADER33            SHADER                  "shader\\Deferred Rendering\\Triangle\\VNTT\\TriangleNT_Tang_Diffuse.ps"
	*/

	Shader_Resource shaders[7];
	shaders[0].resource_ID = IDR_SHADER30;
	shaders[1].resource_ID = IDR_SHADER33;
	shaders[2].resource_ID = IDR_SHADER33;
	shaders[3].resource_ID = IDR_SHADER32;
	shaders[4].resource_ID = IDR_SHADER32;
	shaders[5].resource_ID = IDR_SHADER31;
	shaders[6].resource_ID = IDR_SHADER31;

	shaders[0].type_shader = GL_VERTEX_SHADER;
	shaders[1].type_shader = GL_FRAGMENT_SHADER;
	shaders[2].type_shader = GL_FRAGMENT_SHADER;
	shaders[3].type_shader = GL_FRAGMENT_SHADER;
	shaders[4].type_shader = GL_FRAGMENT_SHADER;
	shaders[5].type_shader = GL_FRAGMENT_SHADER;
	shaders[6].type_shader = GL_FRAGMENT_SHADER;

	shaders[0].define = 0;//vertex shader
	shaders[0].define_size = 0;

	shaders[1].define = "#version 440 core\n#define NOT_USE_CLIP_ALPHA\n";//pixel shader diffuse no cliping(early depth test)
	shaders[1].define_size = 45;

	shaders[2].define = "#version 440 core\n";//pixel shader diffuse cliping(no early depth test)
	shaders[2].define_size = 18;

	shaders[3].define = "#version 440 core\n#define NOT_USE_CLIP_ALPHA\n";//pixel shader diffuse specular no cliping(early depth test)
	shaders[3].define_size = 45;

	shaders[4].define = "#version 440 core\n";//pixel shader diffuse specular cliping(no early depth test)
	shaders[4].define_size = 18;

	shaders[5].define = "#version 440 core\n#define NOT_USE_CLIP_ALPHA\n";//pixel shader diffuse specular reflaction no cliping(early depth test)
	shaders[5].define_size = 45;

	shaders[6].define = "#version 440 core\n";//pixel shader diffuse specular cliping(no early depth test)
	shaders[6].define_size = 18;

	CompileShader(shaders);

	InitProgram(this->shaders[0], this->shaders[1], program[0]);
	InitProgram(this->shaders[0], this->shaders[2], program[1]);
	InitProgram(this->shaders[0], this->shaders[3], program[2]);
	InitProgram(this->shaders[0], this->shaders[4], program[3]);
	InitProgram(this->shaders[0], this->shaders[5], program[4]);
	InitProgram(this->shaders[0], this->shaders[6], program[5]);

	MInitShaderVariable();

	Diffuse_slot = 0;
	Normal_slot = Diffuse_slot + 1;
	Specular_slot = Normal_slot + 1;
	Reflaction_slot = Specular_slot + 1;
}

Shader::ShaderDeferredVNTT::~ShaderDeferredVNTT() { }

void Shader::ShaderDeferredVNTT::InitVariable(GLuint program)
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
	m_PBR[current_program] = glGetUniformLocation(program, to_shader_name(m_PBR));
}

GLuint Shader::ShaderDeferredVNTT::GetProgramDiffuse(void)
{
	current_program = 0;
	return program[0];
}

GLuint Shader::ShaderDeferredVNTT::GetProgramDiffuseClip(void)
{
	current_program = 1;
	return program[1];
}

GLuint Shader::ShaderDeferredVNTT::GetProgramDiffSpec(void)
{
	current_program = 2;
	return program[2];
}

GLuint Shader::ShaderDeferredVNTT::GetProgramDiffSpecClip(void)
{
	current_program = 3;
	return program[3];
}

GLuint Shader::ShaderDeferredVNTT::GetProgramDiffSpecRefl(void)
{
	current_program = 4;
	return program[4];
}

GLuint Shader::ShaderDeferredVNTT::GetProgramDiffSpecReflClip(void)
{
	current_program = 5;
	return program[5];
}

void Shader::ShaderDeferredVNTT::SetPBSData(glm::vec4 data)
{
	correct_programm
		mcheck(m_PBR)glUniform4fv(m_PBR[current_program], 1, glm::value_ptr(data));
}

void Shader::ShaderDeferredVNTT::SetProjViewWorld(glm::mat4 matrix)
{
	correct_programm
	mcheck(g_proj_view)glUniformMatrix4fv(g_proj_view[current_program], 1, GL_FALSE, glm::value_ptr(matrix));
}

void Shader::ShaderDeferredVNTT::SetCamPos(glm::vec3 pos)
{
	correct_programm
	mcheck(g_cam_pos)glUniform3fv(g_cam_pos[current_program], 1, glm::value_ptr(pos));
}

void Shader::ShaderDeferredVNTT::SetDiffuseColor(glm::vec4 color)
{
	correct_programm
	mcheck(m_diffuse)glUniform4fv(m_diffuse[current_program], 1, glm::value_ptr(color));
}

void Shader::ShaderDeferredVNTT::SetSpecularColor(glm::vec4 color)
{
	correct_programm
	mcheck(m_specular)glUniform4fv(m_specular[current_program], 1, glm::value_ptr(color));
}

void Shader::ShaderDeferredVNTT::SetEmissiveColor(glm::vec4 color)
{
	correct_programm
	mcheck(m_emissive)glUniform4fv(m_emissive[current_program], 1, glm::value_ptr(color));
}

void Shader::ShaderDeferredVNTT::SetDiffuseTex(void)
{
	correct_programm
	glActiveTexture(GL_TEXTURE0 + Diffuse_slot);
}

void Shader::ShaderDeferredVNTT::SetSpecularTex(void)
{
	correct_programm
	glActiveTexture(GL_TEXTURE0 + Specular_slot);
}

void Shader::ShaderDeferredVNTT::SetReflactionTex(void)
{
	correct_programm
	glActiveTexture(GL_TEXTURE0 + Reflaction_slot);
}

void Shader::ShaderDeferredVNTT::SetNormalTex(void)
{
	correct_programm
	glActiveTexture(GL_TEXTURE0 + Normal_slot);
}

void Shader::ShaderDeferredVNTT::BindTextureToUnit(void)
{
	correct_programm

	mcheck(g_diffuse) glUniform1i(g_diffuse[current_program], Diffuse_slot);
	mcheck(g_normal) glUniform1i(g_normal[current_program], Normal_slot);
	mcheck(g_specular) glUniform1i(g_specular[current_program], Specular_slot);
	mcheck(g_reflaction) glUniform1i(g_reflaction[current_program], Reflaction_slot);
}
