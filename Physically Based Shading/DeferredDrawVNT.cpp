#include "DeferredDrawVNT.h"


DeferredRendering::DeferredDrawVNT::DeferredDrawVNT(MEngine::MResource *mresource, MEngine::MObjects *VNT_object, MEngine::MDrawList VNT_draw_list, MCamera::Camera *camera) :
	camera(camera), mresource(mresource), VNT_object(VNT_object), VNT_draw_list(VNT_draw_list)
{
	shader_deferred_vnt = new Shader::ShaderDeferredVNT();
	InitPrograms();
}

DeferredRendering::DeferredDrawVNT::~DeferredDrawVNT()
{
	delete shader_deferred_vnt;
	VNT_object->mEntries.clear();

	if (VNT_object->VAO)glDeleteVertexArrays(1, &VNT_object->VAO);
	if (VNT_object->IB)glDeleteBuffers(1, &VNT_object->IB);
	if (VNT_object->VB)glDeleteBuffers(1, &VNT_object->VB);

	delete VNT_object;
}

void DeferredRendering::DeferredDrawVNT::InitBuffer(GLuint program)
{
	GLint artib1 = glGetAttribLocation(program, "v_position");
	GLint artib2 = glGetAttribLocation(program, "v_normal");
	GLint artib3 = glGetAttribLocation(program, "v_tex");

	static const uint32_t size = sizeof(vec3) + sizeof(vec3) + sizeof(vec2);

	glBindVertexArray(VNT_object->VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VNT_object->VB);
	if (artib1 != (-1))glVertexAttribPointer(artib1, 3, GL_FLOAT, GL_FALSE, size, NULL);
	if (artib2 != (-1))glVertexAttribPointer(artib2, 3, GL_FLOAT, GL_FALSE, size, (const void *)(sizeof(vec3)));
	if (artib3 != (-1))glVertexAttribPointer(artib3, 2, GL_FLOAT, GL_FALSE, size, (const void *)(sizeof(vec3) + sizeof(vec3)));

	if (artib1 != (-1))glEnableVertexAttribArray(artib1);
	//else glDisableVertexAttribArray(artib1);

	if (artib2 != (-1))glEnableVertexAttribArray(artib2);
	//else glDisableVertexAttribArray(artib2);

	if (artib3 != (-1))glEnableVertexAttribArray(artib3);
	//else glDisableVertexAttribArray(artib3);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VNT_object->IB);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glBindVertexArray(VNT_object->VAO);
}

void DeferredRendering::DeferredDrawVNT::InitPrograms(void)
{
	glUseProgram(shader_deferred_vnt->GetProgramDiffSpec());
	shader_deferred_vnt->BindTextureToUnit();

	glUseProgram(shader_deferred_vnt->GetProgramDiffSpecClip());
	shader_deferred_vnt->BindTextureToUnit();

	glUseProgram(shader_deferred_vnt->GetProgramDiffSpecRefl());
	shader_deferred_vnt->BindTextureToUnit();

	glUseProgram(shader_deferred_vnt->GetProgramDiffSpecReflClip());
	shader_deferred_vnt->BindTextureToUnit();

	glUseProgram(shader_deferred_vnt->GetProgramDiffuse());
	shader_deferred_vnt->BindTextureToUnit();

	glUseProgram(shader_deferred_vnt->GetProgramDiffuseClip());
	shader_deferred_vnt->BindTextureToUnit();

	glUseProgram(shader_deferred_vnt->GetProgramNoTexture());
	shader_deferred_vnt->BindTextureToUnit();

	shader_deferred_vnt->ResetProgram();

	glUseProgram(0);
}

void DeferredRendering::DeferredDrawVNT::DrawEarlyDepthTest(void)
{
	if(!glIsEnabled(GL_CULL_FACE)) glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
	glCullFace(GL_BACK);
	glDisable(GL_CULL_FACE);
	//[0;c0)
	//[c0;c1)
	//[c1;c2)
	//[c2;c3)

	glUseProgram(shader_deferred_vnt->GetProgramNoTexture());
	InitBuffer(shader_deferred_vnt->GetProgramNoTexture());
	for (register uint32_t i = 0; i < VNT_draw_list.c0; i++)
	{
		uint32_t matID = VNT_object->mEntries[i].GetMaterialID();

		MeshLoad::ObjectMaterial mMaterial = mresource->mMaterial[matID];

		shader_deferred_vnt->SetProjViewWorld(camera->GetProjViewMatrix());
		shader_deferred_vnt->SetCamPos(camera->GetPosition());

		shader_deferred_vnt->SetDiffuseColor(mMaterial.material.diffuse_color);
		shader_deferred_vnt->SetSpecularColor(mMaterial.material.specular_color);
		shader_deferred_vnt->SetEmissiveColor(mMaterial.material.emissive_color);
		shader_deferred_vnt->SetPBSData(glm::vec4(mMaterial.material.mMetallic, mMaterial.material.mRoughness, mMaterial.material.mF0, 1.0f));

		glDrawElementsBaseVertex(GL_TRIANGLES, VNT_object->mEntries[i].GetIndexCount(), GL_UNSIGNED_INT,
			(GLvoid *)(VNT_object->mEntries[i].GetIndexStart() * sizeof(uint32_t)), VNT_object->mEntries[i].GetVertexStart());
	}

	glUseProgram(shader_deferred_vnt->GetProgramDiffuse());
	InitBuffer(shader_deferred_vnt->GetProgramDiffuse());
	for (register uint32_t i = VNT_draw_list.c0; i < VNT_draw_list.c1; i++)
	{
		uint32_t matID = VNT_object->mEntries[i].GetMaterialID();

		MeshLoad::ObjectMaterial mMaterial = mresource->mMaterial[matID];

		shader_deferred_vnt->SetProjViewWorld(camera->GetProjViewMatrix());
		shader_deferred_vnt->SetCamPos(camera->GetPosition());

		Resource::GLTextureResource mtexture_diff = mresource->texture_map->GetResourceByID(mMaterial.diffuse_map_id);

		shader_deferred_vnt->SetDiffuseTex();
		glBindTexture(mtexture_diff.texture_target, mtexture_diff.texture);

		shader_deferred_vnt->SetDiffuseColor(mMaterial.material.diffuse_color);
		shader_deferred_vnt->SetSpecularColor(mMaterial.material.specular_color);
		shader_deferred_vnt->SetEmissiveColor(mMaterial.material.emissive_color);
		shader_deferred_vnt->SetPBSData(glm::vec4(mMaterial.material.mMetallic, mMaterial.material.mRoughness, mMaterial.material.mF0, 1.0f));

		glDrawElementsBaseVertex(GL_TRIANGLES, VNT_object->mEntries[i].GetIndexCount(), GL_UNSIGNED_INT,
			(GLvoid *)(VNT_object->mEntries[i].GetIndexStart() * sizeof(uint32_t)), VNT_object->mEntries[i].GetVertexStart());
	}

	glUseProgram(shader_deferred_vnt->GetProgramDiffSpec());
	InitBuffer(shader_deferred_vnt->GetProgramDiffSpec());
	for (register uint32_t i = VNT_draw_list.c1; i < VNT_draw_list.c2; i++)
	{
		uint32_t matID = VNT_object->mEntries[i].GetMaterialID();

		MeshLoad::ObjectMaterial mMaterial = mresource->mMaterial[matID];

		shader_deferred_vnt->SetProjViewWorld(camera->GetProjViewMatrix());
		shader_deferred_vnt->SetCamPos(camera->GetPosition());

		Resource::GLTextureResource mtexture_diff = mresource->texture_map->GetResourceByID(mMaterial.diffuse_map_id);
		Resource::GLTextureResource mtexture_spec = mresource->texture_map->GetResourceByID(mMaterial.specular_map_id);

		shader_deferred_vnt->SetDiffuseTex();
		glBindTexture(mtexture_diff.texture_target, mtexture_diff.texture);

		shader_deferred_vnt->SetSpecularTex();
		glBindTexture(mtexture_spec.texture_target, mtexture_spec.texture);

		shader_deferred_vnt->SetDiffuseColor(mMaterial.material.diffuse_color);
		shader_deferred_vnt->SetSpecularColor(mMaterial.material.specular_color);
		shader_deferred_vnt->SetEmissiveColor(mMaterial.material.emissive_color);
		shader_deferred_vnt->SetPBSData(glm::vec4(mMaterial.material.mMetallic, mMaterial.material.mRoughness, mMaterial.material.mF0, 1.0f));

		glDrawElementsBaseVertex(GL_TRIANGLES, VNT_object->mEntries[i].GetIndexCount(), GL_UNSIGNED_INT,
			(GLvoid *)(VNT_object->mEntries[i].GetIndexStart() * sizeof(uint32_t)), VNT_object->mEntries[i].GetVertexStart());
	}

	glUseProgram(shader_deferred_vnt->GetProgramDiffSpecRefl());
	InitBuffer(shader_deferred_vnt->GetProgramDiffSpecRefl());
	for (register uint32_t i = VNT_draw_list.c2; i < VNT_draw_list.c3; i++)
	{
		uint32_t matID = VNT_object->mEntries[i].GetMaterialID();

		MeshLoad::ObjectMaterial mMaterial = mresource->mMaterial[matID];

		shader_deferred_vnt->SetProjViewWorld(camera->GetProjViewMatrix());
		shader_deferred_vnt->SetCamPos(camera->GetPosition());

		Resource::GLTextureResource mtexture_diff = mresource->texture_map->GetResourceByID(mMaterial.diffuse_map_id);
		Resource::GLTextureResource mtexture_spec = mresource->texture_map->GetResourceByID(mMaterial.specular_map_id);
		Resource::GLTextureResource mtexture_reflect = mresource->texture_map->GetResourceByID(mMaterial.reflaction_map_id);

		shader_deferred_vnt->SetDiffuseTex();
		glBindTexture(mtexture_diff.texture_target, mtexture_diff.texture);

		shader_deferred_vnt->SetReflactionTex();
		glBindTexture(mtexture_reflect.texture_target, mtexture_reflect.texture);

		shader_deferred_vnt->SetSpecularTex();
		glBindTexture(mtexture_spec.texture_target, mtexture_spec.texture);

		shader_deferred_vnt->SetDiffuseColor(mMaterial.material.diffuse_color);
		shader_deferred_vnt->SetSpecularColor(mMaterial.material.specular_color);
		shader_deferred_vnt->SetEmissiveColor(mMaterial.material.emissive_color);
		shader_deferred_vnt->SetPBSData(glm::vec4(mMaterial.material.mMetallic, mMaterial.material.mRoughness, mMaterial.material.mF0, 1.0f));

		glDrawElementsBaseVertex(GL_TRIANGLES, VNT_object->mEntries[i].GetIndexCount(), GL_UNSIGNED_INT,
			(GLvoid *)(VNT_object->mEntries[i].GetIndexStart() * sizeof(uint32_t)), VNT_object->mEntries[i].GetVertexStart());
	}

}

void DeferredRendering::DeferredDrawVNT::DrawClipTex(void)
{
	glDisable(GL_CULL_FACE);
	//glFrontFace(GL_CCW);
	//glCullFace(GL_BACK);

	//[c3;c4)
	//[c4;c5)
	//[c5;c6)
	//[c6;c7)

	if (VNT_draw_list.c4 != VNT_draw_list.c3) 
	{
		MessageBox(NULL, L"Проверь загрузку материалов", L"ERROR", MB_OK | MB_ICONERROR);
	}

	glUseProgram(shader_deferred_vnt->GetProgramDiffuseClip());
	InitBuffer(shader_deferred_vnt->GetProgramDiffuseClip());
	for (register uint32_t i = VNT_draw_list.c4; i < VNT_draw_list.c5; i++)
	{
		uint32_t matID = VNT_object->mEntries[i].GetMaterialID();

		MeshLoad::ObjectMaterial mMaterial = mresource->mMaterial[matID];

		shader_deferred_vnt->SetProjViewWorld(camera->GetProjViewMatrix());
		shader_deferred_vnt->SetCamPos(camera->GetPosition());

		Resource::GLTextureResource mtexture_diff = mresource->texture_map->GetResourceByID(mMaterial.diffuse_map_id);

		shader_deferred_vnt->SetDiffuseTex();
		glBindTexture(mtexture_diff.texture_target, mtexture_diff.texture);

		shader_deferred_vnt->SetDiffuseColor(mMaterial.material.diffuse_color);
		shader_deferred_vnt->SetSpecularColor(mMaterial.material.specular_color);
		shader_deferred_vnt->SetEmissiveColor(mMaterial.material.emissive_color);
		shader_deferred_vnt->SetPBSData(glm::vec4(mMaterial.material.mMetallic, mMaterial.material.mRoughness, mMaterial.material.mF0, 1.0f));

		glDrawElementsBaseVertex(GL_TRIANGLES, VNT_object->mEntries[i].GetIndexCount(), GL_UNSIGNED_INT,
			(GLvoid *)(VNT_object->mEntries[i].GetIndexStart() * sizeof(uint32_t)), VNT_object->mEntries[i].GetVertexStart());
	}

	glUseProgram(shader_deferred_vnt->GetProgramDiffSpecClip());
	InitBuffer(shader_deferred_vnt->GetProgramDiffSpecClip());
	for (register uint32_t i = VNT_draw_list.c5; i < VNT_draw_list.c6; i++)
	{
		uint32_t matID = VNT_object->mEntries[i].GetMaterialID();

		MeshLoad::ObjectMaterial mMaterial = mresource->mMaterial[matID];

		shader_deferred_vnt->SetProjViewWorld(camera->GetProjViewMatrix());
		shader_deferred_vnt->SetCamPos(camera->GetPosition());

		Resource::GLTextureResource mtexture_diff = mresource->texture_map->GetResourceByID(mMaterial.diffuse_map_id);
		Resource::GLTextureResource mtexture_spec = mresource->texture_map->GetResourceByID(mMaterial.specular_map_id);

		shader_deferred_vnt->SetDiffuseTex();
		glBindTexture(mtexture_diff.texture_target, mtexture_diff.texture);

		shader_deferred_vnt->SetSpecularTex();
		glBindTexture(mtexture_spec.texture_target, mtexture_spec.texture);

		shader_deferred_vnt->SetDiffuseColor(mMaterial.material.diffuse_color);
		shader_deferred_vnt->SetSpecularColor(mMaterial.material.specular_color);
		shader_deferred_vnt->SetEmissiveColor(mMaterial.material.emissive_color);
		shader_deferred_vnt->SetPBSData(glm::vec4(mMaterial.material.mMetallic, mMaterial.material.mRoughness, mMaterial.material.mF0, 1.0f));

		glDrawElementsBaseVertex(GL_TRIANGLES, VNT_object->mEntries[i].GetIndexCount(), GL_UNSIGNED_INT,
			(GLvoid *)(VNT_object->mEntries[i].GetIndexStart() * sizeof(uint32_t)), VNT_object->mEntries[i].GetVertexStart());
	}

	glUseProgram(shader_deferred_vnt->GetProgramDiffSpecReflClip());
	InitBuffer(shader_deferred_vnt->GetProgramDiffSpecReflClip());
	for (register uint32_t i = VNT_draw_list.c6; i < VNT_draw_list.c7; i++)
	{
		uint32_t matID = VNT_object->mEntries[i].GetMaterialID();

		MeshLoad::ObjectMaterial mMaterial = mresource->mMaterial[matID];

		shader_deferred_vnt->SetProjViewWorld(camera->GetProjViewMatrix());
		shader_deferred_vnt->SetCamPos(camera->GetPosition());

		Resource::GLTextureResource mtexture_diff = mresource->texture_map->GetResourceByID(mMaterial.diffuse_map_id);
		Resource::GLTextureResource mtexture_spec = mresource->texture_map->GetResourceByID(mMaterial.specular_map_id);
		Resource::GLTextureResource mtexture_reflect = mresource->texture_map->GetResourceByID(mMaterial.reflaction_map_id);

		shader_deferred_vnt->SetDiffuseTex();
		glBindTexture(mtexture_diff.texture_target, mtexture_diff.texture);

		shader_deferred_vnt->SetReflactionTex();
		glBindTexture(mtexture_reflect.texture_target, mtexture_reflect.texture);

		shader_deferred_vnt->SetSpecularTex();
		glBindTexture(mtexture_spec.texture_target, mtexture_spec.texture);

		shader_deferred_vnt->SetDiffuseColor(mMaterial.material.diffuse_color);
		shader_deferred_vnt->SetSpecularColor(mMaterial.material.specular_color);
		shader_deferred_vnt->SetEmissiveColor(mMaterial.material.emissive_color);
		shader_deferred_vnt->SetPBSData(glm::vec4(mMaterial.material.mMetallic, mMaterial.material.mRoughness, mMaterial.material.mF0, 1.0f));

		glDrawElementsBaseVertex(GL_TRIANGLES, VNT_object->mEntries[i].GetIndexCount(), GL_UNSIGNED_INT,
			(GLvoid *)(VNT_object->mEntries[i].GetIndexStart() * sizeof(uint32_t)), VNT_object->mEntries[i].GetVertexStart());
	}

	glEnable(GL_CULL_FACE);
	glUseProgram(0);
}

void DeferredRendering::DeferredDrawVNT::DrawEarlyDepthTestShadow(const glm::mat4 *matrixes, uint32_t uCount, Shader::ShaderSpotLightShadow *shader)
{
	if (!glIsEnabled(GL_CULL_FACE)) glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
	glCullFace(GL_BACK);
	glDisable(GL_CULL_FACE);
	//[0;c0)
	//[c0;c1)
	//[c1;c2)
	//[c2;c3)

	glUseProgram(shader->GetProgram());
	InitBuffer(shader->GetProgram());

	shader->SetProjViewMatrix(matrixes, uCount);
	shader->SetShadowCount(uCount);

	for (register uint32_t i = 0; i < VNT_draw_list.c0; i++)
	{
		//shader->SetProjViewMatrix(matrixes, uCount);
		//shader->SetShadowCount(uCount);
		
		glDrawElementsBaseVertex(GL_TRIANGLES, VNT_object->mEntries[i].GetIndexCount(), GL_UNSIGNED_INT,
			(GLvoid *)(VNT_object->mEntries[i].GetIndexStart() * sizeof(uint32_t)), VNT_object->mEntries[i].GetVertexStart());
	}

	//glUseProgram(shader->GetProgram());
	//InitBuffer(shader->GetProgram());
	for (register uint32_t i = VNT_draw_list.c0; i < VNT_draw_list.c1; i++)
	{
		//shader->SetProjViewMatrix(matrixes, uCount);
		//shader->SetShadowCount(uCount);

		glDrawElementsBaseVertex(GL_TRIANGLES, VNT_object->mEntries[i].GetIndexCount(), GL_UNSIGNED_INT,
			(GLvoid *)(VNT_object->mEntries[i].GetIndexStart() * sizeof(uint32_t)), VNT_object->mEntries[i].GetVertexStart());
	}

	//glUseProgram(shader->GetProgram());
	//InitBuffer(shader->GetProgram());
	for (register uint32_t i = VNT_draw_list.c1; i < VNT_draw_list.c2; i++)
	{
		//shader->SetProjViewMatrix(matrixes, uCount);
		//shader->SetShadowCount(uCount);

		glDrawElementsBaseVertex(GL_TRIANGLES, VNT_object->mEntries[i].GetIndexCount(), GL_UNSIGNED_INT,
			(GLvoid *)(VNT_object->mEntries[i].GetIndexStart() * sizeof(uint32_t)), VNT_object->mEntries[i].GetVertexStart());
	}

	//glUseProgram(shader->GetProgram());
	//InitBuffer(shader->GetProgram());
	for (register uint32_t i = VNT_draw_list.c2; i < VNT_draw_list.c3; i++)
	{
		//shader->SetProjViewMatrix(matrixes, uCount);
		//shader->SetShadowCount(uCount);

		glDrawElementsBaseVertex(GL_TRIANGLES, VNT_object->mEntries[i].GetIndexCount(), GL_UNSIGNED_INT,
			(GLvoid *)(VNT_object->mEntries[i].GetIndexStart() * sizeof(uint32_t)), VNT_object->mEntries[i].GetVertexStart());
	}

}

void DeferredRendering::DeferredDrawVNT::DrawClipTexShadow(const glm::mat4 *matrixes, uint32_t uCount, Shader::ShaderSpotLightShadow *shader)
{
	glDisable(GL_CULL_FACE);
	//glFrontFace(GL_CCW);
	//glCullFace(GL_BACK);

	//[c3;c4)
	//[c4;c5)
	//[c5;c6)
	//[c6;c7)

	shader->BindTextureToUnit();

	if (VNT_draw_list.c4 != VNT_draw_list.c3)
	{
		MessageBox(NULL, L"Проверь загрузку материалов", L"ERROR", MB_OK | MB_ICONERROR);
	}

	glUseProgram(shader->GetProgramClip());
	InitBuffer(shader->GetProgramClip());

	shader->SetProjViewMatrix(matrixes, uCount);
	shader->SetShadowCount(uCount);

	for (register uint32_t i = VNT_draw_list.c4; i < VNT_draw_list.c5; i++)
	{
		uint32_t matID = VNT_object->mEntries[i].GetMaterialID();

		MeshLoad::ObjectMaterial mMaterial = mresource->mMaterial[matID];

		Resource::GLTextureResource mtexture_diff = mresource->texture_map->GetResourceByID(mMaterial.diffuse_map_id);

		shader->SetDiffuseTex();
		glBindTexture(mtexture_diff.texture_target, mtexture_diff.texture);

		glDrawElementsBaseVertex(GL_TRIANGLES, VNT_object->mEntries[i].GetIndexCount(), GL_UNSIGNED_INT,
			(GLvoid *)(VNT_object->mEntries[i].GetIndexStart() * sizeof(uint32_t)), VNT_object->mEntries[i].GetVertexStart());
	}

	//glUseProgram(shader->GetProgramClip());
	//InitBuffer(shader->GetProgramClip());
	for (register uint32_t i = VNT_draw_list.c5; i < VNT_draw_list.c6; i++)
	{
		uint32_t matID = VNT_object->mEntries[i].GetMaterialID();

		MeshLoad::ObjectMaterial mMaterial = mresource->mMaterial[matID];

		//shader->SetProjViewMatrix(matrixes, uCount);
		//shader->SetShadowCount(uCount);

		Resource::GLTextureResource mtexture_diff = mresource->texture_map->GetResourceByID(mMaterial.diffuse_map_id);

		shader->SetDiffuseTex();
		glBindTexture(mtexture_diff.texture_target, mtexture_diff.texture);

		glDrawElementsBaseVertex(GL_TRIANGLES, VNT_object->mEntries[i].GetIndexCount(), GL_UNSIGNED_INT,
			(GLvoid *)(VNT_object->mEntries[i].GetIndexStart() * sizeof(uint32_t)), VNT_object->mEntries[i].GetVertexStart());
	}

	//glUseProgram(shader->GetProgramClip());
	//InitBuffer(shader->GetProgramClip());
	for (register uint32_t i = VNT_draw_list.c6; i < VNT_draw_list.c7; i++)
	{
		uint32_t matID = VNT_object->mEntries[i].GetMaterialID();

		MeshLoad::ObjectMaterial mMaterial = mresource->mMaterial[matID];

		//shader->SetProjViewMatrix(matrixes, uCount);
		//shader->SetShadowCount(uCount);

		Resource::GLTextureResource mtexture_diff = mresource->texture_map->GetResourceByID(mMaterial.diffuse_map_id);

		shader->SetDiffuseTex();
		glBindTexture(mtexture_diff.texture_target, mtexture_diff.texture);

		glDrawElementsBaseVertex(GL_TRIANGLES, VNT_object->mEntries[i].GetIndexCount(), GL_UNSIGNED_INT,
			(GLvoid *)(VNT_object->mEntries[i].GetIndexStart() * sizeof(uint32_t)), VNT_object->mEntries[i].GetVertexStart());
	}

	glEnable(GL_CULL_FACE);
	glUseProgram(0);
}

void DeferredRendering::DeferredDrawVNT::DrawEarlyDepthTestShadow(const glm::mat4 *matrixes, uint32_t uCount, Shader::ShaderPointLightShadow *shader)
{
	if (!glIsEnabled(GL_CULL_FACE)) glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
	glCullFace(GL_BACK);
	glDisable(GL_CULL_FACE);
	//[0;c0)
	//[c0;c1)
	//[c1;c2)
	//[c2;c3)

	glUseProgram(shader->GetProgram());
	InitBuffer(shader->GetProgram());

	shader->SetProjViewMatrix(matrixes, uCount);
	//shader->SetShadowCount(uCount);

	for (register uint32_t i = 0; i < VNT_draw_list.c0; i++)
	{
		//shader->SetProjViewMatrix(matrixes, uCount);
		//shader->SetShadowCount(uCount);

		glDrawElementsInstancedBaseVertex(GL_TRIANGLES, VNT_object->mEntries[i].GetIndexCount(), GL_UNSIGNED_INT,
			(GLvoid *)(VNT_object->mEntries[i].GetIndexStart() * sizeof(uint32_t)), uCount, VNT_object->mEntries[i].GetVertexStart());
	}

	//glUseProgram(shader->GetProgram());
	//InitBuffer(shader->GetProgram());
	for (register uint32_t i = VNT_draw_list.c0; i < VNT_draw_list.c1; i++)
	{
		//shader->SetProjViewMatrix(matrixes, uCount);
		//shader->SetShadowCount(uCount);

		glDrawElementsInstancedBaseVertex(GL_TRIANGLES, VNT_object->mEntries[i].GetIndexCount(), GL_UNSIGNED_INT,
			(GLvoid *)(VNT_object->mEntries[i].GetIndexStart() * sizeof(uint32_t)), uCount, VNT_object->mEntries[i].GetVertexStart());
	}

	//glUseProgram(shader->GetProgram());
	//InitBuffer(shader->GetProgram());
	for (register uint32_t i = VNT_draw_list.c1; i < VNT_draw_list.c2; i++)
	{
		//shader->SetProjViewMatrix(matrixes, uCount);
		//shader->SetShadowCount(uCount);

		glDrawElementsInstancedBaseVertex(GL_TRIANGLES, VNT_object->mEntries[i].GetIndexCount(), GL_UNSIGNED_INT,
			(GLvoid *)(VNT_object->mEntries[i].GetIndexStart() * sizeof(uint32_t)), uCount, VNT_object->mEntries[i].GetVertexStart());
	}

	//glUseProgram(shader->GetProgram());
	//InitBuffer(shader->GetProgram());
	for (register uint32_t i = VNT_draw_list.c2; i < VNT_draw_list.c3; i++)
	{
		//shader->SetProjViewMatrix(matrixes, uCount);
		//shader->SetShadowCount(uCount);

		glDrawElementsInstancedBaseVertex(GL_TRIANGLES, VNT_object->mEntries[i].GetIndexCount(), GL_UNSIGNED_INT,
			(GLvoid *)(VNT_object->mEntries[i].GetIndexStart() * sizeof(uint32_t)), uCount, VNT_object->mEntries[i].GetVertexStart());
	}

}

void DeferredRendering::DeferredDrawVNT::DrawClipTexShadow(const glm::mat4 *matrixes, uint32_t uCount, Shader::ShaderPointLightShadow *shader)
{
	glDisable(GL_CULL_FACE);
	//glFrontFace(GL_CCW);
	//glCullFace(GL_BACK);

	//[c3;c4)
	//[c4;c5)
	//[c5;c6)
	//[c6;c7)

	shader->BindTextureToUnit();

	if (VNT_draw_list.c4 != VNT_draw_list.c3)
	{
		MessageBox(NULL, L"Проверь загрузку материалов", L"ERROR", MB_OK | MB_ICONERROR);
	}

	glUseProgram(shader->GetProgramClip());
	InitBuffer(shader->GetProgramClip());

	shader->SetProjViewMatrix(matrixes, uCount);
	//shader->SetShadowCount(uCount);

	for (register uint32_t i = VNT_draw_list.c4; i < VNT_draw_list.c5; i++)
	{
		uint32_t matID = VNT_object->mEntries[i].GetMaterialID();

		MeshLoad::ObjectMaterial mMaterial = mresource->mMaterial[matID];

		Resource::GLTextureResource mtexture_diff = mresource->texture_map->GetResourceByID(mMaterial.diffuse_map_id);

		shader->SetDiffuseTex();
		glBindTexture(mtexture_diff.texture_target, mtexture_diff.texture);

		glDrawElementsInstancedBaseVertex(GL_TRIANGLES, VNT_object->mEntries[i].GetIndexCount(), GL_UNSIGNED_INT,
			(GLvoid *)(VNT_object->mEntries[i].GetIndexStart() * sizeof(uint32_t)), uCount, VNT_object->mEntries[i].GetVertexStart());
	}

	//glUseProgram(shader->GetProgramClip());
	//InitBuffer(shader->GetProgramClip());
	for (register uint32_t i = VNT_draw_list.c5; i < VNT_draw_list.c6; i++)
	{
		uint32_t matID = VNT_object->mEntries[i].GetMaterialID();

		MeshLoad::ObjectMaterial mMaterial = mresource->mMaterial[matID];

		//shader->SetProjViewMatrix(matrixes, uCount);
		//shader->SetShadowCount(uCount);

		Resource::GLTextureResource mtexture_diff = mresource->texture_map->GetResourceByID(mMaterial.diffuse_map_id);

		shader->SetDiffuseTex();
		glBindTexture(mtexture_diff.texture_target, mtexture_diff.texture);

		glDrawElementsInstancedBaseVertex(GL_TRIANGLES, VNT_object->mEntries[i].GetIndexCount(), GL_UNSIGNED_INT,
			(GLvoid *)(VNT_object->mEntries[i].GetIndexStart() * sizeof(uint32_t)), uCount, VNT_object->mEntries[i].GetVertexStart());
	}

	//glUseProgram(shader->GetProgramClip());
	//InitBuffer(shader->GetProgramClip());
	for (register uint32_t i = VNT_draw_list.c6; i < VNT_draw_list.c7; i++)
	{
		uint32_t matID = VNT_object->mEntries[i].GetMaterialID();

		MeshLoad::ObjectMaterial mMaterial = mresource->mMaterial[matID];

		//shader->SetProjViewMatrix(matrixes, uCount);
		//shader->SetShadowCount(uCount);

		Resource::GLTextureResource mtexture_diff = mresource->texture_map->GetResourceByID(mMaterial.diffuse_map_id);

		shader->SetDiffuseTex();
		glBindTexture(mtexture_diff.texture_target, mtexture_diff.texture);

		glDrawElementsInstancedBaseVertex(GL_TRIANGLES, VNT_object->mEntries[i].GetIndexCount(), GL_UNSIGNED_INT,
			(GLvoid *)(VNT_object->mEntries[i].GetIndexStart() * sizeof(uint32_t)), uCount, VNT_object->mEntries[i].GetVertexStart());
	}

	glEnable(GL_CULL_FACE);
	glUseProgram(0);
}

void DeferredRendering::DeferredDrawVNT::Draw(void)
{
	if (!VNT_object->VAO || !VNT_object->IB || !VNT_object->VB) return;
	/////////////////////////////////////////////////////////////////////////////////////////////////
	DrawEarlyDepthTest();
	DrawClipTex();
	/////////////////////////////////////////////////////////////////////////////////////////////////
	glBindTexture(GL_TEXTURE_2D, 0);
	/////////////////////////////////////////////////////////////////////////////////////////////////
	glBindVertexArray(0);
}

void DeferredRendering::DeferredDrawVNT::DrawShadow(const glm::mat4 *matrixes, uint32_t uCount, Shader::ShaderSpotLightShadow *shader)
{
	if (!VNT_object->VAO || !VNT_object->IB || !VNT_object->VB) return;
	/////////////////////////////////////////////////////////////////////////////////////////////////
	DrawEarlyDepthTestShadow(matrixes, uCount, shader);
	DrawClipTexShadow(matrixes, uCount, shader);
	/////////////////////////////////////////////////////////////////////////////////////////////////
	glBindTexture(GL_TEXTURE_2D, 0);
	/////////////////////////////////////////////////////////////////////////////////////////////////
	glBindVertexArray(0);
}

void DeferredRendering::DeferredDrawVNT::DrawShadow(const glm::mat4 *matrixes, uint32_t uCount, Shader::ShaderPointLightShadow *shader)
{
	if (!VNT_object->VAO || !VNT_object->IB || !VNT_object->VB) return;
	/////////////////////////////////////////////////////////////////////////////////////////////////
	DrawEarlyDepthTestShadow(matrixes, uCount, shader);
	DrawClipTexShadow(matrixes, uCount, shader);
	/////////////////////////////////////////////////////////////////////////////////////////////////
	glBindTexture(GL_TEXTURE_2D, 0);
	/////////////////////////////////////////////////////////////////////////////////////////////////
	glBindVertexArray(0);
}