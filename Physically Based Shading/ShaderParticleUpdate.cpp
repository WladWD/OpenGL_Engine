#include "ShaderParticleUpdate.h"


Shader::ShaderParticleUpdate::ShaderParticleUpdate() : ShaderBase(1, 1)
{
	Shader_Resource shaders[1];
	shaders[0].resource_ID = IDR_SHADER24;
	//IDR_SHADER24            Shader                  "shader\\Rain\\PatricleUpdate.tf.vs"
	shaders[0].type_shader = GL_VERTEX_SHADER;

	shaders[0].define = 0;
	shaders[0].define_size = 0;

	CompileShader(shaders);

	glAttachShader(program[0], this->shaders[0]);

	/*out float	vo_random;
out uint	vo_Type;*/
	const GLchar* feedbackVaryings[5] =
	{
		"vo_pos",
		"vo_seed",
		"vo_speed",
		"vo_random",
		"vo_Type"
	};
	
	glTransformFeedbackVaryings(program[0], 5, feedbackVaryings, GL_INTERLEAVED_ATTRIBS);
	glLinkProgram(program[0]);

	GLint link_res;
	glGetProgramiv(program[0], GL_LINK_STATUS, &link_res);
	if (link_res != GL_TRUE)
	{
		MessageBox(NULL, L"Eror link shader programm!!!", L"Error", MB_OK || MB_ICONERROR);

		GLint info_length = 0;
		glGetProgramiv(program[0], GL_INFO_LOG_LENGTH, &info_length);
		if (info_length > 0)
		{
			char *info_message = new char[info_length];

			glGetProgramInfoLog(program[0], info_length, NULL, info_message);
			MessageBoxA(NULL, info_message, "", MB_OK || MB_ICONINFORMATION);

			delete[] info_message;
		}
		throw L"Programm Link Error";
	}

	MInitShaderVariable();
}

Shader::ShaderParticleUpdate::~ShaderParticleUpdate() { }

void Shader::ShaderParticleUpdate::InitVariable(GLuint program)
{
	correct_programm

	g_dt_s[current_program] = glGetUniformLocation(program, to_shader_name(g_dt_s));
	g_TotalVel[current_program] = glGetUniformLocation(program, to_shader_name(g_TotalVel));
	g_EyePos[current_program] = glGetUniformLocation(program, to_shader_name(g_EyePos));
	g_HeightRange[current_program] = glGetUniformLocation(program, to_shader_name(g_HeightRange));
}

GLuint Shader::ShaderParticleUpdate::GetProgram(void)
{
	current_program = 0;
	return program[0];
}

void Shader::ShaderParticleUpdate::SetDtSecond(float dt)
{
	correct_programm
	mcheck(g_dt_s) glUniform1f(g_dt_s[current_program], dt);
}

void Shader::ShaderParticleUpdate::SetTotalVel(glm::vec3 vector)
{
	correct_programm
	mcheck(g_TotalVel) glUniform3fv(g_TotalVel[current_program], 1,  glm::value_ptr(vector));
}

void Shader::ShaderParticleUpdate::SetEyePos(glm::vec3 vector) 
{
	correct_programm
	mcheck(g_EyePos) glUniform3fv(g_EyePos[current_program], 1, glm::value_ptr(vector));
}

void Shader::ShaderParticleUpdate::SetHeightRage(float val) 
{
	correct_programm
	mcheck(g_HeightRange) glUniform1f(g_HeightRange[current_program], val);
}
