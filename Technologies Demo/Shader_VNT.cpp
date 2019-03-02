#include "Shader_VNT.h"


Shader::Shader_VNT::Shader_VNT()
{
	program[0] = glCreateProgram();
	program[1] = glCreateProgram();
	program[2] = glCreateProgram();

	if (!program[0] || !program[1] || !program[2])
	{
		MessageBox(NULL, L"Eror create program!!!", L"Error", MB_OK || MB_ICONERROR);
		throw L"Create Program Error";
	}

	Shader_Resource shaders[4];
	shaders[0].resource_ID = IDR_SHADER2;
	shaders[1].resource_ID = IDR_SHADER1;
	shaders[2].resource_ID = IDR_SHADER3;
	shaders[3].resource_ID = IDR_SHADER4;

	shaders[0].type_shader = GL_VERTEX_SHADER;
	shaders[1].type_shader = GL_FRAGMENT_SHADER;
	shaders[2].type_shader = GL_FRAGMENT_SHADER;
	shaders[3].type_shader = GL_FRAGMENT_SHADER;

	shaders[0].define = 0;
	shaders[0].define_size = 0;

	shaders[1].define = 0;
	shaders[1].define_size = 0;

	shaders[2].define = 0;
	shaders[2].define_size = 0;

	shaders[3].define = 0;
	shaders[3].define_size = 0;


	Shader_Resource_Compiler *shader_compiler = new Shader_Resource_Compiler(shaders, 4, L"Shader");
	if (!shader_compiler->GetResult())throw L"Shader Create Error";
	delete shader_compiler;

	this->shaders[0] = shaders[0].shader;
	this->shaders[1] = shaders[1].shader;//no tex
	this->shaders[2] = shaders[2].shader;//diff reflect
	this->shaders[3] = shaders[3].shader;//diff

	InitProgramm(shaders[0].shader, shaders[1].shader, program[0]);
	InitProgramm(shaders[0].shader, shaders[2].shader, program[1]);
	InitProgramm(shaders[0].shader, shaders[3].shader, program[2]);

	diff_slot = 0;
	spec_slot = 1;
	reflect_slot = 2;

	programm_id = (-1);

	InitVariable(program[1]);
}

Shader::Shader_VNT::~Shader_VNT()
{
	glDeleteShader(shaders[0]);
	glDeleteShader(shaders[1]);
	glDeleteShader(shaders[2]);
	glDeleteShader(shaders[3]);

	glDeleteProgram(program[0]);
	glDeleteProgram(program[1]);
	glDeleteProgram(program[2]);
}

void Shader::Shader_VNT::InitProgramm(GLuint vs, GLuint ps, GLuint programm)
{
	glAttachShader(programm, vs);//vertex shader
	glAttachShader(programm, ps);//fragment shader

	glLinkProgram(programm);

	GLint link_res;
	glGetProgramiv(programm, GL_LINK_STATUS, &link_res);
	if (link_res != GL_TRUE)
	{
		MessageBox(NULL, L"Eror link shader programm!!!", L"Error", MB_OK || MB_ICONERROR);
		//error

		GLint info_length = 0;
		glGetProgramiv(programm, GL_INFO_LOG_LENGTH, &info_length);
		if (info_length > 0)
		{
			char *info_message = new char[info_length];

			glGetProgramInfoLog(programm, info_length, NULL, info_message);
			MessageBoxA(NULL, info_message, "", MB_OK || MB_ICONINFORMATION);

			delete[] info_message;
		}
		//delete shader_compiler;
		throw L"Programm Link Error";
	}
}

void Shader::Shader_VNT::InitVariable(GLuint programm)
{
	g_proj_view = glGetUniformLocation(programm, "g_proj_view");
	g_cam_pos = glGetUniformLocation(programm, "g_cam_pos");
	
	m_diffuse = glGetUniformLocation(programm, "m_diffuse");//to_shader_name(m_diffuse));//"m_diffuse");
	m_specular = glGetUniformLocation(programm, "m_specular");
	m_emissive = glGetUniformLocation(programm, "m_emissive");
	
	g_diffuse = glGetUniformLocation(programm, "g_diffuse");
	g_specular = glGetUniformLocation(programm, "g_specular");
	g_reflaction = glGetUniformLocation(programm, "g_reflaction");
	
	g_lightDir = glGetUniformLocation(programm, "g_lightDir");
	g_lightDiffuse = glGetUniformLocation(programm, "g_lightDiffuse");
	g_lightSpecular = glGetUniformLocation(programm, "g_lightSpecular");

	if (g_diffuse > (-1)) glUniform1i(g_diffuse, diff_slot);
	if (g_specular > (-1))glUniform1i(g_specular, spec_slot);
	if (g_reflaction > (-1))glUniform1i(g_reflaction, reflect_slot);
}

GLuint Shader::Shader_VNT::GetProgram()
{
	programm_id = 0;
	return program[0];
}

GLuint Shader::Shader_VNT::GetProgramDiff(void)
{
	programm_id = 2;
	return program[2];
}

GLuint Shader::Shader_VNT::GetProgramDiffReflect(void)
{
	programm_id = 1;
	return program[1];
}

void Shader::Shader_VNT::BindTextureToUnit(void)
{
	if (programm_id) 
	{
		if (g_diffuse > (-1))glUniform1i(g_diffuse, diff_slot);

		if (programm_id == 1) {
			if (g_specular > (-1))glUniform1i(g_specular, spec_slot);
			if (g_reflaction > (-1))glUniform1i(g_reflaction, reflect_slot);
		}
	}
}

void Shader::Shader_VNT::SetProjView(glm::mat4 matrix)
{
	glUniformMatrix4fv(g_proj_view, 1, GL_FALSE, glm::value_ptr(matrix));
}

void Shader::Shader_VNT::SetCamPos(glm::vec3 vector)
{
	glUniform3fv(g_cam_pos, 1, glm::value_ptr(vector));
}

void Shader::Shader_VNT::SetMaterialDiffuse(glm::vec4 color)
{
	glUniform4fv(m_diffuse, 1, glm::value_ptr(color));
}

void Shader::Shader_VNT::SetMaterialSpecular(glm::vec4 color)
{
	glUniform4fv(m_specular, 1, glm::value_ptr(color));
}

void Shader::Shader_VNT::SetMaterialEmissive(glm::vec4 color)
{
	glUniform4fv(m_emissive, 1, glm::value_ptr(color));
}

void Shader::Shader_VNT::SetTextureDiffuse(void) 
{
	glActiveTexture(GL_TEXTURE0 + diff_slot);
}

void Shader::Shader_VNT::SetTextureSpecular(void) 
{
	glActiveTexture(GL_TEXTURE0 + spec_slot);
}

void Shader::Shader_VNT::SetTextureReflaction(void) 
{
	glActiveTexture(GL_TEXTURE0 + reflect_slot);
}

void Shader::Shader_VNT::SetLightDir(glm::vec3 vector)
{
	glUniform3fv(g_lightDir, 1, glm::value_ptr(vector));
}

void Shader::Shader_VNT::SetLightDiffuse(glm::vec4 color)
{
	glUniform4fv(g_lightDiffuse, 1, glm::value_ptr(color));
}

void Shader::Shader_VNT::SetLightSpecular(glm::vec4 color)
{
	glUniform4fv(g_lightSpecular, 1, glm::value_ptr(color));
}