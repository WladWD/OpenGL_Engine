#include "Shader_VNT.h"
#include "LoadModel.h"
#include "AtmosphereParameter.h"
#include "MCamera.h"

#pragma once
namespace ForwardRendering
{
	class DrawVNT
	{
		/////////////////////////////////////////////////////////////
		MEngine::MResource *mresource;
		MEngine::MObjects *VNT_object;
		/////////////////////////////////////////////////////////////
		MCamera::Camera *camera;
		/////////////////////////////////////////////////////////////
		Shader::Shader_VNT *shader_vnt;
		/////////////////////////////////////////////////////////////
		void InitBuffer(GLuint program);
		/////////////////////////////////////////////////////////////
	public:
		DrawVNT(MEngine::MResource *mresource, MEngine::MObjects *VNT_object, Shader::Shader_VNT *shader_vnt_,  MCamera::Camera *camera);
		~DrawVNT();
		/////////////////////////////////////////////////////////////
		void Draw(Atmosphere::GlobalLightingParam atmosphere_parameter);
	};
}

