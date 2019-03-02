#include "ShaderDecale.h"


Shader::ShaderDecale::ShaderDecale(void) : ShaderBase(1, 1)
{
	/*
		IDR_SHADER50            SHADER                  "shader\\Decals\\compute.glsl"
	*/
	Shader_Resource shaders[1];
	shaders[0].resource_ID = IDR_SHADER50;
	shaders[0].type_shader = GL_COMPUTE_SHADER;

	shaders[0].define = 0;
	shaders[0].define_size = 0;

	CompileShader(shaders);
	InitProgram(this->shaders[0], program[0]);

	MInitShaderVariable();

	gDepth_slot = 0;
	gDecaleTexture_slot = 1;

	gDiffuse_slot = 0;
}

Shader::ShaderDecale::~ShaderDecale()
{
}

void Shader::ShaderDecale::InitVariable(GLuint program)
{
	correct_programm

	gDepth[current_program] = glGetUniformLocation(program, to_shader_name(gDepth));
	gDecaleTextures[current_program] = glGetUniformLocation(program, to_shader_name(gDecaleTextures));

	gDecaleMatrix[current_program] = glGetUniformLocation(program, to_shader_name(gDecaleMatrix));
	gDecaleTextureID[current_program] = glGetUniformLocation(program, to_shader_name(gDecaleTextureID));
	mRealDecaleCount[current_program] = glGetUniformLocation(program, to_shader_name(mRealDecaleCount));
	gProjViewInv[current_program] = glGetUniformLocation(program, to_shader_name(gProjViewInv));

	gDiffuse[current_program] = glGetUniformLocation(program, to_shader_name(gDiffuse));
}

GLuint Shader::ShaderDecale::GetProgram(void)
{
	current_program = 0;
	return program[0];
}

void Shader::ShaderDecale::SetDecaleMatrixes(const glm::mat4 * mMatrix, uint32_t mDecaleCount)
{
	correct_programm
		glUniformMatrix4fv(gDecaleMatrix[current_program], mDecaleCount, GL_FALSE, reinterpret_cast<const GLfloat *>(mMatrix));
}

void Shader::ShaderDecale::SetTextureID(const int32_t * mTextureIds, uint32_t mDecaleCount)
{
	correct_programm
		glUniform1iv(gDecaleTextureID[current_program], mDecaleCount, mTextureIds);
}

void Shader::ShaderDecale::SetDepthTexture(void)
{
	correct_programm
		glActiveTexture(GL_TEXTURE0 + gDepth_slot);
}

void Shader::ShaderDecale::SetDecaleTextures(void)
{
	correct_programm
		glActiveTexture(GL_TEXTURE0 + gDecaleTexture_slot);
}

void Shader::ShaderDecale::SetRealDecaleCount(int32_t mDecaleCount)
{
	correct_programm
		glUniform1i(mRealDecaleCount[current_program], mDecaleCount);
}

void Shader::ShaderDecale::SetProjViewInv(glm::mat4 mProjViewInv)
{
	correct_programm
		glUniformMatrix4fv(gProjViewInv[current_program], 1, GL_FALSE, glm::value_ptr(mProjViewInv));
}

GLuint Shader::ShaderDecale::GetDiffuseSlot(void)
{
	correct_programm

	return gDiffuse_slot;
}

void Shader::ShaderDecale::BindTextureToUnit(void)
{
	correct_programm

	mcheck(gDiffuse) glUniform1i(gDiffuse[current_program], gDiffuse_slot);

	mcheck(gDepth) glUniform1i(gDepth[current_program], gDepth_slot);
	mcheck(gDecaleTextures) glUniform1i(gDecaleTextures[current_program], gDecaleTexture_slot);
}
