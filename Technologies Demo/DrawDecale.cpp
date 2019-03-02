#include "DrawDecale.h"


Decale::DrawDecale::DrawDecale(Resource::LoadTexture *mTextureLoader, std::string mDecaleImage)
{
	mShader = new Shader::ShaderDecale();
	mDecaleManager = new DecaleManager(mTextureLoader, mDecaleImage);

	Init();
}

Decale::DrawDecale::~DrawDecale()
{
	delete mDecaleManager;
	delete mShader;
}

void Decale::DrawDecale::Init(void)
{
	glUseProgram(mShader->GetProgram());

	mShader->BindTextureToUnit();

	glUseProgram(0);
}

void Decale::DrawDecale::AppendDecale(MCamera::Camera * mCamera)
{
	mDecaleManager->AppendDecale(mCamera);
}

void Decale::DrawDecale::Draw(MCamera::Camera * mCamera, DeferredRendering::GBufferData mGBufferData)
{
	//////////////////////////////////////////////////////////////////////////////////////////////
	const int32_t NUM_THREAD_X = 8;
	const int32_t NUM_THREAD_Y = 8;
	//////////////////////////////////////////////////////////////////////////////////////////////
	int32_t x = mCamera->GetWeight() / NUM_THREAD_X, y = mCamera->GetHeight() / NUM_THREAD_Y, z = 1;
	//////////////////////////////////////////////////////////////////////////////////////////////
	glUseProgram(mShader->GetProgram());

	mShader->SetDecaleTextures();
	mDecaleManager->BindDecaleTexture();

	mShader->SetDepthTexture();
	glBindTexture(GL_TEXTURE_2D, mGBufferData.g_buffer_depth);

	mShader->SetDecaleMatrixes(mDecaleManager->GetDecaleMatrixes(), mDecaleManager->GetDecaleCount());
	mShader->SetTextureID(mDecaleManager->GetDecaleIds(), mDecaleManager->GetDecaleCount());
	mShader->SetRealDecaleCount(mDecaleManager->GetDecaleCount());

	mShader->SetProjViewInv(glm::inverse(mCamera->GetProjViewMatrix()));
	
	glBindImageTexture(mShader->GetDiffuseSlot(), mGBufferData.g_buffer_textures[1], 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA8);

	glDispatchCompute(x, y, z);
	glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
	//////////////////////////////////////////////////////////////////////////////////////////////
	glUseProgram(0);
	glBindImageTexture(mShader->GetDiffuseSlot(), 0, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA8);

	glBindTexture(GL_TEXTURE_2D, mGBufferData.g_buffer_textures[1]);
	glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, 0);

	//////////////////////////////////////////////////////////////////////////////////////////////
}
