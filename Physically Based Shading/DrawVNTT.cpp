#include "DrawVNTT.h"


ForwardRendering::DrawVNTT::DrawVNTT(MEngine::MResource *mresource_, MEngine::MObjects *VNTT_object_, Shader::Shader_VNTT *shader_vntt_, MCamera::Camera *camera) :
	shader_vntt(shader_vntt_), camera(camera), mresource(mresource_), VNTT_object(VNTT_object_)
{

}

ForwardRendering::DrawVNTT::~DrawVNTT()
{
	VNTT_object->mEntries.clear();

	if (VNTT_object->VAO)glDeleteVertexArrays(1, &VNTT_object->VAO);
	if (VNTT_object->IB)glDeleteBuffers(1, &VNTT_object->IB);
	if (VNTT_object->VB)glDeleteBuffers(1, &VNTT_object->VB);

	delete VNTT_object;
}

void ForwardRendering::DrawVNTT::InitBuffer(GLuint program)
{
	GLint artib1 = glGetAttribLocation(program, "v_position");
	GLint artib2 = glGetAttribLocation(program, "v_normal");
	GLint artib3 = glGetAttribLocation(program, "m_tn");
	GLint artib4 = glGetAttribLocation(program, "v_tex");

	uint32_t mvert_size = sizeof(MeshLoad::MVertexBump);
	uint32_t offset = sizeof(vec3), offset1 = offset * 2, offset2 = offset * 3;
	glBindVertexArray(VNTT_object->VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VNTT_object->VB);

	glVertexAttribPointer(artib1, 3, GL_FLOAT, GL_FALSE, mvert_size, NULL);
	glVertexAttribPointer(artib2, 3, GL_FLOAT, GL_FALSE, mvert_size, (const void *)(offset ));
	glVertexAttribPointer(artib3, 3, GL_FLOAT, GL_FALSE, mvert_size, (const void *)(offset1));
	glVertexAttribPointer(artib4, 2, GL_FLOAT, GL_FALSE, mvert_size, (const void *)(offset2));

	glEnableVertexAttribArray(artib1);
	glEnableVertexAttribArray(artib2);
	glEnableVertexAttribArray(artib3);
	glEnableVertexAttribArray(artib4);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VNTT_object->IB);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glBindVertexArray(VNTT_object->VAO);
}

void ForwardRendering::DrawVNTT::Draw(Atmosphere::GlobalLightingParam atmosphere_parameter)
{
	if (!VNTT_object->VAO || !VNTT_object->IB || !VNTT_object->VB) return;

	GLuint current_program = (-1);
	current_program = shader_vntt->GetProgramDiff();

	glUseProgram(current_program);
	shader_vntt->BindTextureToUnit();
	InitBuffer(current_program);

	uint32_t last_index = 0;
	for (register uint32_t i = 0; i < VNTT_object->mEntries.size(); i++, last_index++)
	{
		uint32_t matID = VNTT_object->mEntries[i].GetMaterialID();

		MeshLoad::ObjectMaterial mMaterial = mresource->mMaterial[matID];

		if (VNTT_object->mEntries[i].IsUseReflaction()) break;

		bool is_clip_opacity = VNTT_object->mEntries[i].IsClipOpacity();
		if (is_clip_opacity && glIsEnabled(GL_CULL_FACE))glDisable(GL_CULL_FACE);
		else if(!glIsEnabled(GL_CULL_FACE) && !is_clip_opacity)glEnable(GL_CULL_FACE);
		
		shader_vntt->SetProjView(camera->GetProjViewMatrix());
		shader_vntt->SetCamPos(camera->GetPosition());

		shader_vntt->SetLightDir(atmosphere_parameter.light_dir);
		shader_vntt->SetLightDiffuse(atmosphere_parameter.light_diffuse_color);
		shader_vntt->SetLightSpecular(atmosphere_parameter.light_specurar_color);

		shader_vntt->SetMaterialDiffuse(mMaterial.material.diffuse_color);
		shader_vntt->SetMaterialSpecular(mMaterial.material.specular_color);
		shader_vntt->SetMaterialEmissive(mMaterial.material.emissive_color);

		Resource::GLTextureResource mtexture_diff = mresource->texture_map->GetResourceByID(mMaterial.diffuse_map_id);
		Resource::GLTextureResource mtexture_norm = mresource->texture_map->GetResourceByID(mMaterial.bump_map_id);

		shader_vntt->SetTextureDiffuse();
		glBindTexture(mtexture_diff.texture_target, mtexture_diff.texture);	

		shader_vntt->SetTextureNormal();
		glBindTexture(mtexture_norm.texture_target, mtexture_norm.texture);

		glDrawElementsBaseVertex(GL_TRIANGLES, VNTT_object->mEntries[i].GetIndexCount(), GL_UNSIGNED_INT,
			(GLvoid *)(VNTT_object->mEntries[i].GetIndexStart() * sizeof(uint32_t)), VNTT_object->mEntries[i].GetVertexStart());
	}

	current_program = shader_vntt->GetProgramDiffReflect();

	glUseProgram(current_program);
	shader_vntt->BindTextureToUnit();
	InitBuffer(current_program);

	for (register uint32_t i = last_index; i < VNTT_object->mEntries.size(); i++, last_index++)
	{
		if (!VNTT_object->mEntries[i].IsUseDiffuse())break;//удалить при forward render

		uint32_t matID = VNTT_object->mEntries[i].GetMaterialID();

		bool is_clip_opacity = VNTT_object->mEntries[i].IsClipOpacity();
		if (is_clip_opacity && glIsEnabled(GL_CULL_FACE))glDisable(GL_CULL_FACE);
		else if (!glIsEnabled(GL_CULL_FACE) && !is_clip_opacity)glEnable(GL_CULL_FACE);

		MeshLoad::ObjectMaterial mMaterial = mresource->mMaterial[matID];

		shader_vntt->SetProjView(camera->GetProjViewMatrix());
		shader_vntt->SetCamPos(camera->GetPosition());

		shader_vntt->SetLightDir(atmosphere_parameter.light_dir);
		shader_vntt->SetLightDiffuse(atmosphere_parameter.light_diffuse_color);
		shader_vntt->SetLightSpecular(atmosphere_parameter.light_specurar_color);

		shader_vntt->SetMaterialDiffuse(mMaterial.material.diffuse_color);
		shader_vntt->SetMaterialSpecular(mMaterial.material.specular_color);
		shader_vntt->SetMaterialEmissive(mMaterial.material.emissive_color);

		Resource::GLTextureResource mtexture_diff = mresource->texture_map->GetResourceByID(mMaterial.diffuse_map_id);
		Resource::GLTextureResource mtexture_spec = mresource->texture_map->GetResourceByID(mMaterial.specular_map_id);
		Resource::GLTextureResource mtexture_reflect = mresource->texture_map->GetResourceByID(mMaterial.reflaction_map_id);
		Resource::GLTextureResource mtexture_norm = mresource->texture_map->GetResourceByID(mMaterial.bump_map_id);

		shader_vntt->SetTextureDiffuse();
		glBindTexture(mtexture_diff.texture_target, mtexture_diff.texture);

		shader_vntt->SetTextureReflaction();
		glBindTexture(mtexture_reflect.texture_target, mtexture_reflect.texture);

		shader_vntt->SetTextureSpecular();
		glBindTexture(mtexture_spec.texture_target, mtexture_spec.texture);

		shader_vntt->SetTextureNormal();
		glBindTexture(mtexture_norm.texture_target, mtexture_norm.texture);

		glDrawElementsBaseVertex(GL_TRIANGLES, VNTT_object->mEntries[i].GetIndexCount(), GL_UNSIGNED_INT,
			(GLvoid *)(VNTT_object->mEntries[i].GetIndexStart() * sizeof(uint32_t)), VNTT_object->mEntries[i].GetVertexStart());
	}

	shader_vntt->ResetProgram();

	glBindTexture(GL_TEXTURE_2D, 0);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}