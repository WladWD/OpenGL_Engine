#include "ShaderGaussianBlur.h"


Shader::ShaderGaussianBlur::ShaderGaussianBlur() : ShaderBase(1, 2) {
	//IDR_SHADER55            SHADER                  "shader\\SSAO\\GaussianBlur\\fragment.glsl"
	//IDR_SHADER56            SHADER                  "shader\\SSAO\\GaussianBlur\\vertex.glsl"

	Shader_Resource shaders[2];
	shaders[0].resource_ID = IDR_SHADER56;
	shaders[1].resource_ID = IDR_SHADER55;

	shaders[0].type_shader = GL_VERTEX_SHADER;
	shaders[1].type_shader = GL_FRAGMENT_SHADER;

	shaders[0].define = 0;				// vertex shader
	shaders[0].define_size = 0;

	shaders[1].define = 0;				// fragment shader
	shaders[1].define_size = 0;

	CompileShader(shaders);

	InitProgram(this->shaders[0], this->shaders[1], program[0]);

	gSourceTexture_slot = 0;

	MInitShaderVariable();
}

Shader::ShaderGaussianBlur::~ShaderGaussianBlur() { }

void Shader::ShaderGaussianBlur::InitVariable(GLuint program) {
	correct_programm

	mipBias[current_program] = glGetUniformLocation(program, to_shader_name(mipBias));
	texOffset[current_program] = glGetUniformLocation(program, to_shader_name(texOffset));
	gaussian[current_program] = glGetUniformLocation(program, to_shader_name(gaussian));

	gTexture[current_program] = glGetUniformLocation(program, to_shader_name(gTexture));

	bindTextureToSlot();
}

void Shader::ShaderGaussianBlur::bindTextureToSlot() {
	correct_programm

	mcheck(gTexture) glUniform1i(gTexture[current_program], gSourceTexture_slot);
}

GLuint Shader::ShaderGaussianBlur::getProgram() {
	current_program = 0;
	return program[current_program];
}

void Shader::ShaderGaussianBlur::setMipBias(float mipBias) {
	correct_programm
		mcheck(this->mipBias) glUniform1f(this->mipBias[current_program], mipBias);
}

void Shader::ShaderGaussianBlur::setTexOffset(const glm::vec2 & texOffset) {
	correct_programm
		mcheck(this->texOffset) glUniform2fv(this->texOffset[current_program], 1, glm::value_ptr(texOffset));
}

void Shader::ShaderGaussianBlur::setGaussian(const std::vector<float>& gaussian) {
	correct_programm
		mcheck(this->texOffset) glUniform1fv(this->gaussian[current_program], gaussian.size(), gaussian.data());
}

void Shader::ShaderGaussianBlur::bindSourceTexture(GLuint texture) {
	correct_programm
		glActiveTexture(GL_TEXTURE0 + gSourceTexture_slot);
	glBindTexture(GL_TEXTURE_2D, texture);
}
