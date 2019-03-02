#include "ShaderTestDraw.h"
#include "LoadModel.h"
#include "MCamera.h"

#pragma once
namespace TEST
{
	class TestDraw
	{
		/////////////////////////////////////////////////////////////
		MCamera::Camera *camera;
		/////////////////////////////////////////////////////////////
		MEngine::MObjects *scene;
		Shader::ShaderTestDraw *shader;
		/////////////////////////////////////////////////////////////
	public:
		TestDraw(MEngine::MObjects *model_, MCamera::Camera *camera);
		~TestDraw();

		void Draw(void);
	};
}

