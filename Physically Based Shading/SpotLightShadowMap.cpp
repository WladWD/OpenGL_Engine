#include "SpotLightShadowMap.h"


ComputeTileShading::SpotLightShadowMap::SpotLightShadowMap(uint32_t smWidth, uint32_t smHeight) : smWidth(smWidth), smHeight(smHeight)
{
	smShader = new Shader::ShaderSpotLightShadow();
	Init();
}

ComputeTileShading::SpotLightShadowMap::~SpotLightShadowMap() 
{
	smShadowMatrices.clear();

	delete smShader;

	glDeleteTextures(1, &smShadowTextureArray);
	glDeleteFramebuffers(1, &smFramebuffer);
}

void ComputeTileShading::SpotLightShadowMap::AddLightShadow(glm::vec4 fPositionAndRadius, glm::vec3 fEyePos, glm::vec3 fLookAt, float fFov)
{
	glm::mat4 mSpotLightProj = glm::perspectiveFov(fFov, 1.0f, 1.0f, 1.0f, fPositionAndRadius.w);
	glm::mat4 mSpotView = glm::lookAt(fEyePos, fLookAt, glm::vec3(0.0f, 1.0f, 0.0f));

	smShadowMatrices.push_back(mSpotLightProj * mSpotView);
	/*glm::mat4 mSpotTranslate(1.0f, 0.0f, 0.0f, 0.0f,
							0.0f, 1.0f, 0.0f, 0.0f,
							0.0f, 0.0f, 1.0f, 0.0f,
							-fPositionAndRadius.x, -fPositionAndRadius.y, -fPositionAndRadius.z, 1.0f);
	glm::vec4 vX, vY;
	glm::mat4 mSpotRotate(	vX,
							vY,
							glm::vec4(-lDirection.x, -lDirection.y, -lDirection.z, 0.0f),
							glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));*/
	/* D3DXMATRIX proj;
    D3DXMATRIX view;
    CalcSpotLightProj( positionAndRadius, proj );
    CalcSpotLightView( eye, lookAt, view );
    D3DXMATRIX viewProj = view * proj;
    D3DXMatrixTranspose( viewProjTransposed, &viewProj );
    D3DXMATRIX viewProjInv;
    D3DXMatrixInverse( &viewProjInv, NULL, &viewProj );
    D3DXMatrixTranspose( viewProjInvTransposed, &viewProjInv );*/
	// D3DXMatrixPerspectiveFovLH(&Proj, D3DXToRadian(70.52877936f), 1.0f, 2.0f, PositionAndRadius.w);
	/* D3DXVECTOR3 Up( 0.0f, 1.0f, 0.0f );
    D3DXMatrixLookAtLH( &View, &Eye, &LookAt, &Up );*/
}

const glm::mat4 *ComputeTileShading::SpotLightShadowMap::Draw(uint32_t &uCount)
{
	//////////////////////////////////////////////////////////////////////////////////
	glGetIntegerv(GL_VIEWPORT, sViewport);
	//////////////////////////////////////////////////////////////////////////////////
	glBindFramebuffer(GL_FRAMEBUFFER, smFramebuffer);
	//////////////////////////////////////////////////////////////////////////////////
	GLenum drawMode[1] = { GL_NONE };
	glDrawBuffers(1, drawMode);
	//////////////////////////////////////////////////////////////////////////////////
	glViewport(0, 0, smWidth, smHeight);
	//////////////////////////////////////////////////////////////////////////////////
	glClearDepth(1.0f);
	glClear(GL_DEPTH_BUFFER_BIT);
	//////////////////////////////////////////////////////////////////////////////////
	glEnable(GL_POLYGON_OFFSET_FILL);
	glPolygonOffset(2.0f, 4.0f);//16.0f, 44.0f);
	//////////////////////////////////////////////////////////////////////////////////

	uCount = smShadowMatrices.size();

	return smShadowMatrices.data();
}

void ComputeTileShading::SpotLightShadowMap::EndDraw(void)
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glDrawBuffer(GL_BACK);
	glViewport(sViewport[0], sViewport[1], sViewport[2], sViewport[3]);
	glDisable(GL_POLYGON_OFFSET_FILL);

	glBindTexture(GL_TEXTURE_2D_ARRAY, smShadowTextureArray);
	glGenerateMipmap(GL_TEXTURE_2D_ARRAY);
	glBindTexture(GL_TEXTURE_2D_ARRAY, 0);
}

void ComputeTileShading::SpotLightShadowMap::Init(void)
{
	glGenTextures(1, &smShadowTextureArray);
	glBindTexture(GL_TEXTURE_2D_ARRAY, smShadowTextureArray);
	glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_DEPTH_COMPONENT32, smWidth, smHeight, 1, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

	glTexParameterf(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameterf(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);// GL_COMPARE_REF_TO_TEXTURE);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);

	glTexParameterf(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_BASE_LEVEL, 0);

	glGenerateMipmap(GL_TEXTURE_2D_ARRAY);
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	glBindTexture(GL_TEXTURE_2D_ARRAY, 0);
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	glGenFramebuffers(1, &smFramebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, smFramebuffer);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, smShadowTextureArray, 0);
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	//////////////////////////////////////////////////////////////////////////////////////////////////////
}

 Shader::ShaderSpotLightShadow *ComputeTileShading::SpotLightShadowMap::GetShadowShader(void)
{
	return smShader;
}

 GLuint ComputeTileShading::SpotLightShadowMap::GetMaps(void)
 {
	 return smShadowTextureArray;
 }

 uint32_t ComputeTileShading::SpotLightShadowMap::GetShadowMapCount(void)
 {
	 return smShadowMatrices.size();
 }

 const glm::mat4 *ComputeTileShading::SpotLightShadowMap::GetShadowMapMatrixes(void)
 {
	 return smShadowMatrices.data();
 }

void ComputeTileShading::SpotLightShadowMap::ReCreateShadowMaps(void) 
{
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, 0, 0);
	glDeleteTextures(1, &smShadowTextureArray);
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	glGenTextures(1, &smShadowTextureArray);
	glBindTexture(GL_TEXTURE_2D_ARRAY, smShadowTextureArray);
	glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_DEPTH_COMPONENT32, smWidth, smHeight, smShadowMatrices.size(), 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

	glTexParameterf(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameterf(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);

	glTexParameterf(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_BASE_LEVEL, 0);

	glGenerateMipmap(GL_TEXTURE_2D_ARRAY);
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	glBindTexture(GL_TEXTURE_2D_ARRAY, 0);
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	glGenFramebuffers(1, &smFramebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, smFramebuffer);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, smShadowTextureArray, 0);
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	//////////////////////////////////////////////////////////////////////////////////////////////////////
}