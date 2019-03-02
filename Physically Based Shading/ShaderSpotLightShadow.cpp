#include "ShaderSpotLightShadow.h"


Shader::ShaderSpotLightShadow::ShaderSpotLightShadow(void) : ShaderBase(2, 4)
{
/*
IDR_SHADER36            SHADER                  "shader\\Shadow\\Triangle\\VNT\\VSTriangleNTShadow.glsl"

IDR_SHADER37            SHADER                  "shader\\Shadow\\Triangle\\VNT\\PSTriangleNTShadow.glsl"

IDR_SHADER40            SHADER                  "shader\\Shadow\\Triangle\\VNT\\PSTriangleNTShadowClip.glsl"

IDR_SHADER41            SHADER                  "shader\\Shadow\\Triangle\\VNT\\GSTriangleNTShadow.glsl"
*/

	Shader_Resource shaders[4];
	shaders[0].resource_ID = IDR_SHADER36;
	shaders[1].resource_ID = IDR_SHADER37;
	shaders[2].resource_ID = IDR_SHADER40;
	shaders[3].resource_ID = IDR_SHADER41;


	shaders[0].type_shader = GL_VERTEX_SHADER;
	shaders[1].type_shader = GL_FRAGMENT_SHADER;
	shaders[2].type_shader = GL_FRAGMENT_SHADER;
	shaders[3].type_shader = GL_GEOMETRY_SHADER;

	shaders[0].define = 0;//vertex shader
	shaders[0].define_size = 0;

	shaders[1].define = 0;//fragment shader
	shaders[1].define_size = 0;

	shaders[2].define = 0;//fragment shader
	shaders[2].define_size = 0;

	shaders[3].define = 0;//geometry shader
	shaders[3].define_size = 0;


	CompileShader(shaders);

	InitProgram(this->shaders[0], this->shaders[3], this->shaders[1], program[0]);
	InitProgram(this->shaders[0], this->shaders[3], this->shaders[2], program[1]);// clip diffuse

	MInitShaderVariable();

	Diffuse_slot = 0;
}

Shader::ShaderSpotLightShadow::~ShaderSpotLightShadow() { }

void Shader::ShaderSpotLightShadow::InitVariable(GLuint program)
{
	correct_programm

	mProjView[current_program] = glGetUniformLocation(program, to_shader_name(mProjView));
	uShadowMapCount[current_program] = glGetUniformLocation(program, to_shader_name(uShadowMapCount));
	gDiffTexture[current_program] = glGetUniformLocation(program, to_shader_name(gDiffTexture));
}

GLuint Shader::ShaderSpotLightShadow::GetProgram(void)
{
	current_program = 0;
	return program[0];
}

GLuint Shader::ShaderSpotLightShadow::GetProgramClip(void) 
{
	current_program = 1;
	return program[1];
}

void Shader::ShaderSpotLightShadow::SetProjViewMatrix(const glm::mat4 matrix[], uint32_t count)
{
	correct_programm
	mcheck(mProjView) glUniformMatrix4fv(mProjView[current_program], count, GL_FALSE, glm::value_ptr(matrix[0]));
}

void Shader::ShaderSpotLightShadow::SetShadowCount(uint32_t count) 
{
	correct_programm
	mcheck(uShadowMapCount) glUniform1ui(uShadowMapCount[current_program], count);
}

void Shader::ShaderSpotLightShadow::SetDiffuseTex(void) 
{
	correct_programm
	glActiveTexture(GL_TEXTURE0 + Diffuse_slot);
}

void Shader::ShaderSpotLightShadow::BindTextureToUnit(void) 
{
	correct_programm
	mcheck(gDiffTexture) glUniform1i(gDiffTexture[current_program], Diffuse_slot);
}