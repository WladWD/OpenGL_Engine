#include "ShaderDeferredVNT.h"


Shader::ShaderDeferredVNT::ShaderDeferredVNT(void) : ShaderBase(7, 8)
{
	/*
IDR_SHADER25            SHADER                  "shader\\Deferred Rendering\\Triangle\\VNT\\TriangleNT.ps"

IDR_SHADER26            SHADER                  "shader\\Deferred Rendering\\Triangle\\VNT\\TriangleNT.vs"

IDR_SHADER27            SHADER                  "shader\\Deferred Rendering\\Triangle\\VNT\\TriangleNT_Diffuse.ps"

IDR_SHADER28            SHADER                  "shader\\Deferred Rendering\\Triangle\\VNT\\TriangleNT_Diff_Spec_Reflaction.ps"

IDR_SHADER29            SHADER                  "shader\\Deferred Rendering\\Triangle\\VNT\\TriangleNT_Diffuse_Spec.ps"
	*/

	Shader_Resource shaders[8];
	shaders[0].resource_ID = IDR_SHADER26;
	shaders[1].resource_ID = IDR_SHADER25;
	shaders[2].resource_ID = IDR_SHADER27;
	shaders[3].resource_ID = IDR_SHADER27;
	shaders[4].resource_ID = IDR_SHADER29;
	shaders[5].resource_ID = IDR_SHADER29;
	shaders[6].resource_ID = IDR_SHADER28;
	shaders[7].resource_ID = IDR_SHADER28;

	shaders[0].type_shader = GL_VERTEX_SHADER;
	shaders[1].type_shader = GL_FRAGMENT_SHADER;
	shaders[2].type_shader = GL_FRAGMENT_SHADER;
	shaders[3].type_shader = GL_FRAGMENT_SHADER;
	shaders[4].type_shader = GL_FRAGMENT_SHADER;
	shaders[5].type_shader = GL_FRAGMENT_SHADER;
	shaders[6].type_shader = GL_FRAGMENT_SHADER;
	shaders[7].type_shader = GL_FRAGMENT_SHADER;

	shaders[0].define = 0;//vertex shader
	shaders[0].define_size = 0;

	shaders[1].define = "#version 440 core\n";//pixel shader no tex
	shaders[1].define_size = 18;

	shaders[2].define = "#version 440 core\n#define NOT_USE_CLIP_ALPHA\n";//pixel shader diffuse no cliping(early depth test)
	shaders[2].define_size = 45;

	shaders[3].define = "#version 440 core\n";//pixel shader diffuse cliping(no early depth test)
	shaders[3].define_size = 18;

	shaders[4].define = "#version 440 core\n#define NOT_USE_CLIP_ALPHA\n";//pixel shader diffuse specular no cliping(early depth test)
	shaders[4].define_size = 45;

	shaders[5].define = "#version 440 core\n";//pixel shader diffuse specular cliping(no early depth test)
	shaders[5].define_size = 18;

	shaders[6].define = "#version 440 core\n#define NOT_USE_CLIP_ALPHA\n";//pixel shader diffuse specular reflaction no cliping(early depth test)
	shaders[6].define_size = 45;

	shaders[7].define = "#version 440 core\n";//pixel shader diffuse specular cliping(no early depth test)
	shaders[7].define_size = 18;

	CompileShader(shaders);

	InitProgram(this->shaders[0], this->shaders[1], program[0]);
	InitProgram(this->shaders[0], this->shaders[2], program[1]);
	InitProgram(this->shaders[0], this->shaders[3], program[2]);
	InitProgram(this->shaders[0], this->shaders[4], program[3]);
	InitProgram(this->shaders[0], this->shaders[5], program[4]);
	InitProgram(this->shaders[0], this->shaders[6], program[5]);
	InitProgram(this->shaders[0], this->shaders[7], program[6]);

	MInitShaderVariable();

	Diffuse_slot = 0;
	Specular_slot = Diffuse_slot + 1;
	Reflaction_slot = Specular_slot + 1;
}

Shader::ShaderDeferredVNT::~ShaderDeferredVNT() { }

void Shader::ShaderDeferredVNT::InitVariable(GLuint program)
{
	correct_programm
	
	g_proj_view[current_program] = glGetUniformLocation(program, to_shader_name(g_proj_view));
	g_cam_pos[current_program] = glGetUniformLocation(program, to_shader_name(g_cam_pos));
	m_diffuse[current_program] = glGetUniformLocation(program, to_shader_name(m_diffuse));
	m_specular[current_program] = glGetUniformLocation(program, to_shader_name(m_specular));
	m_emissive[current_program] = glGetUniformLocation(program, to_shader_name(m_emissive));
	g_diffuse[current_program] = glGetUniformLocation(program, to_shader_name(g_diffuse));
	g_specular[current_program] = glGetUniformLocation(program, to_shader_name(g_specular));
	g_reflaction[current_program] = glGetUniformLocation(program, to_shader_name(g_reflaction));
	m_PBR[current_program] = glGetUniformLocation(program, to_shader_name(m_PBR));
}

GLuint Shader::ShaderDeferredVNT::GetProgramNoTexture(void) 
{
	current_program = 0;
	return program[0];
}

GLuint Shader::ShaderDeferredVNT::GetProgramDiffuse(void)
{
	current_program = 1;
	return program[1];
}

GLuint Shader::ShaderDeferredVNT::GetProgramDiffuseClip(void) 
{
	current_program = 2;
	return program[2];
}

GLuint Shader::ShaderDeferredVNT::GetProgramDiffSpec(void) 
{
	current_program = 3;
	return program[3];
}

GLuint Shader::ShaderDeferredVNT::GetProgramDiffSpecClip(void) 
{
	current_program = 4;
	return program[4];
}

GLuint Shader::ShaderDeferredVNT::GetProgramDiffSpecRefl(void) 
{
	current_program = 5;
	return program[5];
}

GLuint Shader::ShaderDeferredVNT::GetProgramDiffSpecReflClip(void) 
{
	current_program = 6;
	return program[6];
}

void Shader::ShaderDeferredVNT::SetProjViewWorld(glm::mat4 matrix) 
{
	correct_programm
	mcheck(g_proj_view)glUniformMatrix4fv(g_proj_view[current_program], 1, GL_FALSE, glm::value_ptr(matrix));
}

void Shader::ShaderDeferredVNT::SetPBSData(glm::vec4 data)
{
	correct_programm
		mcheck(m_PBR)glUniform4fv(m_PBR[current_program], 1, glm::value_ptr(data));
}

void Shader::ShaderDeferredVNT::SetCamPos(glm::vec3 pos) 
{
	correct_programm
	mcheck(g_cam_pos)glUniform3fv(g_cam_pos[current_program], 1, glm::value_ptr(pos));
}

void Shader::ShaderDeferredVNT::SetDiffuseColor(glm::vec4 color) 
{
	correct_programm
	mcheck(m_diffuse)glUniform4fv(m_diffuse[current_program], 1, glm::value_ptr(color));
}

void Shader::ShaderDeferredVNT::SetSpecularColor(glm::vec4 color) 
{
	correct_programm
	mcheck(m_specular)glUniform4fv(m_specular[current_program], 1, glm::value_ptr(color));
}

void Shader::ShaderDeferredVNT::SetEmissiveColor(glm::vec4 color) 
{
	correct_programm
	mcheck(m_emissive)glUniform4fv(m_emissive[current_program], 1, glm::value_ptr(color));
}

void Shader::ShaderDeferredVNT::SetDiffuseTex(void)
{
	correct_programm
	glActiveTexture(GL_TEXTURE0 + Diffuse_slot);
}

void Shader::ShaderDeferredVNT::SetSpecularTex(void) 
{
	correct_programm
	glActiveTexture(GL_TEXTURE0 + Specular_slot);
}

void Shader::ShaderDeferredVNT::SetReflactionTex(void) 
{
	correct_programm
	glActiveTexture(GL_TEXTURE0 + Reflaction_slot);
}

void Shader::ShaderDeferredVNT::BindTextureToUnit(void)
{
	correct_programm

	mcheck(g_diffuse) glUniform1i(g_diffuse[current_program], Diffuse_slot);
	mcheck(g_specular) glUniform1i(g_specular[current_program], Specular_slot);
	mcheck(g_reflaction) glUniform1i(g_reflaction[current_program], Reflaction_slot);
}
