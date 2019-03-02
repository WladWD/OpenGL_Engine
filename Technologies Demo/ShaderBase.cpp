#include "ShaderBase.h"


Shader::ShaderBase::ShaderBase(uint32_t program_count_, uint32_t shader_count) : program_count(program_count_), shader_count(shader_count)
{
	MCreateProgram();
	ResetProgram();

	shaders = new GLuint[shader_count];
}

Shader::ShaderBase::~ShaderBase() 
{
	for (register uint32_t i = 0; i < shader_count; i++)
	{
		glDeleteShader(shaders[i]);
	}

	for (register uint32_t i = 0; i < program_count; i++)
	{
		glDeleteProgram(program[i]);
	}

	delete[] program;
	delete[] shaders;
}

void Shader::ShaderBase::InitProgram(GLuint vs, GLuint ps, GLuint program) 
{
	glAttachShader(program, vs);
	glAttachShader(program, ps);

	glLinkProgram(program);

	GLint link_res;
	glGetProgramiv(program, GL_LINK_STATUS, &link_res);
	if (link_res != GL_TRUE)
	{
		MessageBox(NULL, L"Eror link shader programm!!!", L"Error", MB_OK || MB_ICONERROR);

		GLint info_length = 0;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &info_length);
		if (info_length > 0)
		{
			char *info_message = new char[info_length];

			glGetProgramInfoLog(program, info_length, NULL, info_message);
			MessageBoxA(NULL, info_message, "", MB_OK || MB_ICONINFORMATION);

			delete[] info_message;
		}
		throw L"Programm Link Error";
	}
}

void Shader::ShaderBase::InitProgram(GLuint vs, GLuint gs, GLuint ps, GLuint program)
{
	glAttachShader(program, vs);
	glAttachShader(program, gs);
	glAttachShader(program, ps);

	glLinkProgram(program);

	GLint link_res;
	glGetProgramiv(program, GL_LINK_STATUS, &link_res);
	if (link_res != GL_TRUE)
	{
		MessageBox(NULL, L"Eror link shader programm!!!", L"Error", MB_OK || MB_ICONERROR);

		GLint info_length = 0;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &info_length);
		if (info_length > 0)
		{
			char *info_message = new char[info_length];

			glGetProgramInfoLog(program, info_length, NULL, info_message);
			MessageBoxA(NULL, info_message, "", MB_OK || MB_ICONINFORMATION);

			delete[] info_message;
		}
		throw L"Programm Link Error";
	}
}

void Shader::ShaderBase::InitProgram(GLuint cs, GLuint program)
{
	glAttachShader(program, cs);

	glLinkProgram(program);

	GLint link_res;
	glGetProgramiv(program, GL_LINK_STATUS, &link_res);
	if (link_res != GL_TRUE)
	{
		MessageBox(NULL, L"Eror link shader programm!!!", L"Error", MB_OK || MB_ICONERROR);

		GLint info_length = 0;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &info_length);
		if (info_length > 0)
		{
			char *info_message = new char[info_length];

			glGetProgramInfoLog(program, info_length, NULL, info_message);
			MessageBoxA(NULL, info_message, "", MB_OK || MB_ICONINFORMATION);

			delete[] info_message;
		}
		throw L"Programm Link Error";
	}
}

void Shader::ShaderBase::MCreateProgram(void)
{
	program = new GLuint[program_count];
	for (register uint32_t i = 0; i < program_count; i++)
	{
		program[i] = glCreateProgram();

		if (!program[i])
		{
			MessageBox(NULL, L"Eror create program!!!", L"Error", MB_OK || MB_ICONERROR);
			throw L"Create Program Error";
		}
	}
}

void Shader::ShaderBase::MInitShaderVariable(void)
{
	for (register uint32_t i = 0; i < program_count; i++)
	{
		current_program = i;
		InitVariable(program[i]);
	}
}

void Shader::ShaderBase::ResetProgram(void)
{
	current_program = (-1);
}

void Shader::ShaderBase::CompileShader(Shader_Resource *shaders_compile)
{
	Shader_Resource_Compiler *shader_compiler = new Shader_Resource_Compiler(shaders_compile, shader_count, L"SHADER");//SHADER Shader
	if (!shader_compiler->GetResult())throw L"Shader Create Error";
	delete shader_compiler;

	PackToShader(shaders_compile);
}

void Shader::ShaderBase::PackToShader(Shader_Resource *shaders_compile)
{
	for (register uint32_t i = 0; i < shader_count; i++) 
	{
		shaders[i] = shaders_compile[i].shader;
	}
}