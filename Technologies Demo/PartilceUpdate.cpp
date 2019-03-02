#include "PartilceUpdate.h"


Particle::PartilceUpdate::PartilceUpdate(MCamera::Camera *camera) : g_PatricleCount(150000), camera(camera)
{
	meter = camera->GetMeter();
	g_radiusRange = 35.0f*meter;//45
	g_heightRange = 40.0f*meter;
	shader_particle_update = new Shader::ShaderParticleUpdate();

	GenParticleBuffer();
	cicle = 0;

	SetupSystem();
}

Particle::PartilceUpdate::~PartilceUpdate()
{
	delete shader_particle_update;

	glDeleteTransformFeedbacks(3, g_TransformFeedback);
	glDeleteVertexArrays(3, g_ParticlesVertexArray);
	glDeleteBuffers(3, g_ParticlesBuffers);
}

float Particle::PartilceUpdate::random(void)
{
	return (float((double)rand() / ((double)(RAND_MAX)+(double)(1))));
}

void Particle::PartilceUpdate::GenParticleBuffer(void)
{
	RainVertex *vertices = new RainVertex[g_PatricleCount];

	glm::vec3 g_vecEye = camera->GetPosition();
	for (register int i = 0; i < g_PatricleCount; i++) 
	{
		RainVertex raindrop;
		float SeedX;
		float SeedZ;

		bool pointIsInside = false;
		while(!pointIsInside)
		{
			SeedX = random() - 0.5f;
			SeedZ = random() - 0.5f;

			if (sqrt(SeedX*SeedX + SeedZ*SeedZ) <= 0.5f) pointIsInside = true;
		}

		SeedX *= g_radiusRange;
		SeedZ *= g_radiusRange;
		float SeedY = random()*g_heightRange;// *0.6f + g_heightRange * 0.4f;
		raindrop.seed = glm::vec3(SeedX, SeedY, SeedZ);

		float SpeedX = random();//40.0f*(random() / 20.0f);
		float SpeedZ = random();//40.0f*(random() / 20.0f);
		float SpeedY = random() * 2.0f;//40.0f*(random() / 10.0f);
		SpeedX = SpeedX*2.0f - 1.0f;
		SpeedZ = SpeedZ*2.0f - 1.0f;
		SpeedX *= 5.0f;//8.0f;
		SpeedZ *= 5.0f;//8.0f;
		raindrop.speed = glm::vec3(SpeedX, SpeedY, SpeedZ)*meter;

		float x = SeedX + g_vecEye.x;
		float z = SeedZ + g_vecEye.z;
		float y = SeedY + g_vecEye.y;
		raindrop.pos = glm::vec3(x, y, z);

		raindrop.Type = uint32_t(random() * 370.0f);//int(floor(random() * 8 + 1));

		raindrop.random = 1;
		float randomIncrease = random();
		if (randomIncrease > 0.8) raindrop.random += randomIncrease;

		vertices[i] = raindrop;
	}

	glGenTransformFeedbacks(3, g_TransformFeedback);
	glGenBuffers(3, g_ParticlesBuffers);
	glGenVertexArrays(3, g_ParticlesVertexArray);
	for (register uint32_t i = 0; i < 2; i++) 
	{
		glBindVertexArray(g_ParticlesVertexArray[i]);
		glBindBuffer(GL_ARRAY_BUFFER, g_ParticlesBuffers[i]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(RainVertex) * g_PatricleCount, NULL, GL_STATIC_DRAW);

		glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, g_TransformFeedback[i]);
		glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, g_ParticlesBuffers[i]);
		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		//glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, 0);
	}
	glBindVertexArray(g_ParticlesVertexArray[2]);
	glBindBuffer(GL_ARRAY_BUFFER, g_ParticlesBuffers[2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(RainVertex) * g_PatricleCount, vertices, GL_STATIC_DRAW);

	glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, g_TransformFeedback[2]);
	glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, g_ParticlesBuffers[2]);
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	delete[] vertices;
}

void Particle::PartilceUpdate::InitAttrib(GLuint program, GLuint VBO, GLuint VAO)
{
	GLint artib1 = glGetAttribLocation(program, "v_pos");
	GLint artib2 = glGetAttribLocation(program, "v_seed");
	GLint artib3 = glGetAttribLocation(program, "v_speed");
	GLint artib4 = glGetAttribLocation(program, "v_random");
	GLint artib5 = glGetAttribLocation(program, "v_Type");

	uint32_t mvert_size = sizeof(RainVertex), offset = sizeof(glm::vec3);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glVertexAttribPointer(artib1, 3, GL_FLOAT, GL_FALSE, mvert_size, NULL);
	glVertexAttribPointer(artib2, 3, GL_FLOAT, GL_FALSE, mvert_size, (const void *)(offset));
	glVertexAttribPointer(artib3, 3, GL_FLOAT, GL_FALSE, mvert_size, (const void *)(offset*2));
	glVertexAttribPointer(artib4, 1, GL_FLOAT, GL_FALSE, mvert_size, (const void *)(offset*3));
	glVertexAttribIPointer(artib5, 1, GL_UNSIGNED_INT, mvert_size, (const void *)(offset*3 + sizeof(float)));

	glEnableVertexAttribArray(artib1);
	glEnableVertexAttribArray(artib2);
	glEnableVertexAttribArray(artib3);
	glEnableVertexAttribArray(artib4);
	glEnableVertexAttribArray(artib5);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(VAO);
}

void Particle::PartilceUpdate::SetupSystem(void)
{
	glEnable(GL_RASTERIZER_DISCARD);

	glUseProgram(shader_particle_update->GetProgram());
	glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, g_TransformFeedback[0]);

	cicle = 1;
	glBeginTransformFeedback(GL_POINTS);

	shader_particle_update->SetDtSecond(0.0);
	shader_particle_update->SetEyePos(camera->GetPosition());
	shader_particle_update->SetHeightRage(g_heightRange);
	shader_particle_update->SetTotalVel(glm::vec3(0.0f, 0.0f, 0.0f));

	InitAttrib(shader_particle_update->GetProgram(), g_ParticlesBuffers[2], g_ParticlesVertexArray[2]);
	glDrawArrays(GL_POINTS, 0, g_PatricleCount);
	glEndTransformFeedback();

	glUseProgram(0);
	glBindVertexArray(0);
	glDisable(GL_RASTERIZER_DISCARD);
}

GLuint *Particle::PartilceUpdate::GetVAOs(void)
{
	return g_ParticlesVertexArray;
}

uint32_t Particle::PartilceUpdate::GetParticleCount(void)
{
	return g_PatricleCount;
}

GLuint *Particle::PartilceUpdate::GetVBOs(void)
{
	return g_ParticlesBuffers;
}

void Particle::PartilceUpdate::Update(glm::vec3 wind_dir, float dt_s, float intensity)
{
	glEnable(GL_RASTERIZER_DISCARD);
	glUseProgram(shader_particle_update->GetProgram());
	glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, g_TransformFeedback[cicle]);

	cicle ^= 1;
	glBeginTransformFeedback(GL_POINTS);
	
	shader_particle_update->SetDtSecond( dt_s);
	shader_particle_update->SetEyePos(camera->GetPosition());
	shader_particle_update->SetHeightRage(g_heightRange);
	shader_particle_update->SetTotalVel(wind_dir);

	InitAttrib(shader_particle_update->GetProgram(), g_ParticlesBuffers[cicle], g_ParticlesVertexArray[cicle]);
	glDrawArrays(GL_POINTS, 0, g_PatricleCount*intensity);
	glEndTransformFeedback();

	glUseProgram(0);
	glBindVertexArray(0);
	glDisable(GL_RASTERIZER_DISCARD);
}
/*float g_ScreenWidth = 1264.0f;
float g_ScreenHeight = 958.0f;
float g_SpriteSize = 0.8;
float g_dirLightRadius = 1000;
bool g_bRenderBg = true;
bool g_bMoveParticles = true; 
bool g_bDrawParticles = true;
int g_numRainVertices = 150000;
float g_dirLightIntensity = 0.27f;
float g_PointLightIntensity = 0.58;
float g_znear = 1.0f;
float g_zfar  = 30000.0f;
float g_fov =  0.3*D3DX_PI;
float g_cosSpotLight = 0.54;
bool g_bUseSpotLight = true;
float g_responseDirLight = 0.9f;
float g_responsePointLight = 2.0f;
float g_heightMin = 0.0f;
float g_heightRange = 40.0f;
float g_radiusMin = 0.0f;
float g_radiusRange = 45.0f; 
bool g_bUseCheapShader = false;
bool firstFrame;
int frameCount = 0;
float g_WindAmount;
float g_DrawFraction = 1.0;
D3DXVECTOR3 g_fogVector;
D3DXVECTOR3 g_vecEye;
D3DXVECTOR3 g_vecAt;
D3DXVECTOR3 g_lightPos;
D3DXVECTOR3 g_eyeInObjectSpace;
D3DXVECTOR3 g_TotalVel = D3DXVECTOR3(-0.05,-0.5,0);
D3DXVECTOR3 g_PointLightPos = D3DXVECTOR3(  3.7,5.8,3.15);
D3DXVECTOR3 g_PointLightPos2 = D3DXVECTOR3(-3.7,5.8,3.15);
D3DXMATRIX g_WorldMatrix;*/