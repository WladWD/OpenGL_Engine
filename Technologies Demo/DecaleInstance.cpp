#include "DecaleInstance.h"


void Decale::DecaleInstance::Init(MCamera::Camera * mCamera)
{
	float mUnit = mCamera->GetMeter() * 0.01;

	mView = mCamera->GetViewMatrix();
	mProj = glm::ortho(mUnit * -40.0f, mUnit * 40.0f, mUnit * -40.0f, mUnit * 40.0f, mUnit * 0.0f, mUnit * 12000.0f);
	mProjView = mProj * mView;// mCamera->GetProjViewMatrix();//mProj * mView;
	mCamDir = mCamera->GetCamDir();
	mCamPos = mCamera->GetPosition();
}

Decale::DecaleInstance::DecaleInstance(MCamera::Camera * mCamera, uint32_t mId_Decale) : mId_Decale(mId_Decale)
{
	Init(mCamera);
}

Decale::DecaleInstance::~DecaleInstance()
{
}

const glm::mat4 & Decale::DecaleInstance::GetDecaleProjView(void)
{
	return mProjView;
}

uint32_t Decale::DecaleInstance::GetDecaleId(void)
{
	return mId_Decale;
}
