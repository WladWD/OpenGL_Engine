#include "PointLightShadowMap.h"


ComputeTileShading::PointLightShadowMap::PointLightShadowMap(uint32_t smWidth, uint32_t smHeight) : smWidth(smWidth), smHeight(smHeight)
{
	smShader = new Shader::ShaderPointLightShadow();
	Init();
}

ComputeTileShading::PointLightShadowMap::~PointLightShadowMap()
{
	smShadowMatrices.clear();

	delete smShader;

	glDeleteTextures(1, &smShadowTextureArray);
	glDeleteFramebuffers(1, &smFramebuffer);
}

glm::mat4 ComputeTileShading::PointLightShadowMap::GetViewMatrix(glm::vec4 fPositionAndRadius, int faceID)
{
	glm::vec3 vDir[6] = { glm::vec3( 1.0f, 0.0f, 0.0f),
						  glm::vec3(-1.0f, 0.0f, 0.0f), 
						  glm::vec3(0.0f,  1.0f, 0.0f), 
						  glm::vec3(0.0f, -1.0f, 0.0f),
						  glm::vec3(0.0f, 0.0f,  1.0f),
						  glm::vec3(0.0f, 0.0f, -1.0f) };

	glm::vec3 up[6] = 
	{
		glm::vec3(0.0f, -1.0f, 0.0f),
		glm::vec3(0.0f, -1.0f, 0.0f),
		glm::vec3(0.0f, 0.0f,  1.0f),	//-1
		glm::vec3(0.0f, 0.0f, -1.0f),	// 1
		glm::vec3(0.0f, -1.0f, 0.0f),
		glm::vec3(0.0f, -1.0f, 0.0f)
	};

	glm::vec3 fEyePos = glm::vec3(fPositionAndRadius.x, fPositionAndRadius.y, fPositionAndRadius.z);
	glm::vec3 fLookAt = fEyePos + vDir[faceID];
	
	glm::mat4 mPointView = glm::lookAt(fEyePos, fLookAt, up[faceID]);
		//fLookAt, fEyePos, up[faceID]);
	//fEyePos, fLookAt, up[faceID]); //- cur
	return mPointView;
}

void ComputeTileShading::PointLightShadowMap::AddLightShadow(glm::vec4 fPositionAndRadius)
{
	/*#define GL_TEXTURE_CUBE_MAP_POSITIVE_X 0x8515
#define GL_TEXTURE_CUBE_MAP_NEGATIVE_X 0x8516
#define GL_TEXTURE_CUBE_MAP_POSITIVE_Y 0x8517
#define GL_TEXTURE_CUBE_MAP_NEGATIVE_Y 0x8518
#define GL_TEXTURE_CUBE_MAP_POSITIVE_Z 0x8519
#define GL_TEXTURE_CUBE_MAP_NEGATIVE_Z 0x851A*/

	glm::mat4 mProjMatrix = glm::perspectiveFov(glm::pi<float>() * 0.5f, 1.0f, 1.0f, 1.0f, fPositionAndRadius.w);
	/*Result[2][2] = - (zFar + zNear) / (zFar - zNear);
		Result[2][3] = - static_cast<T>(1);
		Result[3][2] = - (static_cast<T>(2) * zFar * zNear) / (zFar - zNear);*/
	smParamData.push_back(glm::vec4(mProjMatrix[2][2], mProjMatrix[3][2], mProjMatrix[2][3], 1.0f));



	float z = -250.0;
	float d = z * smParamData[0].x + smParamData[0].y;
	d /= z*smParamData[0].z;

	glm::vec4 md = mProjMatrix * glm::vec4(0.0, 0.0, z, 1.0f);
	md /= md.w;

	uint32_t index[] = { 0, 1, 2, 3, 4, 5};
	//{ 1, 0, 2, 3, 4, 5};
	//{ 1, 0, 3, 2, 4, 5};
	//{ 1, 0, 3, 2, 4, 5};
	for (register uint32_t i = 0; i < 6; i++) 
	{
		glm::mat4 mViewFaceMatrix = GetViewMatrix(fPositionAndRadius, index[i]);
		glm::mat4 mProjViewMatrix = mProjMatrix * mViewFaceMatrix;

		smShadowMatrices.push_back(mProjViewMatrix);
	}
}

const glm::mat4 *ComputeTileShading::PointLightShadowMap::Draw(uint32_t &uCount)
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
	glPolygonOffset(4.0f, 2.0f);//2.0f, 4.0f);//16.0f, 44.0f);
	//////////////////////////////////////////////////////////////////////////////////

	uCount = smShadowMatrices.size() / 6;

	return smShadowMatrices.data();
}

void ComputeTileShading::PointLightShadowMap::EndDraw(void)
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glDrawBuffer(GL_BACK);
	glViewport(sViewport[0], sViewport[1], sViewport[2], sViewport[3]);
	glDisable(GL_POLYGON_OFFSET_FILL);

	glBindTexture(GL_TEXTURE_CUBE_MAP_ARRAY, smShadowTextureArray);
	glGenerateMipmap(GL_TEXTURE_CUBE_MAP_ARRAY);
	glBindTexture(GL_TEXTURE_CUBE_MAP_ARRAY, 0);
}

void ComputeTileShading::PointLightShadowMap::Init(void)
{
	glGenTextures(1, &smShadowTextureArray);
	glBindTexture(GL_TEXTURE_CUBE_MAP_ARRAY, smShadowTextureArray);
	glTexImage3D(GL_TEXTURE_CUBE_MAP_ARRAY, 0, GL_DEPTH_COMPONENT32, smWidth, smHeight, 6, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

	glTexParameterf(GL_TEXTURE_CUBE_MAP_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameterf(GL_TEXTURE_CUBE_MAP_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexParameteri(GL_TEXTURE_CUBE_MAP_ARRAY, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);// GL_COMPARE_REF_TO_TEXTURE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP_ARRAY, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);

	glTexParameterf(GL_TEXTURE_CUBE_MAP_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_CUBE_MAP_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_CUBE_MAP_ARRAY, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_CUBE_MAP_ARRAY, GL_TEXTURE_BASE_LEVEL, 0);

	glGenerateMipmap(GL_TEXTURE_CUBE_MAP_ARRAY);
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	glBindTexture(GL_TEXTURE_CUBE_MAP_ARRAY, 0);
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	glGenFramebuffers(1, &smFramebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, smFramebuffer);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, smShadowTextureArray, 0);
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	//////////////////////////////////////////////////////////////////////////////////////////////////////
}

Shader::ShaderPointLightShadow *ComputeTileShading::PointLightShadowMap::GetShadowShader(void)
{
	return smShader;
}

const glm::vec4 * ComputeTileShading::PointLightShadowMap::GetShadowParam(void)
{
	return smParamData.data();
}

GLuint ComputeTileShading::PointLightShadowMap::GetMaps(void)
{
	return smShadowTextureArray;
}

uint32_t ComputeTileShading::PointLightShadowMap::GetShadowMapCount(void)
{
	return smShadowMatrices.size();
}

const glm::mat4 *ComputeTileShading::PointLightShadowMap::GetShadowMapMatrixes(void)
{
	return smShadowMatrices.data();
}

void ComputeTileShading::PointLightShadowMap::ReCreateShadowMaps(void)
{
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, 0, 0);
	glDeleteTextures(1, &smShadowTextureArray);
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	glGenTextures(1, &smShadowTextureArray);
	glBindTexture(GL_TEXTURE_CUBE_MAP_ARRAY, smShadowTextureArray);
	glTexImage3D(GL_TEXTURE_CUBE_MAP_ARRAY, 0, GL_DEPTH_COMPONENT32, smWidth, smHeight, smShadowMatrices.size(), 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

	glTexParameterf(GL_TEXTURE_CUBE_MAP_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameterf(GL_TEXTURE_CUBE_MAP_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexParameteri(GL_TEXTURE_CUBE_MAP_ARRAY, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP_ARRAY, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);

	glTexParameterf(GL_TEXTURE_CUBE_MAP_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_CUBE_MAP_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_CUBE_MAP_ARRAY, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_CUBE_MAP_ARRAY, GL_TEXTURE_BASE_LEVEL, 0);

	glGenerateMipmap(GL_TEXTURE_CUBE_MAP_ARRAY);
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	glBindTexture(GL_TEXTURE_CUBE_MAP_ARRAY, 0);
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	glGenFramebuffers(1, &smFramebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, smFramebuffer);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, smShadowTextureArray, 0);
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	//////////////////////////////////////////////////////////////////////////////////////////////////////
}

/*static void CalcPointLightView( const D3DXVECTOR4& PositionAndRadius, int nFace, D3DXMATRIX& View )
{
    const D3DXVECTOR3 dir[ 6 ] = 
    {
        D3DXVECTOR3(  1.0f,  0.0f,  0.0f ),
        D3DXVECTOR3( -1.0f,  0.0f,  0.0f ),
        D3DXVECTOR3(  0.0f,  1.0f,  0.0f ),
        D3DXVECTOR3(  0.0f, -1.0f,  0.0f ),
        D3DXVECTOR3(  0.0f,  0.0f, -1.0f ),
        D3DXVECTOR3(  0.0f,  0.0f,  1.0f ),
    };

    const D3DXVECTOR3 up[ 6 ] = 
    {
        D3DXVECTOR3( 0.0f, 1.0f,  0.0f ),
        D3DXVECTOR3( 0.0f, 1.0f,  0.0f ),
        D3DXVECTOR3( 0.0f, 0.0f,  1.0f ),
        D3DXVECTOR3( 0.0f, 0.0f, -1.0f ),
        D3DXVECTOR3( 0.0f, 1.0f,  0.0f ),
        D3DXVECTOR3( 0.0f, 1.0f,  0.0f ),
    };

    D3DXVECTOR3 eye( PositionAndRadius.x, PositionAndRadius.y, PositionAndRadius.z );

    D3DXVECTOR3 at = eye + dir[ nFace ];
    D3DXMatrixLookAtLH( &View, &eye, &at, &up[ nFace ] ); 
}*/
/*static void CalcPointLightProj( const D3DXVECTOR4& PositionAndRadius, D3DXMATRIX& Proj )
{
    D3DXMatrixPerspectiveFovLH( &Proj, D3DXToRadian( 90.0f ), 1.0f, 2.0f, PositionAndRadius.w );
}
*/
/*static void CalcPointLightViewProj( const D3DXVECTOR4& positionAndRadius, D3DXMATRIX* viewProjTransposed, D3DXMATRIX* viewProjInvTransposed )
{*/
/*D3DXMATRIX proj;
    D3DXMATRIX view;
    CalcPointLightProj( positionAndRadius, proj );
    for ( int i = 0; i < 6; i++ )
    {
        CalcPointLightView( positionAndRadius, i, view );
        D3DXMATRIX viewProj = view * proj;
        D3DXMatrixTranspose( &viewProjTransposed[i], &viewProj );
        D3DXMATRIX viewProjInv;
        D3DXMatrixInverse( &viewProjInv, NULL, &viewProj );
        D3DXMatrixTranspose( &viewProjInvTransposed[i], &viewProjInv );
    }*/
/*static void AddShadowCastingPointLight( const D3DXVECTOR4& positionAndRadius, DWORD color )
{
    static unsigned uShadowCastingPointLightCounter = 0;

    assert( uShadowCastingPointLightCounter < TiledLighting11::MAX_NUM_SHADOWCASTING_POINTS );

    g_ShadowCastingPointLightDataArrayCenterAndRadius[ uShadowCastingPointLightCounter ] = positionAndRadius;
    g_ShadowCastingPointLightDataArrayColor[ uShadowCastingPointLightCounter ] = color;

    CalcPointLightViewProj( positionAndRadius, g_ShadowCastingPointLightViewProjTransposed[uShadowCastingPointLightCounter], g_ShadowCastingPointLightViewProjInvTransposed[uShadowCastingPointLightCounter] );

	uShadowCastingPointLightCounter++;*/