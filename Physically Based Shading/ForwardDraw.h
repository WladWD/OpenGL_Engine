#include "LoadModel.h"
#include "MCamera.h"
#include "DrawVNT.h"
#include "DrawVNTT.h"

#pragma once
namespace ForwardRendering
{
	class ForwardDraw
	{
		/////////////////////////////////////////////////////////////
		MEngine::MResource *mresource;
		MEngine::MObjects *VNT_object;
		MEngine::MObjects *VNTT_object;
		/////////////////////////////////////////////////////////////
		MCamera::Camera *camera;
		/////////////////////////////////////////////////////////////
		Shader::Shader_VNT *shader_vnt;
		Shader::Shader_VNTT *shader_vntt;
		/////////////////////////////////////////////////////////////
		DrawVNT *draw_vnt;
		DrawVNTT *draw_vntt;
		/////////////////////////////////////////////////////////////
	public:
		ForwardDraw(MEngine::MResource *mresource, MEngine::MObjects *VNT_object, MEngine::MObjects *VNTT_object, MCamera::Camera *camera);
		~ForwardDraw();

		void Draw(float delta_ms, Atmosphere::GlobalLightingParam atmosphere_parameter);
	};
}

