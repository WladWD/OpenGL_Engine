#include "ForwardDraw.h"


ForwardRendering::ForwardDraw::ForwardDraw(MEngine::MResource *mresource, MEngine::MObjects *VNT_object, MEngine::MObjects *VNTT_object, MCamera::Camera *camera) :
	mresource(mresource), VNT_object(VNT_object), VNTT_object(VNTT_object)
{
	shader_vnt = new Shader::Shader_VNT();
	draw_vnt = new DrawVNT(mresource, VNT_object, shader_vnt, camera);

	shader_vntt = new Shader::Shader_VNTT();
	draw_vntt = new DrawVNTT(mresource, VNTT_object, shader_vntt, camera);
}

ForwardRendering::ForwardDraw::~ForwardDraw() 
{
	mresource->mMaterial.clear();
	delete mresource->texture_map;
	delete mresource;

	delete shader_vnt;
	delete draw_vnt;

	delete shader_vntt;
	delete draw_vntt;
}

void ForwardRendering::ForwardDraw::Draw(float delta_ms, Atmosphere::GlobalLightingParam atmosphere_parameter)
{
	//////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////
	draw_vnt->Draw(atmosphere_parameter);
	draw_vntt->Draw(atmosphere_parameter);
	//////////////////////////////////////////////////////////
}