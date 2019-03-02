#include "ShaderSkyDome.h"


Shader::ShaderSkyDome::ShaderSkyDome(void) : ShaderBase(1, 2)
{
	/*
	IDR_SHADER10            Shader                  "shader\\Atmosphere\\SkyDome.ps"

	IDR_SHADER11            Shader                  "shader\\Atmosphere\\SkyDome.vs"
	*/

	Shader_Resource shaders[2];
	shaders[0].resource_ID = IDR_SHADER11;
	shaders[1].resource_ID = IDR_SHADER10;

	shaders[0].type_shader = GL_VERTEX_SHADER;
	shaders[1].type_shader = GL_FRAGMENT_SHADER;

	shaders[0].define = 0;
	shaders[0].define_size = 0;

	shaders[1].define = 0;
	shaders[1].define_size = 0;

	CompileShader(shaders);

	InitProgram(this->shaders[0], this->shaders[1], program[0]);

	MInitShaderVariable();

	RayLeigh_slot = 0;
	Mie_slot = 1;
	Star_slot = 2;
}

Shader::ShaderSkyDome::~ShaderSkyDome() { }

void Shader::ShaderSkyDome::InitVariable(GLuint program)
{
	correct_programm

	g_proj_view_world[current_program] = glGetUniformLocation(program, to_shader_name(g_proj_view_world));
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

	RayLeighTexture[current_program] = glGetUniformLocation(program, to_shader_name(RayLeighTexture));
	MieTexture[current_program] = glGetUniformLocation(program, to_shader_name(MieTexture));
	StarTexture[current_program] = glGetUniformLocation(program, to_shader_name(StarTexture));

}

void Shader::ShaderSkyDome::SetDefValue(void)
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

	tmp = 0.0025f * 80;//0.0025f * 20.0f;
	mcheck(KrESun) glUniform1fv(KrESun[current_program], 1, &tmp);

	tmp = 0.01010f * 20; //0.0010f * 20.0f;
	mcheck(KmESun) glUniform1fv(KmESun[current_program], 1, &tmp);

	tmp = 0.0025f * 4.0f * 3.1415159; //0.0025f * 4.0f * 3.1415159;
	mcheck(Kr4PI) glUniform1fv(Kr4PI[current_program], 1, &tmp);

	tmp = 0.01010f * 4.0f * 3.1415159; //0.0010f * 4.0f * 3.1415159;
	mcheck(Km4PI) glUniform1fv(Km4PI[current_program], 1, &tmp);


	tmp = -0.990;
	mcheck(g) glUniform1fv(g[current_program], 1, &tmp);

	tmp = (-0.990) * (-0.990);
	mcheck(g2) glUniform1fv(g2[current_program], 1, &tmp);

	tmp = -1.5;//-2.0;
	mcheck(fExposure) glUniform1fv(fExposure[current_program], 1, &tmp);

	tmp = 0.5f;
	mcheck(starIntensity) glUniform1fv(starIntensity[current_program], 1, &tmp);
}

GLuint Shader::ShaderSkyDome::GetProgram(void)
{
	current_program = 0;
	return program[0];
}

void Shader::ShaderSkyDome::SetInvWavelength(glm::vec3 vector)
{
	correct_programm
		mcheck(InvWavelength) glUniform3fv(InvWavelength[current_program], 1, glm::value_ptr(vector));
}

void Shader::ShaderSkyDome::SetWavelengthMie(glm::vec3 vector)
{
	correct_programm
		mcheck(WavelengthMie) glUniform3fv(WavelengthMie[current_program], 1, glm::value_ptr(vector));
}

void Shader::ShaderSkyDome::Setv3SunDir(glm::vec3 vector)
{
	correct_programm
		mcheck(v3SunDir) glUniform3fv(v3SunDir[current_program], 1, glm::value_ptr(vector));
}

void Shader::ShaderSkyDome::SetProjViewWorld(glm::mat4 matrix)
{
	correct_programm
	mcheck(g_proj_view_world) glUniformMatrix4fv(g_proj_view_world[current_program], 1, GL_FALSE, glm::value_ptr(matrix));
}

void Shader::ShaderSkyDome::SetRayLeighTexture(void)
{
	correct_programm
	glActiveTexture(GL_TEXTURE0 + RayLeigh_slot);
}

void Shader::ShaderSkyDome::SetMieTexture(void)
{
	correct_programm
	glActiveTexture(GL_TEXTURE0 + Mie_slot);
}

void Shader::ShaderSkyDome::SetStarTexture(void)
{
	correct_programm
	glActiveTexture(GL_TEXTURE0 + Star_slot);
}

void Shader::ShaderSkyDome::SetNumSamples(float num_samples_)
{
	correct_programm
	mcheck(NumSamples) glUniform1fv(NumSamples[current_program], 1, &num_samples_);
}

void Shader::ShaderSkyDome::SetExposure(float val)
{
	correct_programm
	mcheck(fExposure) glUniform1fv(fExposure[current_program], 1, &val);
}

void Shader::ShaderSkyDome::SetStarIntensity(float val)
{
	correct_programm
	mcheck(starIntensity) glUniform1fv(starIntensity[current_program], 1, &val);
}

void Shader::ShaderSkyDome::BindTextureToUnit(void)
{
	correct_programm

	mcheck(RayLeighTexture) glUniform1i(RayLeighTexture[current_program], RayLeigh_slot);
	mcheck(MieTexture) glUniform1i(MieTexture[current_program], Mie_slot);
	mcheck(StarTexture) glUniform1i(StarTexture[current_program], Star_slot);
}
