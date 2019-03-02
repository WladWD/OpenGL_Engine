#include "LoadTexture.h"
#include "DecaleInstance.h"

#pragma once
namespace Decale
{
	class DecaleManager
	{
		const uint32_t vMaxDecaleTexture;
		const uint32_t vMaxDecaleCount;
		//////////////////////////////////////////////////////////////////////
		Resource::MapResource *mResource;
		GLuint mDecaleTexture;
		//////////////////////////////////////////////////////////////////////
		std::vector<DecaleInstance *> mDecaleInstance;
		std::vector<glm::mat4> mDecaleMatrix;
		std::vector<int32_t> mDecaleID;
		//////////////////////////////////////////////////////////////////////
		uint32_t mNextDecaleID;
		//////////////////////////////////////////////////////////////////////
		void LoadDecaleTextureArray(Resource::LoadTexture *mTextureLoader, std::string mDecaleImage);
	public:
		DecaleManager(Resource::LoadTexture *mTextureLoader, std::string mDecaleImage);
		~DecaleManager();
		//////////////////////////////////////////////////////////////////////
		void AppendDecale(MCamera::Camera *mCamera);
		//////////////////////////////////////////////////////////////////////
		void BindDecaleTexture(void);
		const glm::mat4 *GetDecaleMatrixes(void);
		const int32_t *GetDecaleIds(void);
		int32_t GetDecaleCount(void);
	};
}
