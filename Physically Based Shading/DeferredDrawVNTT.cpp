#include "DeferredDrawVNTT.h"


DeferredRendering::DeferredDrawVNTT::DeferredDrawVNTT(MEngine::MResource *mresource, MEngine::MObjects *VNTT_object, MEngine::MDrawList VNTT_draw_list, MCamera::Camera *camera) :
	camera(camera), mresource(mresource), VNTT_object(VNTT_object), VNTT_draw_list(VNTT_draw_list)
{
	shader_deferred_vntt = new Shader::ShaderDeferredVNTT();
	InitPrograms();
}

DeferredRendering::DeferredDrawVNTT::~DeferredDrawVNTT()
{
	delete shader_deferred_vntt;
	VNTT_object->mEntries.clear();

	if (VNTT_object->VAO)glDeleteVertexArrays(1, &VNTT_object->VAO);
	if (VNTT_object->IB)glDeleteBuffers(1, &VNTT_object->IB);
	if (VNTT_object->VB)glDeleteBuffers(1, &VNTT_object->VB);

	delete VNTT_object;
}

void DeferredRendering::DeferredDrawVNTT::InitBuffer(GLuint program)
{
	GLint artib1 = glGetAttribLocation(program, "v_position");
	GLint artib2 = glGetAttribLocation(program, "v_normal");
	GLint artib3 = glGetAttribLocation(program, "v_tangent");
	GLint artib4 = glGetAttribLocation(program, "v_tex");

	static const uint32_t size = sizeof(vec3) + sizeof(vec3) + sizeof(vec3) + sizeof(vec2);

	static const uint32_t size1 = sizeof(vec3);
	static const uint32_t size2 = size1 << 1;
	static const uint32_t size3 = size2 + sizeof(vec3);

	glBindVertexArray(VNTT_object->VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VNTT_object->VB);
	if (artib1 != (-1))glVertexAttribPointer(artib1, 3, GL_FLOAT, GL_FALSE, size, NULL);
	if (artib2 != (-1))glVertexAttribPointer(artib2, 3, GL_FLOAT, GL_FALSE, size, (const void *)(size1));
	if (artib3 != (-1))glVertexAttribPointer(artib3, 3, GL_FLOAT, GL_FALSE, size, (const void *)(size2));
	if (artib4 != (-1))glVertexAttribPointer(artib4, 2, GL_FLOAT, GL_FALSE, size, (const void *)(size3));

	if (artib1 != (-1))glEnableVertexAttribArray(artib1);
	//else glDisableVertexAttribArray(artib1);

	if (artib2 != (-1))glEnableVertexAttribArray(artib2);
	//else glDisableVertexAttribArray(artib2);

	if (artib3 != (-1))glEnableVertexAttribArray(artib3);
	//else glDisableVertexAttribArray(artib3);

	if (artib4 != (-1))glEnableVertexAttribArray(artib4);
	//else glDisableVertexAttribArray(artib4);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VNTT_object->IB);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glBindVertexArray(VNTT_object->VAO);
}

void DeferredRendering::DeferredDrawVNTT::InitPrograms(void)
{
	glUseProgram(shader_deferred_vntt->GetProgramDiffSpec());
	shader_deferred_vntt->BindTextureToUnit();

	glUseProgram(shader_deferred_vntt->GetProgramDiffSpecClip());
	shader_deferred_vntt->BindTextureToUnit();

	glUseProgram(shader_deferred_vntt->GetProgramDiffSpecRefl());
	shader_deferred_vntt->BindTextureToUnit();

	glUseProgram(shader_deferred_vntt->GetProgramDiffSpecReflClip());
	shader_deferred_vntt->BindTextureToUnit();

	glUseProgram(shader_deferred_vntt->GetProgramDiffuse());
	shader_deferred_vntt->BindTextureToUnit();

	glUseProgram(shader_deferred_vntt->GetProgramDiffuseClip());
	shader_deferred_vntt->BindTextureToUnit();

	shader_deferred_vntt->ResetProgram();

	glUseProgram(0);
}

void DeferredRendering::DeferredDrawVNTT::DrawEarlyDepthTest(void)
{
	//if (!glIsEnabled(GL_CULL_FACE)) glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
	glCullFace(GL_BACK);
	glDisable(GL_CULL_FACE);
	//[0;c0)
	//[c0;c1)
	//[c1;c2)

	glUseProgram(shader_deferred_vntt->GetProgramDiffuse());
	InitBuffer(shader_deferred_vntt->GetProgramDiffuse());
	for (register uint32_t i = 0; i < VNTT_draw_list.c0; i++)
	{
		uint32_t matID = VNTT_object->mEntries[i].GetMaterialID();

		MeshLoad::ObjectMaterial mMaterial = mresource->mMaterial[matID];

		shader_deferred_vntt->SetProjViewWorld(camera->GetProjViewMatrix());
		shader_deferred_vntt->SetCamPos(camera->GetPosition());

		Resource::GLTextureResource mtexture_diff = mresource->texture_map->GetResourceByID(mMaterial.diffuse_map_id);
		Resource::GLTextureResource mtexture_norm = mresource->texture_map->GetResourceByID(mMaterial.bump_map_id);

		shader_deferred_vntt->SetDiffuseTex();
		glBindTexture(mtexture_diff.texture_target, mtexture_diff.texture);

		shader_deferred_vntt->SetNormalTex();
		glBindTexture(mtexture_norm.texture_target, mtexture_norm.texture);

		shader_deferred_vntt->SetDiffuseColor(mMaterial.material.diffuse_color);
		shader_deferred_vntt->SetSpecularColor(mMaterial.material.specular_color);
		shader_deferred_vntt->SetEmissiveColor(mMaterial.material.emissive_color);
		shader_deferred_vntt->SetPBSData(glm::vec4(mMaterial.material.mMetallic, mMaterial.material.mRoughness, mMaterial.material.mF0, 1.0f));

		glDrawElementsBaseVertex(GL_TRIANGLES, VNTT_object->mEntries[i].GetIndexCount(), GL_UNSIGNED_INT,
			(GLvoid *)(VNTT_object->mEntries[i].GetIndexStart() * sizeof(uint32_t)), VNTT_object->mEntries[i].GetVertexStart());
	}

	glUseProgram(shader_deferred_vntt->GetProgramDiffSpec());
	InitBuffer(shader_deferred_vntt->GetProgramDiffSpec());
	for (register uint32_t i = VNTT_draw_list.c0; i < VNTT_draw_list.c1; i++)
	{
		uint32_t matID = VNTT_object->mEntries[i].GetMaterialID();

		MeshLoad::ObjectMaterial mMaterial = mresource->mMaterial[matID];

		shader_deferred_vntt->SetProjViewWorld(camera->GetProjViewMatrix());
		shader_deferred_vntt->SetCamPos(camera->GetPosition());

		Resource::GLTextureResource mtexture_diff = mresource->texture_map->GetResourceByID(mMaterial.diffuse_map_id);
		Resource::GLTextureResource mtexture_norm = mresource->texture_map->GetResourceByID(mMaterial.bump_map_id);
		Resource::GLTextureResource mtexture_spec = mresource->texture_map->GetResourceByID(mMaterial.specular_map_id);

		shader_deferred_vntt->SetDiffuseTex();
		glBindTexture(mtexture_diff.texture_target, mtexture_diff.texture);

		shader_deferred_vntt->SetNormalTex();
		glBindTexture(mtexture_norm.texture_target, mtexture_norm.texture);

		shader_deferred_vntt->SetSpecularTex();
		glBindTexture(mtexture_spec.texture_target, mtexture_spec.texture);

		shader_deferred_vntt->SetDiffuseColor(mMaterial.material.diffuse_color);
		shader_deferred_vntt->SetSpecularColor(mMaterial.material.specular_color);
		shader_deferred_vntt->SetEmissiveColor(mMaterial.material.emissive_color);
		shader_deferred_vntt->SetPBSData(glm::vec4(mMaterial.material.mMetallic, mMaterial.material.mRoughness, mMaterial.material.mF0, 1.0f));

		glDrawElementsBaseVertex(GL_TRIANGLES, VNTT_object->mEntries[i].GetIndexCount(), GL_UNSIGNED_INT,
			(GLvoid *)(VNTT_object->mEntries[i].GetIndexStart() * sizeof(uint32_t)), VNTT_object->mEntries[i].GetVertexStart());
	}

	glUseProgram(shader_deferred_vntt->GetProgramDiffSpecRefl());
	InitBuffer(shader_deferred_vntt->GetProgramDiffSpecRefl());
	for (register uint32_t i = VNTT_draw_list.c1; i < VNTT_draw_list.c2; i++)
	{
		uint32_t matID = VNTT_object->mEntries[i].GetMaterialID();

		MeshLoad::ObjectMaterial mMaterial = mresource->mMaterial[matID];

		shader_deferred_vntt->SetProjViewWorld(camera->GetProjViewMatrix());
		shader_deferred_vntt->SetCamPos(camera->GetPosition());

		Resource::GLTextureResource mtexture_diff = mresource->texture_map->GetResourceByID(mMaterial.diffuse_map_id);
		Resource::GLTextureResource mtexture_norm = mresource->texture_map->GetResourceByID(mMaterial.bump_map_id);
		Resource::GLTextureResource mtexture_spec = mresource->texture_map->GetResourceByID(mMaterial.specular_map_id);
		Resource::GLTextureResource mtexture_reflect = mresource->texture_map->GetResourceByID(mMaterial.reflaction_map_id);

		shader_deferred_vntt->SetDiffuseTex();
		glBindTexture(mtexture_diff.texture_target, mtexture_diff.texture);

		shader_deferred_vntt->SetNormalTex();
		glBindTexture(mtexture_norm.texture_target, mtexture_norm.texture);

		shader_deferred_vntt->SetReflactionTex();
		glBindTexture(mtexture_reflect.texture_target, mtexture_reflect.texture);

		shader_deferred_vntt->SetSpecularTex();
		glBindTexture(mtexture_spec.texture_target, mtexture_spec.texture);

		shader_deferred_vntt->SetDiffuseColor(mMaterial.material.diffuse_color);
		shader_deferred_vntt->SetSpecularColor(mMaterial.material.specular_color);
		shader_deferred_vntt->SetEmissiveColor(mMaterial.material.emissive_color);
		shader_deferred_vntt->SetPBSData(glm::vec4(mMaterial.material.mMetallic, mMaterial.material.mRoughness, mMaterial.material.mF0, 1.0f));

		glDrawElementsBaseVertex(GL_TRIANGLES, VNTT_object->mEntries[i].GetIndexCount(), GL_UNSIGNED_INT,
			(GLvoid *)(VNTT_object->mEntries[i].GetIndexStart() * sizeof(uint32_t)), VNTT_object->mEntries[i].GetVertexStart());
	}

}

void DeferredRendering::DeferredDrawVNTT::DrawClipTex(void)
{
	glDisable(GL_CULL_FACE);
	//glFrontFace(GL_CCW);
	//glCullFace(GL_BACK);

	//[c2;c3)
	//[c3;c4)
	//[c4;c5)


	glUseProgram(shader_deferred_vntt->GetProgramDiffuseClip());
	InitBuffer(shader_deferred_vntt->GetProgramDiffuseClip());
	for (register uint32_t i = VNTT_draw_list.c2; i < VNTT_draw_list.c3; i++)
	{
		uint32_t matID = VNTT_object->mEntries[i].GetMaterialID();

		MeshLoad::ObjectMaterial mMaterial = mresource->mMaterial[matID];

		shader_deferred_vntt->SetProjViewWorld(camera->GetProjViewMatrix());
		shader_deferred_vntt->SetCamPos(camera->GetPosition());

		Resource::GLTextureResource mtexture_diff = mresource->texture_map->GetResourceByID(mMaterial.diffuse_map_id);
		Resource::GLTextureResource mtexture_norm = mresource->texture_map->GetResourceByID(mMaterial.bump_map_id);

		shader_deferred_vntt->SetDiffuseTex();
		glBindTexture(mtexture_diff.texture_target, mtexture_diff.texture);

		shader_deferred_vntt->SetNormalTex();
		glBindTexture(mtexture_norm.texture_target, mtexture_norm.texture);

		shader_deferred_vntt->SetDiffuseColor(mMaterial.material.diffuse_color);
		shader_deferred_vntt->SetSpecularColor(mMaterial.material.specular_color);
		shader_deferred_vntt->SetEmissiveColor(mMaterial.material.emissive_color);
		shader_deferred_vntt->SetPBSData(glm::vec4(mMaterial.material.mMetallic, mMaterial.material.mRoughness, mMaterial.material.mF0, 1.0f));

		glDrawElementsBaseVertex(GL_TRIANGLES, VNTT_object->mEntries[i].GetIndexCount(), GL_UNSIGNED_INT,
			(GLvoid *)(VNTT_object->mEntries[i].GetIndexStart() * sizeof(uint32_t)), VNTT_object->mEntries[i].GetVertexStart());
	}

	glUseProgram(shader_deferred_vntt->GetProgramDiffSpecClip());
	InitBuffer(shader_deferred_vntt->GetProgramDiffSpecClip());
	for (register uint32_t i = VNTT_draw_list.c3; i < VNTT_draw_list.c4; i++)
	{
		uint32_t matID = VNTT_object->mEntries[i].GetMaterialID();

		MeshLoad::ObjectMaterial mMaterial = mresource->mMaterial[matID];

		shader_deferred_vntt->SetProjViewWorld(camera->GetProjViewMatrix());
		shader_deferred_vntt->SetCamPos(camera->GetPosition());

		Resource::GLTextureResource mtexture_diff = mresource->texture_map->GetResourceByID(mMaterial.diffuse_map_id);
		Resource::GLTextureResource mtexture_norm = mresource->texture_map->GetResourceByID(mMaterial.bump_map_id);
		Resource::GLTextureResource mtexture_spec = mresource->texture_map->GetResourceByID(mMaterial.specular_map_id);

		shader_deferred_vntt->SetDiffuseTex();
		glBindTexture(mtexture_diff.texture_target, mtexture_diff.texture);

		shader_deferred_vntt->SetNormalTex();
		glBindTexture(mtexture_norm.texture_target, mtexture_norm.texture);

		shader_deferred_vntt->SetSpecularTex();
		glBindTexture(mtexture_spec.texture_target, mtexture_spec.texture);

		shader_deferred_vntt->SetDiffuseColor(mMaterial.material.diffuse_color);
		shader_deferred_vntt->SetSpecularColor(mMaterial.material.specular_color);
		shader_deferred_vntt->SetEmissiveColor(mMaterial.material.emissive_color);
		shader_deferred_vntt->SetPBSData(glm::vec4(mMaterial.material.mMetallic, mMaterial.material.mRoughness, mMaterial.material.mF0, 1.0f));

		glDrawElementsBaseVertex(GL_TRIANGLES, VNTT_object->mEntries[i].GetIndexCount(), GL_UNSIGNED_INT,
			(GLvoid *)(VNTT_object->mEntries[i].GetIndexStart() * sizeof(uint32_t)), VNTT_object->mEntries[i].GetVertexStart());
	}

	glUseProgram(shader_deferred_vntt->GetProgramDiffSpecReflClip());
	InitBuffer(shader_deferred_vntt->GetProgramDiffSpecReflClip());
	for (register uint32_t i = VNTT_draw_list.c4; i < VNTT_draw_list.c5; i++)
	{
		uint32_t matID = VNTT_object->mEntries[i].GetMaterialID();

		MeshLoad::ObjectMaterial mMaterial = mresource->mMaterial[matID];

		shader_deferred_vntt->SetProjViewWorld(camera->GetProjViewMatrix());
		shader_deferred_vntt->SetCamPos(camera->GetPosition());

		Resource::GLTextureResource mtexture_diff = mresource->texture_map->GetResourceByID(mMaterial.diffuse_map_id);
		Resource::GLTextureResource mtexture_norm = mresource->texture_map->GetResourceByID(mMaterial.bump_map_id);
		Resource::GLTextureResource mtexture_spec = mresource->texture_map->GetResourceByID(mMaterial.specular_map_id);
		Resource::GLTextureResource mtexture_reflect = mresource->texture_map->GetResourceByID(mMaterial.reflaction_map_id);

		shader_deferred_vntt->SetDiffuseTex();
		glBindTexture(mtexture_diff.texture_target, mtexture_diff.texture);

		shader_deferred_vntt->SetNormalTex();
		glBindTexture(mtexture_norm.texture_target, mtexture_norm.texture);

		shader_deferred_vntt->SetReflactionTex();
		glBindTexture(mtexture_reflect.texture_target, mtexture_reflect.texture);

		shader_deferred_vntt->SetSpecularTex();
		glBindTexture(mtexture_spec.texture_target, mtexture_spec.texture);

		shader_deferred_vntt->SetDiffuseColor(mMaterial.material.diffuse_color);
		shader_deferred_vntt->SetSpecularColor(mMaterial.material.specular_color);
		shader_deferred_vntt->SetEmissiveColor(mMaterial.material.emissive_color);
		shader_deferred_vntt->SetPBSData(glm::vec4(mMaterial.material.mMetallic, mMaterial.material.mRoughness, mMaterial.material.mF0, 1.0f));

		glDrawElementsBaseVertex(GL_TRIANGLES, VNTT_object->mEntries[i].GetIndexCount(), GL_UNSIGNED_INT,
			(GLvoid *)(VNTT_object->mEntries[i].GetIndexStart() * sizeof(uint32_t)), VNTT_object->mEntries[i].GetVertexStart());
	}

	glEnable(GL_CULL_FACE);
	glUseProgram(0);
}

void DeferredRendering::DeferredDrawVNTT::Draw(void)
{
	if (!VNTT_object->VAO || !VNTT_object->IB || !VNTT_object->VB) return;
	/////////////////////////////////////////////////////////////////////////////////////////////////
	DrawEarlyDepthTest();
	DrawClipTex();
	/////////////////////////////////////////////////////////////////////////////////////////////////
	glBindTexture(GL_TEXTURE_2D, 0);
	/////////////////////////////////////////////////////////////////////////////////////////////////
	glBindVertexArray(0);
}

void DeferredRendering::DeferredDrawVNTT::DrawEarlyDepthTestShadow(const glm::mat4 *matrixes, uint32_t uCount, Shader::ShaderSpotLightShadow *shader)
{
	//if (!glIsEnabled(GL_CULL_FACE)) glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
	glCullFace(GL_BACK);
	glDisable(GL_CULL_FACE);
	//[0;c0)
	//[c0;c1)
	//[c1;c2)

	glUseProgram(shader->GetProgram());
	InitBuffer(shader->GetProgram());

	shader->SetProjViewMatrix(matrixes, uCount);
	shader->SetShadowCount(uCount);

	for (register uint32_t i = 0; i < VNTT_draw_list.c0; i++)
	{
		//shader->SetProjViewMatrix(matrixes, uCount);
		//shader->SetShadowCount(uCount);

		glDrawElementsBaseVertex(GL_TRIANGLES, VNTT_object->mEntries[i].GetIndexCount(), GL_UNSIGNED_INT,
			(GLvoid *)(VNTT_object->mEntries[i].GetIndexStart() * sizeof(uint32_t)), VNTT_object->mEntries[i].GetVertexStart());
	}

	//glUseProgram(shader->GetProgram());
	//InitBuffer(shader->GetProgram());
	for (register uint32_t i = VNTT_draw_list.c0; i < VNTT_draw_list.c1; i++)
	{
		//shader->SetProjViewMatrix(matrixes, uCount);
		//shader->SetShadowCount(uCount);

		glDrawElementsBaseVertex(GL_TRIANGLES, VNTT_object->mEntries[i].GetIndexCount(), GL_UNSIGNED_INT,
			(GLvoid *)(VNTT_object->mEntries[i].GetIndexStart() * sizeof(uint32_t)), VNTT_object->mEntries[i].GetVertexStart());
	}

	//glUseProgram(shader->GetProgram());
	//InitBuffer(shader->GetProgram());
	for (register uint32_t i = VNTT_draw_list.c1; i < VNTT_draw_list.c2; i++)
	{
		//shader->SetProjViewMatrix(matrixes, uCount);
		//shader->SetShadowCount(uCount);

		glDrawElementsBaseVertex(GL_TRIANGLES, VNTT_object->mEntries[i].GetIndexCount(), GL_UNSIGNED_INT,
			(GLvoid *)(VNTT_object->mEntries[i].GetIndexStart() * sizeof(uint32_t)), VNTT_object->mEntries[i].GetVertexStart());
	}
}

void DeferredRendering::DeferredDrawVNTT::DrawClipTexShadow(const glm::mat4 *matrixes, uint32_t uCount, Shader::ShaderSpotLightShadow *shader)
{
	glDisable(GL_CULL_FACE);
	//glFrontFace(GL_CCW);
	//glCullFace(GL_BACK);

	//[c2;c3)
	//[c3;c4)
	//[c4;c5)

	shader->BindTextureToUnit();

	glUseProgram(shader->GetProgramClip());
	InitBuffer(shader->GetProgramClip());

	shader->SetProjViewMatrix(matrixes, uCount);
	shader->SetShadowCount(uCount);

	for (register uint32_t i = VNTT_draw_list.c2; i < VNTT_draw_list.c3; i++)
	{
		uint32_t matID = VNTT_object->mEntries[i].GetMaterialID();

		MeshLoad::ObjectMaterial mMaterial = mresource->mMaterial[matID];

		Resource::GLTextureResource mtexture_diff = mresource->texture_map->GetResourceByID(mMaterial.diffuse_map_id);

		shader->SetDiffuseTex();
		glBindTexture(mtexture_diff.texture_target, mtexture_diff.texture);

		glDrawElementsBaseVertex(GL_TRIANGLES, VNTT_object->mEntries[i].GetIndexCount(), GL_UNSIGNED_INT,
			(GLvoid *)(VNTT_object->mEntries[i].GetIndexStart() * sizeof(uint32_t)), VNTT_object->mEntries[i].GetVertexStart());
	}

	//glUseProgram(shader->GetProgramClip());
	//InitBuffer(shader->GetProgramClip());
	for (register uint32_t i = VNTT_draw_list.c3; i < VNTT_draw_list.c4; i++)
	{
		uint32_t matID = VNTT_object->mEntries[i].GetMaterialID();

		MeshLoad::ObjectMaterial mMaterial = mresource->mMaterial[matID];

		//shader->SetProjViewMatrix(matrixes, uCount);
		//shader->SetShadowCount(uCount);

		Resource::GLTextureResource mtexture_diff = mresource->texture_map->GetResourceByID(mMaterial.diffuse_map_id);

		shader->SetDiffuseTex();
		glBindTexture(mtexture_diff.texture_target, mtexture_diff.texture);

		glDrawElementsBaseVertex(GL_TRIANGLES, VNTT_object->mEntries[i].GetIndexCount(), GL_UNSIGNED_INT,
			(GLvoid *)(VNTT_object->mEntries[i].GetIndexStart() * sizeof(uint32_t)), VNTT_object->mEntries[i].GetVertexStart());
	}

	//glUseProgram(shader->GetProgramClip());
	//InitBuffer(shader->GetProgramClip());
	for (register uint32_t i = VNTT_draw_list.c4; i < VNTT_draw_list.c5; i++)
	{
		uint32_t matID = VNTT_object->mEntries[i].GetMaterialID();

		MeshLoad::ObjectMaterial mMaterial = mresource->mMaterial[matID];

		//shader->SetProjViewMatrix(matrixes, uCount);
		//shader->SetShadowCount(uCount);

		Resource::GLTextureResource mtexture_diff = mresource->texture_map->GetResourceByID(mMaterial.diffuse_map_id);

		shader->SetDiffuseTex();
		glBindTexture(mtexture_diff.texture_target, mtexture_diff.texture);

		glDrawElementsBaseVertex(GL_TRIANGLES, VNTT_object->mEntries[i].GetIndexCount(), GL_UNSIGNED_INT,
			(GLvoid *)(VNTT_object->mEntries[i].GetIndexStart() * sizeof(uint32_t)), VNTT_object->mEntries[i].GetVertexStart());
	}

	glEnable(GL_CULL_FACE);
	glUseProgram(0);
}

void DeferredRendering::DeferredDrawVNTT::DrawEarlyDepthTestShadow(const glm::mat4 *matrixes, uint32_t uCount, Shader::ShaderPointLightShadow *shader)
{
	//if (!glIsEnabled(GL_CULL_FACE)) glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
	glCullFace(GL_BACK);
	glDisable(GL_CULL_FACE);
	//[0;c0)
	//[c0;c1)
	//[c1;c2)

	glUseProgram(shader->GetProgram());
	InitBuffer(shader->GetProgram());

	shader->SetProjViewMatrix(matrixes, uCount);
	//shader->SetShadowCount(uCount);

	for (register uint32_t i = 0; i < VNTT_draw_list.c0; i++)
	{
		//shader->SetProjViewMatrix(matrixes, uCount);
		//shader->SetShadowCount(uCount);

		glDrawElementsInstancedBaseVertex(GL_TRIANGLES, VNTT_object->mEntries[i].GetIndexCount(), GL_UNSIGNED_INT,
			(GLvoid *)(VNTT_object->mEntries[i].GetIndexStart() * sizeof(uint32_t)), uCount, VNTT_object->mEntries[i].GetVertexStart());
	}

	//glUseProgram(shader->GetProgram());
	//InitBuffer(shader->GetProgram());
	for (register uint32_t i = VNTT_draw_list.c0; i < VNTT_draw_list.c1; i++)
	{
		//shader->SetProjViewMatrix(matrixes, uCount);
		//shader->SetShadowCount(uCount);

		glDrawElementsInstancedBaseVertex(GL_TRIANGLES, VNTT_object->mEntries[i].GetIndexCount(), GL_UNSIGNED_INT,
			(GLvoid *)(VNTT_object->mEntries[i].GetIndexStart() * sizeof(uint32_t)), uCount, VNTT_object->mEntries[i].GetVertexStart());
	}

	//glUseProgram(shader->GetProgram());
	//InitBuffer(shader->GetProgram());
	for (register uint32_t i = VNTT_draw_list.c1; i < VNTT_draw_list.c2; i++)
	{
		//shader->SetProjViewMatrix(matrixes, uCount);
		//shader->SetShadowCount(uCount);

		glDrawElementsInstancedBaseVertex(GL_TRIANGLES, VNTT_object->mEntries[i].GetIndexCount(), GL_UNSIGNED_INT,
			(GLvoid *)(VNTT_object->mEntries[i].GetIndexStart() * sizeof(uint32_t)), uCount, VNTT_object->mEntries[i].GetVertexStart());
	}
}

void DeferredRendering::DeferredDrawVNTT::DrawClipTexShadow(const glm::mat4 *matrixes, uint32_t uCount, Shader::ShaderPointLightShadow *shader)
{
	glDisable(GL_CULL_FACE);
	//glFrontFace(GL_CCW);
	//glCullFace(GL_BACK);

	//[c2;c3)
	//[c3;c4)
	//[c4;c5)

	shader->BindTextureToUnit();

	glUseProgram(shader->GetProgramClip());
	InitBuffer(shader->GetProgramClip());

	shader->SetProjViewMatrix(matrixes, uCount);
	//shader->SetShadowCount(uCount);

	for (register uint32_t i = VNTT_draw_list.c2; i < VNTT_draw_list.c3; i++)
	{
		uint32_t matID = VNTT_object->mEntries[i].GetMaterialID();

		MeshLoad::ObjectMaterial mMaterial = mresource->mMaterial[matID];

		Resource::GLTextureResource mtexture_diff = mresource->texture_map->GetResourceByID(mMaterial.diffuse_map_id);

		shader->SetDiffuseTex();
		glBindTexture(mtexture_diff.texture_target, mtexture_diff.texture);

		glDrawElementsInstancedBaseVertex(GL_TRIANGLES, VNTT_object->mEntries[i].GetIndexCount(), GL_UNSIGNED_INT,
			(GLvoid *)(VNTT_object->mEntries[i].GetIndexStart() * sizeof(uint32_t)), uCount, VNTT_object->mEntries[i].GetVertexStart());
	}

	//glUseProgram(shader->GetProgramClip());
	//InitBuffer(shader->GetProgramClip());
	for (register uint32_t i = VNTT_draw_list.c3; i < VNTT_draw_list.c4; i++)
	{
		uint32_t matID = VNTT_object->mEntries[i].GetMaterialID();

		MeshLoad::ObjectMaterial mMaterial = mresource->mMaterial[matID];

		//shader->SetProjViewMatrix(matrixes, uCount);
		//shader->SetShadowCount(uCount);

		Resource::GLTextureResource mtexture_diff = mresource->texture_map->GetResourceByID(mMaterial.diffuse_map_id);

		shader->SetDiffuseTex();
		glBindTexture(mtexture_diff.texture_target, mtexture_diff.texture);

		glDrawElementsInstancedBaseVertex(GL_TRIANGLES, VNTT_object->mEntries[i].GetIndexCount(), GL_UNSIGNED_INT,
			(GLvoid *)(VNTT_object->mEntries[i].GetIndexStart() * sizeof(uint32_t)), uCount, VNTT_object->mEntries[i].GetVertexStart());
	}

	//glUseProgram(shader->GetProgramClip());
	//InitBuffer(shader->GetProgramClip());
	for (register uint32_t i = VNTT_draw_list.c4; i < VNTT_draw_list.c5; i++)
	{
		uint32_t matID = VNTT_object->mEntries[i].GetMaterialID();

		MeshLoad::ObjectMaterial mMaterial = mresource->mMaterial[matID];

		//shader->SetProjViewMatrix(matrixes, uCount);
		//shader->SetShadowCount(uCount);

		Resource::GLTextureResource mtexture_diff = mresource->texture_map->GetResourceByID(mMaterial.diffuse_map_id);

		shader->SetDiffuseTex();
		glBindTexture(mtexture_diff.texture_target, mtexture_diff.texture);

		glDrawElementsInstancedBaseVertex(GL_TRIANGLES, VNTT_object->mEntries[i].GetIndexCount(), GL_UNSIGNED_INT,
			(GLvoid *)(VNTT_object->mEntries[i].GetIndexStart() * sizeof(uint32_t)), uCount, VNTT_object->mEntries[i].GetVertexStart());
	}

	glEnable(GL_CULL_FACE);
	glUseProgram(0);
}

void DeferredRendering::DeferredDrawVNTT::DrawShadow(const glm::mat4 *matrixes, uint32_t uCount, Shader::ShaderSpotLightShadow *shader)
{
	if (!VNTT_object->VAO || !VNTT_object->IB || !VNTT_object->VB) return;
	/////////////////////////////////////////////////////////////////////////////////////////////////
	DrawEarlyDepthTestShadow(matrixes, uCount, shader);
	DrawClipTexShadow(matrixes, uCount, shader);
	/////////////////////////////////////////////////////////////////////////////////////////////////
	glBindTexture(GL_TEXTURE_2D, 0);
	/////////////////////////////////////////////////////////////////////////////////////////////////
	glBindVertexArray(0);
}

void DeferredRendering::DeferredDrawVNTT::DrawShadow(const glm::mat4 *matrixes, uint32_t uCount, Shader::ShaderPointLightShadow *shader)
{
	if (!VNTT_object->VAO || !VNTT_object->IB || !VNTT_object->VB) return;
	/////////////////////////////////////////////////////////////////////////////////////////////////
	DrawEarlyDepthTestShadow(matrixes, uCount, shader);
	DrawClipTexShadow(matrixes, uCount, shader);
	/////////////////////////////////////////////////////////////////////////////////////////////////
	glBindTexture(GL_TEXTURE_2D, 0);
	/////////////////////////////////////////////////////////////////////////////////////////////////
	glBindVertexArray(0);
}