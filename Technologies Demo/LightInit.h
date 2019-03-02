#include "LightManager.h"

#pragma once
namespace ComputeTileShading
{
	class LightInit
	{
		//////////////////////////////////////////////////////////////
		void Init(LightManager *light_data);
	public:
		LightInit(LightManager *light_data);
		~LightInit();

		static void SInit(LightManager *light_data);
	};
}

