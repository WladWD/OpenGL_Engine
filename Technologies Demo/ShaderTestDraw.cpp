#include "ShaderTestDraw.h"


Shader::ShaderTestDraw::ShaderTestDraw()
{
	program = glCreateProgram();

	Shader_Resource shaders[2];
	shaders[0].resource_ID = IDR_TEST2;
	shaders[1].resource_ID = IDR_TEST1;

	shaders[0].type_shader = GL_VERTEX_SHADER;
	shaders[1].type_shader = GL_FRAGMENT_SHADER;

	shaders[0].define = 0;
	shaders[0].define_size = 0;

	shaders[1].define = 0;
	shaders[2].define_size = 0;

	Shader_Resource_Compiler *shader_compiler = new Shader_Resource_Compiler(shaders, 2, L"TEST");
	if (!shader_compiler->GetResult())throw L"Shader Create Error";
	delete shader_compiler;

	//GLuint program = glCreateProgram();
	if (!program)
	{
		MessageBox(NULL, L"Eror create program!!!", L"Error", MB_OK || MB_ICONERROR);
		//error
		//delete shader_compiler;
		throw L"Create Program Error";
		return;
	}

	glAttachShader(program, shaders[0].shader);//vertex shader
	glAttachShader(program, shaders[1].shader);//fragment shader

	glLinkProgram(program);

	GLint link_res;
	glGetProgramiv(program, GL_LINK_STATUS, &link_res);
	if (link_res != GL_TRUE)
	{
		MessageBox(NULL, L"Eror link shader programm!!!", L"Error", MB_OK || MB_ICONERROR);
		//error

		GLint info_length = 0;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &info_length);
		if (info_length > 0)
		{
			char *info_message = new char[info_length];

			glGetProgramInfoLog(program, info_length, NULL, info_message);
			MessageBoxA(NULL, info_message, "", MB_OK || MB_ICONINFORMATION);

			delete[] info_message;
		}
		//delete shader_compiler;
		throw L"Programm Link Error";
		return;
	}

	this->shaders[0] = shaders[0].shader;
	this->shaders[1] = shaders[1].shader;

	this->program = program;

	proj_view_world = glGetUniformLocation(program, "proj_view_world");
	g_cam_pos = glGetUniformLocation(program, "g_cam_pos");
	diffuse = glGetUniformLocation(program, "diffuse");
	specular = glGetUniformLocation(program, "specular");
	emissive = glGetUniformLocation(program, "emissive"); 
	g_diffuse = glGetUniformLocation(program, "g_diffuse");
	g_lightDir = glGetUniformLocation(program, "g_lightDir");
	g_lightAmbient = glGetUniformLocation(program, "g_lightAmbient");
	g_lightDiffuse = glGetUniformLocation(program, "g_lightDiffuse");
	g_lightSpecular = glGetUniformLocation(program, "g_lightSpecular");
}

Shader::ShaderTestDraw::~ShaderTestDraw()
{
	glDeleteShader(shaders[0]);
	glDeleteShader(shaders[1]);

	glDeleteProgram(program);
}

GLuint Shader::ShaderTestDraw::GetProgram()
{
	return program;
}

void Shader::ShaderTestDraw::SetProjViewWorld(glm::mat4 matrix)
{
	glUniformMatrix4fv(proj_view_world, 1, GL_FALSE, glm::value_ptr(matrix));
}

void Shader::ShaderTestDraw::SetCamPos(glm::vec3 vector) 
{
	glUniform3fv(g_cam_pos, 1, glm::value_ptr(vector));
}

void Shader::ShaderTestDraw::SetMaterialDiffuse(glm::vec4 color) 
{
	glUniform4fv(diffuse, 1, glm::value_ptr(color));
}

void Shader::ShaderTestDraw::SetMaterialSpecular(glm::vec4 color) 
{
	glUniform4fv(specular, 1, glm::value_ptr(color));
}

void Shader::ShaderTestDraw::SetMaterialEmissive(glm::vec4 color) 
{
	glUniform4fv(emissive, 1, glm::value_ptr(color));
}

void Shader::ShaderTestDraw::BindTextureDiffuse(GLenum target, GLuint texture) 
{
	glUniform1i(g_diffuse, 0);
	glActiveTexture(GL_TEXTURE0);
}

void Shader::ShaderTestDraw::SetLightDir(glm::vec3 vector) 
{
	glUniform3fv(g_lightDir, 1, glm::value_ptr(vector));
}

void Shader::ShaderTestDraw::SetLightAmbient(glm::vec4 color) 
{
	glUniform4fv(g_lightAmbient, 1, glm::value_ptr(color));
}

void Shader::ShaderTestDraw::SetLightDiffuse(glm::vec4 color) 
{
	glUniform4fv(g_lightDiffuse, 1, glm::value_ptr(color));
}

void Shader::ShaderTestDraw::SetLightSpecular(glm::vec4 color) 
{
	glUniform4fv(g_lightSpecular, 1, glm::value_ptr(color));
}