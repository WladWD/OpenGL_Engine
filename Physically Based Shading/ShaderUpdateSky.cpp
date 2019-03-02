#include "ShaderUpdateSky.h"


Shader::ShaderUpdateSky::ShaderUpdateSky(void) : ShaderBase(1, 2)
{
	/*
	IDR_SHADER8             Shader                  "shader\\Atmosphere\\UpdateQuad.ps"

	IDR_SHADER9             Shader                  "shader\\Atmosphere\\UpdateQuad.vs"
	*/

	Shader_Resource shaders[2];
	shaders[0].resource_ID = IDR_SHADER9;
	shaders[1].resource_ID = IDR_SHADER8;

	shaders[0].type_shader = GL_VERTEX_SHADER;
	shaders[1].type_shader = GL_FRAGMENT_SHADER;

	shaders[0].define = 0;
	shaders[0].define_size = 0;

	shaders[1].define = 0;
	shaders[1].define_size = 0;

	CompileShader(shaders);

	InitProgram(this->shaders[0], this->shaders[1], program[0]);

	MInitShaderVariable();
}

Shader::ShaderUpdateSky::~ShaderUpdateSky() { }

void Shader::ShaderUpdateSky::InitVariable(GLuint program)
{
	correct_programm

	InvOpticalDepthNLessOne[current_program] = glGetUniformLocation(program, to_shader_name(InvOpticalDepthNLessOne));
	InvOpticalDepthN[current_program] = glGetUniformLocation(program, to_shader_name(InvOpticalDepthN));
	InnerRadius[current_program] = glGetUniformLocation(program, to_shader_name(InnerRadius));
	OuterRadius[current_program] = glGetUniformLocation(program, to_shader_name(OuterRadius));
	mPI[current_program] = glGetUniformLocation(program, "PI");
	NumSamples[current_program] = glGetUniformLocation(program, to_shader_name(NumSamples));
	fScale[current_program] = glGetUniformLocation(program, to_shader_name(fScale));
	v2dRayleighMieScaleHeight[current_program] = glGetUniformLocation(program, to_shader_name(v2dRayleighMieScaleHeight));

	InvRayleighMieNLessOne[current_program] = glGetUniformLocation(program, to_shader_name(InvRayleighMieNLessOne));
	v3SunDir[current_program] = glGetUniformLocation(program, to_shader_name(v3SunDir));
	KrESun[current_program] = glGetUniformLocation(program, to_shader_name(KrESun));
	KmESun[current_program] = glGetUniformLocation(program, to_shader_name(KmESun));
	Kr4PI[current_program] = glGetUniformLocation(program, to_shader_name(Kr4PI));
	Km4PI[current_program] = glGetUniformLocation(program, to_shader_name(Km4PI));

	g[current_program] = glGetUniformLocation(program, to_shader_name(g));
	g2[current_program] = glGetUniformLocation(program, to_shader_name(g2));
	fExposure[current_program] = glGetUniformLocation(program, to_shader_name(fExposure));

	InvWavelength[current_program] = glGetUniformLocation(program, to_shader_name(InvWavelength));
	WavelengthMie[current_program] = glGetUniformLocation(program, to_shader_name(WavelengthMie));

	starIntensity[current_program] = glGetUniformLocation(program, to_shader_name(starIntensity));
}

void Shader::ShaderUpdateSky::SetDefValue(void)
{
	correct_programm

	float tmp;

	tmp = 1.0f / 255.0f;
	mcheck(InvOpticalDepthNLessOne) glUniform1fv(InvOpticalDepthNLessOne[current_program], 1, &tmp);

	tmp = 1.0f / 256.0f;
	mcheck(InvOpticalDepthN) glUniform1fv(InvOpticalDepthN[current_program], 1, &tmp);

	tmp = 6356.7523142;
	mcheck(InnerRadius) glUniform1fv(InnerRadius[current_program], 1, &tmp);

	tmp = 6356.7523142 * 1.0157313;
	mcheck(OuterRadius) glUniform1fv(OuterRadius[current_program], 1, &tmp);

	tmp = 3.1415159;
	mcheck(mPI) glUniform1fv(mPI[current_program], 1, &tmp);

	tmp = 20;
	mcheck(NumSamples) glUniform1fv(NumSamples[current_program], 1, &tmp);

	tmp = 1.0 / (6356.7523142 * 1.0157313 - 6356.7523142);
	mcheck(fScale) glUniform1fv(fScale[current_program], 1, &tmp);
	mcheck(v2dRayleighMieScaleHeight) glUniform2fv(v2dRayleighMieScaleHeight[current_program], 1, glm::value_ptr(glm::vec2(0.9, 0.1)));//0.25, 0.1)));


	mcheck(InvRayleighMieNLessOne) glUniform2fv(InvRayleighMieNLessOne[current_program], 1, glm::value_ptr(glm::vec2(1.0f / 255.0f, 1.0f / 127.0f)));
	mcheck(v3SunDir) glUniform3fv(v3SunDir[current_program], 1, glm::value_ptr(glm::vec3(0, 1, 0)));

	/*float KrESun = 0.0025f * 80;
float KmESun = 0.01010f * 20;
float Kr4PI = 0.0025f * 4.0f * 3.1415159;
float Km4PI = 0.01010f * 4.0f * 3.1415159;*/

	tmp = 0.0025f * 40.0f;//0.0025f * 80;// 0.0025f * 20.0f;//
	mcheck(KrESun) glUniform1fv(KrESun[current_program], 1, &tmp);

	tmp = 0.01010f * 20; //0.0010f * 20.0f;
	mcheck(KmESun) glUniform1fv(KmESun[current_program], 1, &tmp);

	tmp = 0.0025f * 4.0f * 3.1415159;//0.0025f * 4.0f * 3.1415159; //
	mcheck(Kr4PI) glUniform1fv(Kr4PI[current_program], 1, &tmp);

	tmp = 0.01010f * 4.0f * 3.1415159; //0.0010f * 4.0f * 3.1415159;
	mcheck(Km4PI) glUniform1fv(Km4PI[current_program], 1, &tmp);


	tmp = -0.990;
	mcheck(g) glUniform1fv(g[current_program], 1, &tmp);

	tmp = (-0.990) * (-0.990);
	mcheck(g2) glUniform1fv(g2[current_program], 1, &tmp);

	tmp = -2.0;// -1.5;//
	mcheck(fExposure) glUniform1fv(fExposure[current_program], 1, &tmp);

	tmp = 0.5f;
	mcheck(starIntensity) glUniform1fv(starIntensity[current_program], 1, &tmp);
}

GLuint Shader::ShaderUpdateSky::GetProgram(void)
{
	current_program = 0;
	return program[0];
}

void Shader::ShaderUpdateSky::SetInvWavelength(glm::vec3 vector)
{
	correct_programm
	mcheck(InvWavelength) glUniform3fv(InvWavelength[current_program], 1, glm::value_ptr(vector));
}

void Shader::ShaderUpdateSky::SetWavelengthMie(glm::vec3 vector)
{
	correct_programm
	mcheck(WavelengthMie) glUniform3fv(WavelengthMie[current_program], 1, glm::value_ptr(vector));
}

void Shader::ShaderUpdateSky::Setv3SunDir(glm::vec3 vector)
{
	correct_programm
	mcheck(v3SunDir) glUniform3fv(v3SunDir[current_program], 1, glm::value_ptr(vector));
}