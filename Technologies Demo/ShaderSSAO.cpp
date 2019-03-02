#include "ShaderSSAO.h"

Shader::ShaderSSAO::ShaderSSAO() : ShaderBase(1, 2) {
	//IDR_SHADER53            SHADER                  "shader\\SSAO\\fragment.glsl"
	//	IDR_SHADER54            SHADER                  "shader\\SSAO\\vertex.glsl"

	Shader_Resource shaders[2];
	shaders[0].resource_ID = IDR_SHADER54;
	shaders[1].resource_ID = IDR_SHADER53;

	shaders[0].type_shader = GL_VERTEX_SHADER;
	shaders[1].type_shader = GL_FRAGMENT_SHADER;

	shaders[0].define = 0;				// vertex shader
	shaders[0].define_size = 0;

	shaders[1].define = 0;				// fragment shader
	shaders[1].define_size = 0;

	CompileShader(shaders);

	InitProgram(this->shaders[0], this->shaders[1], program[0]);

	gDepth_slot = 0;
	gNormal_slot = 1;//gDepth_slot + 1;
	gDiffuse_slot = 2;// gNormal_slot + 1;
	gSpecular_slot = 3;// gDiffuse_slot + 1;
	gNoise_slot = 4;// gSpecular_slot + 1;

	MInitShaderVariable();
}

Shader::ShaderSSAO::~ShaderSSAO() { }

GLuint Shader::ShaderSSAO::getProgram() {
	current_program = 0;
	return program[current_program];
}

void Shader::ShaderSSAO::InitVariable(GLuint program) {
	correct_programm

	mProj[current_program] = glGetUniformLocation(program, to_shader_name(mProj));
	mInvProj[current_program] = glGetUniformLocation(program, to_shader_name(mInvProj));

	mView[current_program] = glGetUniformLocation(program, to_shader_name(mView));
	mInvView[current_program] = glGetUniformLocation(program, to_shader_name(mInvView));

	mProjView[current_program] = glGetUniformLocation(program, to_shader_name(mProjView));
	mInvProjView[current_program] = glGetUniformLocation(program, to_shader_name(mInvProjView));

	mWorld[current_program] = glGetUniformLocation(program, to_shader_name(mWorld));
	mInvWorld[current_program] = glGetUniformLocation(program, to_shader_name(mInvWorld));

	mProjViewWorld[current_program] = glGetUniformLocation(program, to_shader_name(mProjViewWorld));
	mInvProjViewWorld[current_program] = glGetUniformLocation(program, to_shader_name(mInvProjViewWorld));

	noiseScale[current_program] = glGetUniformLocation(program, to_shader_name(noiseScale));
	randomVector[current_program] = glGetUniformLocation(program, to_shader_name(randomVector));

	gDepth[current_program] = glGetUniformLocation(program, to_shader_name(gDepth));
	gNormal[current_program] = glGetUniformLocation(program, to_shader_name(gNormal));
	gDiffuse[current_program] = glGetUniformLocation(program, to_shader_name(gDiffuse));
	gSpecular[current_program] = glGetUniformLocation(program, to_shader_name(gSpecular));
	gNoise[current_program] = glGetUniformLocation(program, to_shader_name(gNoise));

	bindTextureToSlot();
}

void Shader::ShaderSSAO::bindTextureToSlot() {
	correct_programm

	mcheck(gDepth) glUniform1i(gDepth[current_program], gDepth_slot);
	mcheck(gNormal) glUniform1i(gNormal[current_program], gNormal_slot);
	mcheck(gDiffuse) glUniform1i(gDiffuse[current_program], gDiffuse_slot);
	mcheck(gSpecular) glUniform1i(gSpecular[current_program], gSpecular_slot);
	mcheck(gNoise) glUniform1i(gNoise[current_program], gNoise_slot);
}

void Shader::ShaderSSAO::setProj(const glm::mat4& mProj) {
	correct_programm
		mcheck(this->mProj) glUniformMatrix4fv(this->mProj[current_program], 1, GL_FALSE, glm::value_ptr(mProj));
}

void Shader::ShaderSSAO::setInvProj(const glm::mat4& mInvProj) {
	correct_programm
		mcheck(this->mInvProj) glUniformMatrix4fv(this->mInvProj[current_program], 1, GL_FALSE, glm::value_ptr(mInvProj));
}

void Shader::ShaderSSAO::setView(const glm::mat4 & mView) {
	correct_programm
		mcheck(this->mView) glUniformMatrix4fv(this->mView[current_program], 1, GL_FALSE, glm::value_ptr(mView));
}

void Shader::ShaderSSAO::setInvView(const glm::mat4 & mInvView) {
	correct_programm
		mcheck(this->mInvView) glUniformMatrix4fv(this->mInvView[current_program], 1, GL_FALSE, glm::value_ptr(mInvView));
}

void Shader::ShaderSSAO::setProjView(const glm::mat4 & mProjView) {
	correct_programm
		mcheck(this->mProjView) glUniformMatrix4fv(this->mProjView[current_program], 1, GL_FALSE, glm::value_ptr(mProjView));
}

void Shader::ShaderSSAO::setInvProjView(const glm::mat4 & mInvProjView) {
	correct_programm
		mcheck(this->mInvProjView) glUniformMatrix4fv(this->mInvProjView[current_program], 1, GL_FALSE, glm::value_ptr(mInvProjView));
}

void Shader::ShaderSSAO::setWorld(const glm::mat4 & mWorld) {
	correct_programm
		mcheck(this->mWorld) glUniformMatrix4fv(this->mWorld[current_program], 1, GL_FALSE, glm::value_ptr(mWorld));
}

void Shader::ShaderSSAO::setInvWorld(const glm::mat4 & mInvWorld) {
	correct_programm
		mcheck(this->mInvWorld) glUniformMatrix4fv(this->mInvWorld[current_program], 1, GL_FALSE, glm::value_ptr(mInvWorld));
}

void Shader::ShaderSSAO::setProjViewWorld(const glm::mat4 & mProjViewWorld) {
	correct_programm
		mcheck(this->mProjViewWorld) glUniformMatrix4fv(this->mProjViewWorld[current_program], 1, GL_FALSE, glm::value_ptr(mProjViewWorld));
}

void Shader::ShaderSSAO::setInvProjViewWorld(const glm::mat4 & mInvProjViewWorld) {
	correct_programm
		mcheck(this->mInvProjViewWorld) glUniformMatrix4fv(this->mInvProjViewWorld[current_program], 1, GL_FALSE, glm::value_ptr(mInvProjViewWorld));
}

void Shader::ShaderSSAO::bindDepthTexture(GLuint texture) {
	glActiveTexture(GL_TEXTURE0 + gDepth_slot);
	glBindTexture(GL_TEXTURE_2D, texture);
}

void Shader::ShaderSSAO::bindNormalTexture(GLuint texture) {
	glActiveTexture(GL_TEXTURE0 + gNormal_slot);
	glBindTexture(GL_TEXTURE_2D, texture);
}

void Shader::ShaderSSAO::bindDiffuseTexture(GLuint texture) {
	glActiveTexture(GL_TEXTURE0 + gDiffuse_slot);
	glBindTexture(GL_TEXTURE_2D, texture);
}

void Shader::ShaderSSAO::bindSpecularTexture(GLuint texture) {
	glActiveTexture(GL_TEXTURE0 + gSpecular_slot);
	glBindTexture(GL_TEXTURE_2D, texture);
}

void Shader::ShaderSSAO::bindNoiseTexture(GLuint texture) {
	glActiveTexture(GL_TEXTURE0 + gNoise_slot);
	glBindTexture(GL_TEXTURE_2D, texture);
}

void Shader::ShaderSSAO::setNoiseScale(const glm::vec2 &texDim) {
	correct_programm
		mcheck(this->noiseScale) glUniform2fv(this->noiseScale[current_program],1, glm::value_ptr(texDim));
}

void Shader::ShaderSSAO::setRandomVectors(const std::vector<glm::vec4>& rVectors) {
	correct_programm
		mcheck(this->randomVector) glUniform4fv(this->randomVector[current_program], rVectors.size(), reinterpret_cast<const GLfloat *>(rVectors.data()));
}
