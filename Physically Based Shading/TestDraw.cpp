#include "TestDraw.h"


TEST::TestDraw::TestDraw(MEngine::MObjects *model_, MCamera::Camera *camera) : scene(model_), camera(camera)
{
	shader = new Shader::ShaderTestDraw();
}

TEST::TestDraw::~TestDraw()
{
	delete shader;
	scene->mEntries.clear();

	glDeleteVertexArrays(1, &scene->VAO);
	glDeleteBuffers(1, &scene->IB);
	glDeleteBuffers(1, &scene->VB);
}

void TEST::TestDraw::Draw(void)
{
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);

	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);

	glReadBuffer(GL_NONE);
	glDrawBuffer(GL_BACK);
	
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glBindVertexArray(scene->VAO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, scene->IB);

	GLint artib1 = glGetAttribLocation(shader->GetProgram(), "v_position");
	GLint artib2 = glGetAttribLocation(shader->GetProgram(), "v_normal");
	GLint artib3 = glGetAttribLocation(shader->GetProgram(), "v_tex");

	glBindBuffer(GL_ARRAY_BUFFER, scene->VB);
	glVertexAttribPointer(artib1, 3, GL_FLOAT, GL_FALSE, sizeof(vec3) + sizeof(vec3) + sizeof(vec2), NULL);
	glVertexAttribPointer(artib2, 3, GL_FLOAT, GL_FALSE, sizeof(vec3) + sizeof(vec3) + sizeof(vec2), (const void *)(sizeof(vec3)));
	glVertexAttribPointer(artib3, 2, GL_FLOAT, GL_FALSE, sizeof(vec3) + sizeof(vec3) + sizeof(vec2), (const void *)(sizeof(vec3) + sizeof(vec3)));

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, scene->IB);

	glEnableVertexAttribArray(artib1);
	glEnableVertexAttribArray(artib2);
	glEnableVertexAttribArray(artib3);

	glUseProgram(shader->GetProgram());

	shader->SetProjViewWorld(camera->GetProjViewMatrix());
	shader->SetCamPos(camera->GetPosition());

	static vec3 light_dir = vec3(0.0f, -1.0f, -1.0f);

	if (GetAsyncKeyState('J') & 0x8000) light_dir.x -= 0.1;
	if (GetAsyncKeyState('L') & 0x8000) light_dir.x += 0.1;
	if (GetAsyncKeyState('K') & 0x8000) light_dir.z -= 0.1;
	if (GetAsyncKeyState('I') & 0x8000) light_dir.z += 0.1;

	shader->SetLightDir(glm::normalize(light_dir));
	shader->SetLightAmbient(vec4(0.2f, 0.2f, 0.2f, 1.0f));
	shader->SetLightDiffuse(vec4(0.8f, 0.8f, 0.8f, 1.0f));
	shader->SetLightSpecular(vec4(0.6f, 0.6f, 0.6f, 1.0f));

	for (register uint32_t i = 0; i < scene->mEntries.size(); i++) 
	{
		uint32_t matID = scene->mEntries[i].GetMaterialID();

		/*MeshLoad::ObjectMaterial mMaterial = scene->mMaterial[matID];

		shader->SetMaterialDiffuse(mMaterial.material.diffuse_color);
		shader->SetMaterialSpecular(mMaterial.material.specular_color);
		shader->SetMaterialEmissive(mMaterial.material.emissive_color);

		if (!mMaterial.diffuse_map_id)continue;

		Resource::GLTextureResource mtexture = scene->texture_map->GetResourceByID(mMaterial.diffuse_map_id);

		//if (GL_TEXTURE_CUBE_MAP != mtexture.texture_target)continue;

		glBindTexture(mtexture.texture_target, mtexture.texture);
		shader->BindTextureDiffuse(mtexture.texture_target, mtexture.texture);*/

		glDrawElementsBaseVertex(GL_TRIANGLES, scene->mEntries[i].GetIndexCount(), GL_UNSIGNED_INT, 
			(GLvoid *)(scene->mEntries[i].GetIndexStart() * sizeof(uint32_t)), scene->mEntries[i].GetVertexStart());
	}

	glBindTexture(GL_TEXTURE_2D, 0);

	glDisableVertexAttribArray(artib1);
	glDisableVertexAttribArray(artib2);
	glDisableVertexAttribArray(artib3);
}