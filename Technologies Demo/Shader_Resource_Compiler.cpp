#include "Shader_Resource_Compiler.h"


Shader::Shader_Resource_Compiler::Shader_Resource_Compiler(Shader_Resource *shaders, UINT count, const wchar_t *type_resource)
{
	result = true;
	for (register unsigned i = 0; i < count; i++)
	{
		shaders[i].shader = shaders[i].define ? CompileShaderFromWithMacros(shaders[i].type_shader, type_resource, shaders[i].define, shaders[i].define_size, shaders[i].resource_ID) :
			CompileShaderFrom(shaders[i].type_shader, type_resource, shaders[i].resource_ID);

		if (!shaders[i].shader)
		{
			result = false;
			break;
		}
	}
}

Shader::Shader_Resource_Compiler::~Shader_Resource_Compiler()
{
}

bool Shader::Shader_Resource_Compiler::GetResult(void)
{
	return result;
}

GLuint Shader::Shader_Resource_Compiler::CompileShaderFrom(GLenum type, const wchar_t *type_resource, unsigned int resource_ID)
{
	GLint compiler_result;
	HINSTANCE hinstance = GetModuleHandle(NULL);
	if (!hinstance)MessageBox(0, L"Error GetModuleHandle()", 0, 0);
	HRSRC shader_find = FindResource(hinstance, MAKEINTRESOURCE(resource_ID), type_resource);
	if (shader_find == NULL)
	{
		MessageBox(0, L"Не найден ресурс", 0, 0);
	}
	HGLOBAL data_shader_find = LoadResource(hinstance, shader_find);
	const GLchar *shader_code = (const GLchar *)LockResource(data_shader_find);
	GLint size = SizeofResource(hinstance, shader_find);

	GLuint shader = glCreateShader(type);
	if (!shader)
	{
		MessageBox(NULL, L"Eror create shader!!!", L"Error", MB_OK || MB_ICONERROR);
		//error
		return 0;
	}

	glShaderSource(shader, 1, &shader_code, &size);
	glCompileShader(shader);

	glGetShaderiv(shader, GL_COMPILE_STATUS, &compiler_result);
	if (compiler_result != GL_TRUE)
	{
		MessageBox(NULL, L"Eror compile shader code!!!", L"Error", MB_OK || MB_ICONERROR);
		//error

		GLint info_length = 0;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &info_length);
		if (info_length > 0)
		{
			char *info_message = new char[info_length];

			glGetShaderInfoLog(shader, info_length, NULL, info_message);
			MessageBoxA(NULL, info_message, "", MB_OK || MB_ICONINFORMATION);

			delete[] info_message;
		}
		return 0;
	}

	return shader;
}

GLuint Shader::Shader_Resource_Compiler::CompileShaderFromWithMacros(GLenum type, const wchar_t *type_resource,  char * macros, uint32_t macross_size, unsigned int resource_ID)
{
	GLint compiler_result;
	HINSTANCE hinstance = GetModuleHandle(NULL);
	if (!hinstance)MessageBox(0, L"Error GetModuleHandle()", 0, 0);
	HRSRC shader_find = FindResource(hinstance, MAKEINTRESOURCE(resource_ID), type_resource);
	if (shader_find == NULL)
	{
		MessageBox(0, L"Не найден ресурс", 0, 0);
	}
	HGLOBAL data_shader_find = LoadResource(hinstance, shader_find);
	const GLchar *shader_code = (const GLchar *)LockResource(data_shader_find);
	GLint size = SizeofResource(hinstance, shader_find);

	GLuint shader = glCreateShader(type);
	if (!shader)
	{
		MessageBox(NULL, L"Eror create shader!!!", L"Error", MB_OK || MB_ICONERROR);
		//error
		return 0;
	}


	const GLchar *shader_mas[2] = { macros , shader_code };
	GLint shader_size_mas[2] = { macross_size, size };
	glShaderSource(shader, 2, shader_mas, shader_size_mas);
	glCompileShader(shader);

	glGetShaderiv(shader, GL_COMPILE_STATUS, &compiler_result);
	if (compiler_result != GL_TRUE)
	{
		MessageBox(NULL, L"Eror compile shader code!!!", L"Error", MB_OK || MB_ICONERROR);
		//error

		GLint info_length = 0;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &info_length);
		if (info_length > 0)
		{
			char *info_message = new char[info_length];

			glGetShaderInfoLog(shader, info_length, NULL, info_message);
			MessageBoxA(NULL, info_message, "", MB_OK || MB_ICONINFORMATION);
			//MessageBox(NULL, file_name, L"", MB_OK || MB_ICONINFORMATION);
			delete[] info_message;
		}
		return 0;
	}

	return shader;
}