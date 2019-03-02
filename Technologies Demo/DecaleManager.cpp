#include "DecaleManager.h"


Decale::DecaleManager::DecaleManager(Resource::LoadTexture *mTextureLoader, std::string mDecaleImage) : vMaxDecaleTexture(4), vMaxDecaleCount(16), mNextDecaleID(0)
{
	LoadDecaleTextureArray(mTextureLoader, mDecaleImage);
}

Decale::DecaleManager::~DecaleManager()
{
	mResource->ReleaseResource(mDecaleTexture);
	
	for (DecaleInstance *mInstance : mDecaleInstance)
		delete mInstance;
	mDecaleInstance.clear();
}

void Decale::DecaleManager::LoadDecaleTextureArray(Resource::LoadTexture *mTextureLoader, std::string mDecaleImage)
{
	mResource = mTextureLoader->GetMapResource();
	mDecaleTexture = mTextureLoader->LoadTexture2DArray(mDecaleImage, vMaxDecaleTexture);
}

void Decale::DecaleManager::AppendDecale(MCamera::Camera *mCamera)
{
	static bool mMaxDecale = false;
	if (mNextDecaleID >= vMaxDecaleCount || mMaxDecale) {
		mNextDecaleID = mNextDecaleID % vMaxDecaleCount;

		delete mDecaleInstance[mNextDecaleID];

		mDecaleInstance[mNextDecaleID] = new DecaleInstance(mCamera, 0);
		mMaxDecale = true;
	}
	else 
	{
		mDecaleInstance.push_back(new DecaleInstance(mCamera, 0)); 
		mDecaleMatrix.push_back(glm::mat4());
		mDecaleID.push_back(0);
	}

	mDecaleMatrix[mNextDecaleID] = mDecaleInstance[mNextDecaleID]->GetDecaleProjView();
	mDecaleID[mNextDecaleID] = mDecaleInstance[mNextDecaleID]->GetDecaleId();

	++mNextDecaleID;
}

void Decale::DecaleManager::BindDecaleTexture(void)
{
	auto mResTextures = mResource->GetResourceByID(mDecaleTexture);
	glBindTexture(mResTextures.texture_target, mResTextures.texture);
}

const glm::mat4 * Decale::DecaleManager::GetDecaleMatrixes(void)
{
	return mDecaleMatrix.data();
}

const int32_t * Decale::DecaleManager::GetDecaleIds(void)
{
	return mDecaleID.data();
}

int32_t Decale::DecaleManager::GetDecaleCount(void)
{
	return static_cast<int32_t>(mDecaleInstance.size());
}


