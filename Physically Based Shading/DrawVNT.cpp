#include "DrawVNT.h"


ForwardRendering::DrawVNT::DrawVNT(MEngine::MResource *mresource, MEngine::MObjects *VNT_object, Shader::Shader_VNT *shader_vnt_, MCamera::Camera *camera) :
	shader_vnt(shader_vnt_), camera(camera), mresource(mresource), VNT_object(VNT_object)
{

}

ForwardRendering::DrawVNT::~DrawVNT()
{
	VNT_object->mEntries.clear();

	if (VNT_object->VAO)glDeleteVertexArrays(1, &VNT_object->VAO);
	if (VNT_object->IB)glDeleteBuffers(1, &VNT_object->IB);
	if (VNT_object->VB)glDeleteBuffers(1, &VNT_object->VB);

	delete VNT_object;
}

void ForwardRendering::DrawVNT::InitBuffer(GLuint program)
{
	GLint artib1 = glGetAttribLocation(program, "v_position");
	GLint artib2 = glGetAttribLocation(program, "v_normal");
	GLint artib3 = glGetAttribLocation(program, "v_tex");

	glBindVertexArray(VNT_object->VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VNT_object->VB);
	glVertexAttribPointer(artib1, 3, GL_FLOAT, GL_FALSE, sizeof(vec3) + sizeof(vec3) + sizeof(vec2), NULL);
	glVertexAttribPointer(artib2, 3, GL_FLOAT, GL_FALSE, sizeof(vec3) + sizeof(vec3) + sizeof(vec2), (const void *)(sizeof(vec3)));
	glVertexAttribPointer(artib3, 2, GL_FLOAT, GL_FALSE, sizeof(vec3) + sizeof(vec3) + sizeof(vec2), (const void *)(sizeof(vec3) + sizeof(vec3)));

	glEnableVertexAttribArray(artib1);
	glEnableVertexAttribArray(artib2);
	glEnableVertexAttribArray(artib3);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VNT_object->IB);
}

void ForwardRendering::DrawVNT::Draw(Atmosphere::GlobalLightingParam atmosphere_parameter)
{
	if (!VNT_object->VAO || !VNT_object->IB || !VNT_object->VB) return;

	glUseProgram(shader_vnt->GetProgram());
	shader_vnt->BindTextureToUnit();
	InitBuffer(shader_vnt->GetProgram());

	uint32_t last_index = 0;
	for (register uint32_t i = 0; i < VNT_object->mEntries.size(); i++, last_index++)
	{
		uint32_t matID = VNT_object->mEntries[i].GetMaterialID();

		MeshLoad::ObjectMaterial mMaterial = mresource->mMaterial[matID];

		if (VNT_object->mEntries[i].IsUseDiffuse()) break;

		bool is_clip_opacity = VNT_object->mEntries[i].IsClipOpacity();
		if (is_clip_opacity && glIsEnabled(GL_CULL_FACE))glDisable(GL_CULL_FACE);
		else if (!glIsEnabled(GL_CULL_FACE) && !is_clip_opacity)glEnable(GL_CULL_FACE);

		shader_vnt->SetProjView(camera->GetProjViewMatrix());
		shader_vnt->SetCamPos(camera->GetPosition());

		shader_vnt->SetLightDir(atmosphere_parameter.light_dir);
		shader_vnt->SetLightDiffuse(atmosphere_parameter.light_diffuse_color);
		shader_vnt->SetLightSpecular(atmosphere_parameter.light_specurar_color);

		shader_vnt->SetMaterialDiffuse(mMaterial.material.diffuse_color);
		shader_vnt->SetMaterialSpecular(mMaterial.material.specular_color);
		shader_vnt->SetMaterialEmissive(mMaterial.material.emissive_color);

		glDrawElementsBaseVertex(GL_TRIANGLES, VNT_object->mEntries[i].GetIndexCount(), GL_UNSIGNED_INT,
			(GLvoid *)(VNT_object->mEntries[i].GetIndexStart() * sizeof(uint32_t)), VNT_object->mEntries[i].GetVertexStart());
	}

	glUseProgram(shader_vnt->GetProgramDiff());
	InitBuffer(shader_vnt->GetProgramDiff());
	shader_vnt->BindTextureToUnit();

	for (register uint32_t i = last_index; i < VNT_object->mEntries.size(); i++, last_index++)
	{
		if (!VNT_object->mEntries[i].IsUseDiffuse())break;//удалить при forward render

		uint32_t matID = VNT_object->mEntries[i].GetMaterialID();

		MeshLoad::ObjectMaterial mMaterial = mresource->mMaterial[matID];

		if (VNT_object->mEntries[i].IsUseReflaction()) break;

		bool is_clip_opacity = VNT_object->mEntries[i].IsClipOpacity();
		if (is_clip_opacity && glIsEnabled(GL_CULL_FACE))glDisable(GL_CULL_FACE);
		else if (!glIsEnabled(GL_CULL_FACE) && !is_clip_opacity)glEnable(GL_CULL_FACE);

		shader_vnt->SetProjView(camera->GetProjViewMatrix());
		shader_vnt->SetCamPos(camera->GetPosition());

		shader_vnt->SetLightDir(atmosphere_parameter.light_dir);
		shader_vnt->SetLightDiffuse(atmosphere_parameter.light_diffuse_color);
		shader_vnt->SetLightSpecular(atmosphere_parameter.light_specurar_color);

		shader_vnt->SetMaterialDiffuse(mMaterial.material.diffuse_color);
		shader_vnt->SetMaterialSpecular(mMaterial.material.specular_color);
		shader_vnt->SetMaterialEmissive(mMaterial.material.emissive_color);

		Resource::GLTextureResource mtexture_diff = mresource->texture_map->GetResourceByID(mMaterial.diffuse_map_id);

		shader_vnt->SetTextureDiffuse();
		glBindTexture(mtexture_diff.texture_target, mtexture_diff.texture);

		glDrawElementsBaseVertex(GL_TRIANGLES, VNT_object->mEntries[i].GetIndexCount(), GL_UNSIGNED_INT,
			(GLvoid *)(VNT_object->mEntries[i].GetIndexStart() * sizeof(uint32_t)), VNT_object->mEntries[i].GetVertexStart());
	}

	glUseProgram(shader_vnt->GetProgramDiffReflect());
	InitBuffer(shader_vnt->GetProgramDiffReflect());
	shader_vnt->BindTextureToUnit();

	for (register uint32_t i = last_index; i < VNT_object->mEntries.size(); i++, last_index++)
	{
		if (!VNT_object->mEntries[i].IsUseDiffuse())break;//удалить при forward render

		uint32_t matID = VNT_object->mEntries[i].GetMaterialID();

		bool is_clip_opacity = VNT_object->mEntries[i].IsClipOpacity();
		if (is_clip_opacity && glIsEnabled(GL_CULL_FACE))glDisable(GL_CULL_FACE);
		else if (!glIsEnabled(GL_CULL_FACE) && !is_clip_opacity)glEnable(GL_CULL_FACE);

		MeshLoad::ObjectMaterial mMaterial = mresource->mMaterial[matID];

		shader_vnt->SetProjView(camera->GetProjViewMatrix());
		shader_vnt->SetCamPos(camera->GetPosition());

		shader_vnt->SetLightDir(atmosphere_parameter.light_dir);
		shader_vnt->SetLightDiffuse(atmosphere_parameter.light_diffuse_color);
		shader_vnt->SetLightSpecular(atmosphere_parameter.light_specurar_color);

		shader_vnt->SetMaterialDiffuse(mMaterial.material.diffuse_color);
		shader_vnt->SetMaterialSpecular(mMaterial.material.specular_color);
		shader_vnt->SetMaterialEmissive(mMaterial.material.emissive_color);

		Resource::GLTextureResource mtexture_diff = mresource->texture_map->GetResourceByID(mMaterial.diffuse_map_id);
		Resource::GLTextureResource mtexture_spec = mresource->texture_map->GetResourceByID(mMaterial.specular_map_id);
		Resource::GLTextureResource mtexture_reflect = mresource->texture_map->GetResourceByID(mMaterial.reflaction_map_id);

		shader_vnt->SetTextureDiffuse();
		glBindTexture(mtexture_diff.texture_target, mtexture_diff.texture);

		shader_vnt->SetTextureReflaction();
		glBindTexture(mtexture_reflect.texture_target, mtexture_reflect.texture);

		shader_vnt->SetTextureSpecular();
		glBindTexture(mtexture_spec.texture_target, mtexture_spec.texture);

		glDrawElementsBaseVertex(GL_TRIANGLES, VNT_object->mEntries[i].GetIndexCount(), GL_UNSIGNED_INT,
			(GLvoid *)(VNT_object->mEntries[i].GetIndexStart() * sizeof(uint32_t)), VNT_object->mEntries[i].GetVertexStart());
	}

	glBindTexture(GL_TEXTURE_2D, 0);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}