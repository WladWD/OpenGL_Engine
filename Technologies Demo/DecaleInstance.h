#include "MCamera.h"

#pragma once
namespace Decale
{
	class DecaleInstance
	{
		//////////////////////////////////////////////////////////////////////
		const uint32_t mId_Decale;
		//////////////////////////////////////////////////////////////////////
		glm::mat4 mProjView, mProj, mView;
		glm::vec3 mCamPos;
		glm::vec3 mCamDir;
		//////////////////////////////////////////////////////////////////////
		void Init(MCamera::Camera *mCamera);
		//////////////////////////////////////////////////////////////////////
	public:
		DecaleInstance(MCamera::Camera *mCamera, uint32_t mId_Decale);
		~DecaleInstance();
		//////////////////////////////////////////////////////////////////////
		const glm::mat4 &GetDecaleProjView(void);
		uint32_t GetDecaleId(void);
		//////////////////////////////////////////////////////////////////////
	};
}

