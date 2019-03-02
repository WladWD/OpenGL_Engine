#include "ShaderBlurVert.h"
#include "resource.h"


Shader::BlurVert::BlurVert(void) : ShaderBase(1, 1) {

	/*IDR_SHADER51            SHADER                  "shader\\Blur\\blur_X.glsl"

IDR_SHADER52            SHADER                  "shader\\Blur\\blur_Y.glsl"*/

	Shader_Resource shaders[1];
	shaders[0].resource_ID = IDR_SHADER51;
	shaders[0].type_shader = GL_COMPUTE_SHADER;

	shaders[0].define = 0;
	shaders[0].define_size = 0;

	CompileShader(shaders);
	InitProgram(this->shaders[0], program[0]);

	MInitShaderVariable();

	mSourceSlot = 0;
	mAlbedoSlot = 1;
}

Shader::BlurVert::~BlurVert()
{
}

void Shader::BlurVert::InitVariable(GLuint program)
{
	correct_programm

	mSource[current_program] = glGetUniformLocation(program, to_shader_name(mSource));
	mAlbedo[current_program] = glGetUniformLocation(program, to_shader_name(mAlbedo));

	mBlurRadius[current_program] = glGetUniformLocation(program, to_shader_name(mBlurRadius));
	mBlurWeight[current_program] = glGetUniformLocation(program, to_shader_name(mBlurWeight));
}

GLuint Shader::BlurVert::GetProgram(void)
{
	current_program = 0;
	return program[0];
}

void Shader::BlurVert::setBlurRadius(int32_t radius)
{
	correct_programm
	glUniform1i(mBlurRadius[current_program], radius);
}

void Shader::BlurVert::setBlurWeight(int32_t radius, const float *weight)
{
	correct_programm
		glUniform1fv(mBlurWeight[current_program], radius + 1, weight);
}

void Shader::BlurVert::setSourceTexture(void)
{
	glActiveTexture(GL_TEXTURE0 + mSourceSlot);
}

GLuint Shader::BlurVert::getAlbedoTextureSlot(void)
{
	return mAlbedoSlot;
}

void Shader::BlurVert::bindTextureToUnit(void)
{
	correct_programm

	mcheck(mSource) glUniform1i(mSource[current_program], mSourceSlot);
	mcheck(mAlbedo) glUniform1i(mAlbedo[current_program], mAlbedoSlot);
}
