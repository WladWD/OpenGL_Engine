#include "DrawSSR.h"


Draw::DrawSSR::DrawSSR(FullScreenQuad * mFullScreenQuad, MCamera::Camera * mCamera) : mCamera(mCamera), mFullScreenQuad(mFullScreenQuad)
{
	Init();
}

Draw::DrawSSR::~DrawSSR()
{
	delete mShaderSSR;
}

void Draw::DrawSSR::Init(void)
{
	mShaderSSR = new Shader::ShaderSSR();
}

void Draw::DrawSSR::InitAttrib(void)
{
	glUseProgram(mShaderSSR->GetProgram());

	//GLint sAttribLocationPosition = mShaderBlur->GetPositionAttrib();
	GLint sAttribLocationTex = mShaderSSR->GetTexCoordAttribLocation();

	glBindVertexArray(mFullScreenQuad->GetVertexArray());
	glBindBuffer(GL_ARRAY_BUFFER, mFullScreenQuad->GetArrayBuffer());
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mFullScreenQuad->GetElementArrayBuffer());
	int32_t stride = sizeof(glm::vec2);//Mesh::VTMesh);
	int32_t offset = 0;

	//for (register uint32_t i = 0; i < 8; i++) glDisableVertexAttribArray(i);

	/*if (sAttribLocationPosition != (-1))
	{
	glVertexAttribPointer(sAttribLocationPosition, 3, GL_FLOAT, GL_FALSE, stride, (void *)(offset));
	glEnableVertexAttribArray(sAttribLocationPosition);
	}

	offset += sizeof(glm::vec3);*/
	if (sAttribLocationTex != (-1))
	{
		glVertexAttribPointer(sAttribLocationTex, 2, GL_FLOAT, GL_FALSE, stride, (void *)(offset));
		glEnableVertexAttribArray(sAttribLocationTex);
		GLenum mErr = glGetError();
		mErr = glGetError();
		mErr = glGetError();
	}
	//GLenum mErr = glGetError();
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	//glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	mShaderSSR->BindTextureToUnit();

	glUseProgram(0);
	//mShaderBlur->ResetProgram();
}

void Draw::DrawSSR::DrawSSRData(GLuint gSourceDepth, GLuint gSourceImage, GLuint gDepth, GLuint gNormal, GLuint gDiffuse, GLuint gSpecular, glm::mat4 gProjView_Source)
{
	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);
	glDepthMask(GL_FALSE);
	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	InitAttrib();
	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	glBindVertexArray(mFullScreenQuad->GetVertexArray());
	//glBindBuffer(GL_ARRAY_BUFFER, mFullScreenQuad->GetArrayBuffer());
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mFullScreenQuad->GetElementArrayBuffer());
	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	glUseProgram(mShaderSSR->GetProgram());

	mShaderSSR->SetCamDir(mCamera->GetCamDir());
	mShaderSSR->SetCamPos(mCamera->GetPosition());

	mShaderSSR->SetDepthLayer();
	glBindTexture(GL_TEXTURE_2D, gDepth);
	mShaderSSR->SetDiffuseLayer();
	glBindTexture(GL_TEXTURE_2D, gDiffuse);
	mShaderSSR->SetNormalLayer();
	glBindTexture(GL_TEXTURE_2D, gNormal);
	mShaderSSR->SetSpecularLayer();
	glBindTexture(GL_TEXTURE_2D, gSpecular);

	mShaderSSR->SetSourceDepth();
	glBindTexture(GL_TEXTURE_2D, gSourceDepth);
	mShaderSSR->SetSourceImage();
	glBindTexture(GL_TEXTURE_2D, gSourceImage);

	mShaderSSR->SetProjViewSource(gProjView_Source);
	mShaderSSR->SetProjViewInvSource( glm::inverse(gProjView_Source) );

	mShaderSSR->SetProjView(mCamera->GetProjViewMatrix());
	mShaderSSR->SetProjViewInv( glm::inverse(mCamera->GetProjViewMatrix()) );

	GLenum mErr = glGetError();

	glDrawElementsBaseVertex(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0, 0);
	//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0);
	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	glUseProgram(0);
	glBindVertexArray(0);
	//glBindBuffer(GL_ARRAY_BUFFER, 0);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glDepthMask(GL_TRUE);
}
