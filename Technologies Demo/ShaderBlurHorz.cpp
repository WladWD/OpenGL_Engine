#include "ShaderBlurHorz.h"
#include "resource.h"


Shader::BlurHorz::BlurHorz(void) : ShaderBase(1, 1) {
	Shader_Resource shaders[1];
	shaders[0].resource_ID = IDR_SHADER52;
	shaders[0].type_shader = GL_COMPUTE_SHADER;

	shaders[0].define = 0;
	shaders[0].define_size = 0;

	CompileShader(shaders);
	InitProgram(this->shaders[0], program[0]);

	MInitShaderVariable();

	mSourceSlot = 0;
	mAlbedoSlot = 1;
}

Shader::BlurHorz::~BlurHorz()
{
}

void Shader::BlurHorz::InitVariable(GLuint program)
{
	correct_programm

		mSource[current_program] = glGetUniformLocation(program, to_shader_name(mSource));
	mAlbedo[current_program] = glGetUniformLocation(program, to_shader_name(mAlbedo));

	mBlurRadius[current_program] = glGetUniformLocation(program, to_shader_name(mBlurRadius));
	mBlurWeight[current_program] = glGetUniformLocation(program, to_shader_name(mBlurWeight));
}

GLuint Shader::BlurHorz::GetProgram(void)
{
	current_program = 0;
	return program[0];
}

void Shader::BlurHorz::setBlurRadius(int32_t radius)
{
	correct_programm
		glUniform1i(mBlurRadius[current_program], radius);
}

void Shader::BlurHorz::setBlurWeight(int32_t radius, const float *weight)
{
	correct_programm
		glUniform1fv(mBlurWeight[current_program], radius + 1, weight);
}

void Shader::BlurHorz::setSourceTexture(void)
{
	glActiveTexture(GL_TEXTURE0 + mSourceSlot);
}

GLuint Shader::BlurHorz::getAlbedoTextureSlot(void)
{
	return mAlbedoSlot;
}

void Shader::BlurHorz::bindTextureToUnit(void)
{
	correct_programm

		mcheck(mSource) glUniform1i(mSource[current_program], mSourceSlot);
	mcheck(mAlbedo) glUniform1i(mAlbedo[current_program], mAlbedoSlot);
}
