#include "ShaderSSR.h"


Shader::ShaderSSR::ShaderSSR(void) : ShaderBase(1, 2)
{
	/*
	IDR_SHADER48            SHADER                  "shader\\SSR\\ssr.fragment.glsl"

	IDR_SHADER49            SHADER                  "shader\\SSR\\ssr.vertex.glsl"
	*/

	Shader_Resource shaders[2];
	shaders[0].resource_ID = IDR_SHADER49;
	shaders[1].resource_ID = IDR_SHADER48;

	shaders[0].type_shader = GL_VERTEX_SHADER;
	shaders[1].type_shader = GL_FRAGMENT_SHADER;

	shaders[0].define = 0;				// vertex shader
	shaders[0].define_size = 0;

	shaders[1].define = 0;				// fragment shader
	shaders[1].define_size = 0;

	CompileShader(shaders);

	InitProgram(this->shaders[0], this->shaders[1], program[0]);

	MInitShaderVariable();

	gSourceDepth_slot = 0;
	gSourceImage_slot = gSourceDepth_slot + 1;

	gDepth_slot = gSourceImage_slot + 1;
	gNormal_slot = gDepth_slot + 1;
	gDiffuse_slot = gNormal_slot + 1;
	gSpecular_slot = gDiffuse_slot + 1;

}

Shader::ShaderSSR::~ShaderSSR() { }

void Shader::ShaderSSR::InitVariable(GLuint program)
{
	correct_programm

	gCamPos[current_program] = glGetUniformLocation(program, to_shader_name(gCamPos));
	gCamDir[current_program] = glGetUniformLocation(program, to_shader_name(gCamDir));

	gSourceDepth[current_program] = glGetUniformLocation(program, to_shader_name(gSourceDepth));
	gSourceImage[current_program] = glGetUniformLocation(program, to_shader_name(gSourceImage));

	gProjView_Source[current_program] = glGetUniformLocation(program, to_shader_name(gProjView_Source));
	gProjViewInv_Source[current_program] = glGetUniformLocation(program, to_shader_name(gProjViewInv_Source));

	gDepth[current_program] = glGetUniformLocation(program, to_shader_name(gDepth));
	gNormal[current_program] = glGetUniformLocation(program, to_shader_name(gNormal));
	gDiffuse[current_program] = glGetUniformLocation(program, to_shader_name(gDiffuse));
	gSpecular[current_program] = glGetUniformLocation(program, to_shader_name(gSpecular));

	gProjView[current_program] = glGetUniformLocation(program, to_shader_name(gProjView));
	gProjViewInv[current_program] = glGetUniformLocation(program, to_shader_name(gProjViewInv));
}

GLuint Shader::ShaderSSR::GetProgram(void)
{
	current_program = 0;
	return program[0];
}

void Shader::ShaderSSR::SetCamPos(glm::vec3 mCamPos)
{
	correct_programm
		mcheck(gCamPos) glUniform3fv(gCamPos[current_program], 1, glm::value_ptr(mCamPos));
}

void Shader::ShaderSSR::SetCamDir(glm::vec3 mCamDir)
{
	correct_programm
		mcheck(gCamDir) glUniform3fv(gCamDir[current_program], 1, glm::value_ptr(mCamDir));
}

void Shader::ShaderSSR::SetSourceDepth(void)
{
	correct_programm
		glActiveTexture(GL_TEXTURE0 + gSourceDepth_slot);
}

void Shader::ShaderSSR::SetSourceImage(void)
{
	correct_programm
		glActiveTexture(GL_TEXTURE0 + gSourceImage_slot);
}

void Shader::ShaderSSR::SetProjViewSource(glm::mat4 mProjView) 
{
	correct_programm
		mcheck(gProjView_Source) glUniformMatrix4fv(gProjView_Source[current_program], 1, GL_FALSE, glm::value_ptr(mProjView));
}

void Shader::ShaderSSR::SetProjViewInvSource(glm::mat4 mProjViewInv) 
{
	correct_programm
		mcheck(gProjViewInv_Source) glUniformMatrix4fv(gProjViewInv_Source[current_program], 1, GL_FALSE, glm::value_ptr(mProjViewInv));
}

void Shader::ShaderSSR::SetDepthLayer(void) 
{
	correct_programm
		glActiveTexture(GL_TEXTURE0 + gDepth_slot);
}

void Shader::ShaderSSR::SetNormalLayer(void) 
{
	correct_programm
		glActiveTexture(GL_TEXTURE0 + gNormal_slot);
}

void Shader::ShaderSSR::SetDiffuseLayer(void) 
{
	correct_programm
		glActiveTexture(GL_TEXTURE0 + gDiffuse_slot);
}

void Shader::ShaderSSR::SetSpecularLayer(void) 
{
	correct_programm
		glActiveTexture(GL_TEXTURE0 + gSpecular_slot);
}

void Shader::ShaderSSR::SetProjView(glm::mat4 mProjView) 
{
	correct_programm
		mcheck(gProjView) glUniformMatrix4fv(gProjView[current_program], 1, GL_FALSE, glm::value_ptr(mProjView));
}

void Shader::ShaderSSR::SetProjViewInv(glm::mat4 mProjViewInv)
{
	correct_programm
		mcheck(gProjViewInv) glUniformMatrix4fv(gProjViewInv[current_program], 1, GL_FALSE, glm::value_ptr(mProjViewInv));
}

GLuint Shader::ShaderSSR::GetTexCoordAttribLocation(void)
{
	correct_programm
	return glGetAttribLocation(program[current_program], "v_tex");
}

void Shader::ShaderSSR::BindTextureToUnit(void) 
{
	correct_programm
		
	mcheck(gSourceDepth) glUniform1i(gSourceDepth[current_program], gSourceDepth_slot);
	mcheck(gSourceImage) glUniform1i(gSourceImage[current_program], gSourceImage_slot);

	mcheck(gDepth) glUniform1i(gDepth[current_program], gDepth_slot);
	mcheck(gNormal) glUniform1i(gNormal[current_program], gNormal_slot);
	mcheck(gDiffuse) glUniform1i(gDiffuse[current_program], gDiffuse_slot);
	mcheck(gSpecular) glUniform1i(gSpecular[current_program], gSpecular_slot);
}
