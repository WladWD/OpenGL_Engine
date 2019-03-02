#include "Shader_VNTT.h"
#include "LoadModel.h"
#include "MCamera.h"
#include "AtmosphereParameter.h"

#pragma once
namespace ForwardRendering
{
	class DrawVNTT
	{
		/////////////////////////////////////////////////////////////
		MEngine::MResource *mresource;
		MEngine::MObjects *VNTT_object;
		/////////////////////////////////////////////////////////////
		MCamera::Camera *camera;
		/////////////////////////////////////////////////////////////
		Shader::Shader_VNTT *shader_vntt;
		/////////////////////////////////////////////////////////////
		void InitBuffer(GLuint program);
		/////////////////////////////////////////////////////////////
	public:
		DrawVNTT(MEngine::MResource *mresource, MEngine::MObjects *VNTT_object, Shader::Shader_VNTT *shader_vnt_, MCamera::Camera *camera);
		~DrawVNTT();
		/////////////////////////////////////////////////////////////
		void Draw(Atmosphere::GlobalLightingParam atmosphere_parameter);
	};
}

