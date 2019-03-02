#include "MeshObject.h"


MEngine::MeshObject::MeshObject(void) 
{
	use_specular = false;
	use_bump = false;
	use_diffuse = false;
	use_clip_opacity = false;
	use_alpha = false;
	use_height_in_alpha = false;
}

MEngine::MeshObject::~MeshObject() 
{}

void MEngine::MeshObject::SetObjectName(const char *name_material, uint32_t lenght)
{
	name_object_size = std::min(lenght, (uint32_t)80);
	memcpy(name_object, name_material, name_object_size);
	name_object[name_object_size] = 0;
}

void MEngine::MeshObject::SetMaterialID(uint32_t id)
{
	material_id = id;
}

void MEngine::MeshObject::SetVertexCount(uint32_t count)
{
	vertex_count = count;
}
									
void MEngine::MeshObject::SetFaceCount(uint32_t face_count) 
{
	this->face_count = face_count;
}

void MEngine::MeshObject::SetType(MeshLoad::ID_Face type) 
{
	this->type = type;
}

bool MEngine::MeshObject::IsClipOpacity(void)
{
	return use_clip_opacity;
}

bool MEngine::MeshObject::IsUseAlpha(void)
{
	return use_alpha;
}

bool MEngine::MeshObject::UseBump(void)
{
	return use_bump;
}

bool MEngine::MeshObject::UseHeightAlpha(void)
{
	return use_height_in_alpha;
}

void MEngine::MeshObject::SetIndexCount(uint32_t value) 
{
	index_count = value;
}

void MEngine::MeshObject::SetIndexStart(uint32_t value) 
{
	index_buffer_start = value;
}

void MEngine::MeshObject::SetVertexStart(uint32_t value) 
{
	vectex_buffer_start = value;
}

uint32_t MEngine::MeshObject::GetIndexCount(void)
{
	return index_count;
}

uint32_t MEngine::MeshObject::GetIndexStart(void)
{
	return index_buffer_start;
}

uint32_t MEngine::MeshObject::GetVertexStart(void)
{
	return vectex_buffer_start;
}

MeshLoad::DrawParam MEngine::MeshObject::GetDrawParam(void) 
{
	MeshLoad::DrawParam draw_param;
	draw_param.index_buffer_start = index_buffer_start;
	draw_param.index_count = index_count;
	draw_param.vectex_buffer_start = vectex_buffer_start;

	return draw_param;
}

uint32_t MEngine::MeshObject::GetMaterialID(void)
{
	return material_id;
}

uint32_t MEngine::MeshObject::GetVertexCount(void)
{
	return vertex_count;
}

uint32_t MEngine::MeshObject::GetFaceCount(void)
{
	return face_count;
}

void MEngine::MeshObject::SetClipOpacity(bool val)
{
	use_clip_opacity = val;
}

void MEngine::MeshObject::SetUseAlpha(bool val)
{
	use_alpha = val;
}
						   
void MEngine::MeshObject::SetUseBump(bool val)
{
	use_bump = val;
}
						  
void MEngine::MeshObject::SetUseHeightAlpha(bool val)
{
	use_height_in_alpha = val;
}

bool MEngine::MeshObject::IsUseReflaction(void)
{
	return use_reflaction;
}

void MEngine::MeshObject::SetUseReflaction(bool val)
{
	use_reflaction = val;
}

bool MEngine::MeshObject::IsUseDiffuse(void)
{
	return use_diffuse;
}

void MEngine::MeshObject::SetUseDiffuse(bool val) 
{
	use_diffuse = val;
}

void MEngine::MeshObject::SetUseSpecular(bool val)
{
	use_specular = val;
}

bool MEngine::MeshObject::IsUseSpecular(void)
{
	return use_specular;
}