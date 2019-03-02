#include "ShaderBlurHorz.h"
#include "ShaderBlurVert.h"
#include <vector>

#pragma once
namespace Blur 
{
	class BlurManager
	{
		/////////////////////////////////////////////////////////////////
		std::vector<float> weight;
		/////////////////////////////////////////////////////////////////
		Shader::BlurHorz *horzBlur;
		Shader::BlurVert *vertBlur;
		/////////////////////////////////////////////////////////////////
		float gausian(float i, float omega, float u = 0.0);
	public:
		BlurManager();
		~BlurManager();
		/////////////////////////////////////////////////////////////////
		void blured(GLuint sourceTexture, GLuint destanationTexture);
		/////////////////////////////////////////////////////////////////
	};
}

