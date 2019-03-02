#include "SpotLightManager.h"
#include <float.h>
	
ComputeTileShading::SpotLightManager::SpotLightManager(void) 
{
	Init();

	tbs.gBoundingSphereCenterRadius = gBoundingSphereCenterRadius;
	tbs.gLightColor = gLightColor;
	tbs.gSpotLightDirertionCosineAngle = gSpotLightDirertionCosineAngle;
}

ComputeTileShading::SpotLightManager::~SpotLightManager() 
{
	vSpotLightData.clear();
	g_BoundingSphereCenterRadius.clear();
	g_LightColor.clear();
	g_LightDirectionRadiusAndAngleCos.clear();
	///////////////////////////////////////////////////////////////////////////////////////
	glDeleteTextures(1, &gLightColor);
	glDeleteTextures(1, &gBoundingSphereCenterRadius);
	glDeleteTextures(1, &gSpotLightDirertionCosineAngle);

	glDeleteBuffers(1, &gLightColorBuffer);
	glDeleteBuffers(1, &gBoundingSphereCenterRadiusBuffer);
	glDeleteBuffers(1, &gSpotLightDirertionCosineAngleBuffer);
}

uint16_t ComputeTileShading::SpotLightManager::ConvertF32ToF16(float fValueToConvert)
{
	/*unsigned short AMD::ConvertF32ToF16(float fValueToConvert)
	{
	// single precision floating point format:
	// |1|   8    |          23           |
	// |s|eeeeeeee|mmmmmmmmmmmmmmmmmmmmmmm|

	// half precision floating point format:
	// |1|  5  |    10    |
	// |s|eeeee|mmmmmmmmmm|

	unsigned int uFloatBits = (*(unsigned int *) &fValueToConvert);

	// note, this functions does not handle values that are too large (i.e. overflow),
	// nor does it handle NaNs or infinity
	int  nExponent = (int)((uFloatBits & 0x7F800000u) >> 23) - 127 + 15;
	assert(nExponent < 31);

	// if the resulting value would be a denorm or underflow, then just return a (signed) zero
	if( nExponent <= 0 )
	{
	return (unsigned short)((uFloatBits & 0x80000000u) >> 16);
	}

	// else, exponent is in the range [1,30], and so we can represent
	// the value to convert as a normalized 16-bit float (with some loss of precision, of course)
	unsigned int uSignBit =      (uFloatBits & 0x80000000u) >> 16;
	unsigned int uExponentBits = (unsigned int)nExponent << 10;
	unsigned int uMantissaBits = (uFloatBits & 0x007FFFFFu) >> 13;

	return (unsigned short)(uSignBit | uExponentBits | uMantissaBits);
	}*/

	// single precision floating point format:
	// |1|   8    |          23           |
	// |s|eeeeeeee|mmmmmmmmmmmmmmmmmmmmmmm|

	// half precision floating point format:
	// |1|  5  |    10    |
	// |s|eeeee|mmmmmmmmmm|

	uint32_t uFloatBits = (*(unsigned int *)&fValueToConvert);

	// note, this functions does not handle values that are too large (i.e. overflow), 
	// nor does it handle NaNs or infinity
	int32_t  nExponent = (int32_t)((uFloatBits & 0x7F800000u) >> 23) - 127 + 15;
	assert(nExponent < 31);

	// if the resulting value would be a denorm or underflow, then just return a (signed) zero
	if (nExponent <= 0)
	{
		return (uint16_t)((uFloatBits & 0x80000000u) >> 16);
	}

	// else, exponent is in the range [1,30], and so we can represent 
	// the value to convert as a normalized 16-bit float (with some loss of precision, of course)
	uint32_t uSignBit = (uFloatBits & 0x80000000u) >> 16;
	uint32_t uExponentBits = (uint32_t)nExponent << 10;
	uint32_t uMantissaBits = (uFloatBits & 0x007FFFFFu) >> 13;

	return (uint16_t)(uSignBit | uExponentBits | uMantissaBits);
}

void ComputeTileShading::SpotLightManager::AddLight(glm::vec4 vlightPositionRadius, glm::vec3 vlightLookAtPosition, float vConeAngle, glm::vec4 vlightColor) 
{
	assert(vConeAngle > 0.0f);
	assert(vlightPositionRadius.w > 0.0f);

	
	glm::vec3 eye(vlightPositionRadius.x, vlightPositionRadius.y, vlightPositionRadius.z);
	glm::vec3 dir = vlightLookAtPosition - eye;
	dir = glm::normalize(dir);

	glm::vec3 boundingSpherePos = eye + (dir * vlightPositionRadius.w);

	SpotLight vlight;
	vlight.g_BoundingSphereCenterRadius = glm::vec4(boundingSpherePos, vlightPositionRadius.w);
	vlight.g_Diffuse = vlightColor;
	vlight.g_LightDirectionAndAngle = glm::vec4(dir, vConeAngle);

	float fCosAngle = cos(vConeAngle);
	float vLightRadius = vlightPositionRadius.w * 2.0f * fCosAngle;

	assert(fCosAngle > 0.0f);

	fCosAngle = dir.z > 0.0f ? fCosAngle : -fCosAngle;
	glm::vec4 vg_LightDirectionRadiusAndAngleCos = glm::vec4(dir.x, dir.y, fCosAngle, vLightRadius);
	///////////////////////////////////////////////////////////////////////////////////////////////////////
	vSpotLightData.push_back(vlight);

	g_BoundingSphereCenterRadius.push_back(vlight.g_BoundingSphereCenterRadius);
	g_LightColor.push_back(LightColor::GenFromVector4(vlight.g_Diffuse));//{ vlightColor.r, vlightColor.g, vlightColor.b, vlightColor.a });
	g_LightDirectionRadiusAndAngleCos.push_back(vg_LightDirectionRadiusAndAngleCos);
}

void ComputeTileShading::SpotLightManager::UpdateLight(const SpotLight &vlight, uint32_t i) 
{
	assert(i < vSpotLightData.size());

	float fCosAngle = cos(vlight.g_LightDirectionAndAngle.w);

	assert(fCosAngle > 0.0f);

	fCosAngle = vlight.g_LightDirectionAndAngle.z > 0.0f ? fCosAngle : -fCosAngle;

	float vLightRadius = vlight.g_BoundingSphereCenterRadius.w * 2.0f * fCosAngle;
	glm::vec4 vg_LightDirectionRadiusAndAngleCos = glm::vec4(vlight.g_LightDirectionAndAngle.x, vlight.g_LightDirectionAndAngle.y, fCosAngle, vLightRadius);

	vSpotLightData[i] = vlight; 
	g_BoundingSphereCenterRadius[i] = vlight.g_BoundingSphereCenterRadius;
	g_LightColor[i] = LightColor::GenFromVector4(vlight.g_Diffuse);//{ vlight.g_Diffuse.r, vlight.g_Diffuse.g, vlight.g_Diffuse.b, vlight.g_Diffuse.a };
	g_LightDirectionRadiusAndAngleCos[i] = vg_LightDirectionRadiusAndAngleCos;
}

void ComputeTileShading::SpotLightManager::SetLightToBuffer(void)
{
	///////////////////////////////////////////////////////////////////////////////////////
	glBindTexture(GL_TEXTURE_BUFFER, gLightColor);
	glTexBuffer(GL_TEXTURE_BUFFER, GL_RGBA8, 0);
	glBindTexture(GL_TEXTURE_BUFFER, gBoundingSphereCenterRadius);
	glTexBuffer(GL_TEXTURE_BUFFER, GL_RGBA32F, 0);
	glBindTexture(GL_TEXTURE_BUFFER, gSpotLightDirertionCosineAngle);
	glTexBuffer(GL_TEXTURE_BUFFER, GL_RGBA32F, 0);
	///////////////////////////////////////////////////////////////////////////////////////
	glBindTexture(GL_TEXTURE_BUFFER, 0);
	///////////////////////////////////////////////////////////////////////////////////////
	glDeleteBuffers(1, &gLightColorBuffer);
	glDeleteBuffers(1, &gBoundingSphereCenterRadiusBuffer);
	glDeleteBuffers(1, &gSpotLightDirertionCosineAngleBuffer);
	///////////////////////////////////////////////////////////////////////////////////////
	glGenBuffers(1, &gLightColorBuffer);
	glGenBuffers(1, &gBoundingSphereCenterRadiusBuffer);
	glGenBuffers(1, &gSpotLightDirertionCosineAngleBuffer);
	///////////////////////////////////////////////////////////////////////////////////////
	glBindBuffer(GL_TEXTURE_BUFFER, gLightColorBuffer);
	glBufferData(GL_TEXTURE_BUFFER, sizeof(LightColor) * g_LightColor.size(), g_LightColor.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_TEXTURE_BUFFER, gBoundingSphereCenterRadiusBuffer);
	glBufferData(GL_TEXTURE_BUFFER, sizeof(glm::vec4) * g_BoundingSphereCenterRadius.size(), g_BoundingSphereCenterRadius.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_TEXTURE_BUFFER, gSpotLightDirertionCosineAngleBuffer);
	glBufferData(GL_TEXTURE_BUFFER, sizeof(glm::vec4) * g_LightDirectionRadiusAndAngleCos.size(), g_LightDirectionRadiusAndAngleCos.data(), GL_STATIC_DRAW);
	///////////////////////////////////////////////////////////////////////////////////////
	glBindBuffer(GL_TEXTURE_BUFFER, 0);
	///////////////////////////////////////////////////////////////////////////////////////
	glBindTexture(GL_TEXTURE_BUFFER, gLightColor);
	glTexBuffer(GL_TEXTURE_BUFFER, GL_RGBA8, gLightColorBuffer);
	glBindTexture(GL_TEXTURE_BUFFER, gBoundingSphereCenterRadius);
	glTexBuffer(GL_TEXTURE_BUFFER, GL_RGBA32F, gBoundingSphereCenterRadiusBuffer);
	glBindTexture(GL_TEXTURE_BUFFER, gSpotLightDirertionCosineAngle);
	glTexBuffer(GL_TEXTURE_BUFFER, GL_RGBA32F, gSpotLightDirertionCosineAngleBuffer);
	///////////////////////////////////////////////////////////////////////////////////////
	glBindTexture(GL_TEXTURE_BUFFER, 0);
}

GLuint ComputeTileShading::SpotLightManager::GetSpotLightColorTexture(void) 
{
	return gLightColor;
}

GLuint ComputeTileShading::SpotLightManager::GetSpotLightBoundingSpherePositionRadius(void) 
{
	return gBoundingSphereCenterRadius;
}

GLuint ComputeTileShading::SpotLightManager::GetSpotLightDirectionCosineAngleRadius()
{
	return gSpotLightDirertionCosineAngle;
}

int32_t ComputeTileShading::SpotLightManager::GetLightCount(void)
{
	return g_BoundingSphereCenterRadius.size();
}

void ComputeTileShading::SpotLightManager::Init(void) 
{
	glGenTextures(1, &gLightColor);
	glGenTextures(1, &gBoundingSphereCenterRadius);
	glGenTextures(1, &gSpotLightDirertionCosineAngle);

	glGenBuffers(1, &gLightColorBuffer);
	glGenBuffers(1, &gBoundingSphereCenterRadiusBuffer);
	glGenBuffers(1, &gSpotLightDirertionCosineAngleBuffer);
}

const ComputeTileShading::TileBufferSpot &ComputeTileShading::SpotLightManager::GetTextureBuffer(void)
{
	return tbs;
}

/*static LightUtilSpotParams PackSpotParams(const D3DXVECTOR3& vLightDir, float fCosineOfConeAngle, float fFalloffRadius)
{
    assert( fCosineOfConeAngle > 0.0f );
    assert( fFalloffRadius > 0.0f );

    LightUtilSpotParams PackedParams;
    PackedParams.fLightDirX = AMD::ConvertF32ToF16( vLightDir.x );
    PackedParams.fLightDirY = AMD::ConvertF32ToF16( vLightDir.y );
    PackedParams.fCosineOfConeAngleAndLightDirZSign = AMD::ConvertF32ToF16( fCosineOfConeAngle );
    PackedParams.fFalloffRadius = AMD::ConvertF32ToF16( fFalloffRadius );

    // put the sign bit for light dir z in the sign bit for the cone angle
    // (we can do this because we know the cone angle is always positive)
    if( vLightDir.z < 0.0f )
    {
        PackedParams.fCosineOfConeAngleAndLightDirZSign |= 0x8000;
    }
    else
    {
        PackedParams.fCosineOfConeAngleAndLightDirZSign &= 0x7FFF;
    }

    return PackedParams;

}
static void AddShadowCastingSpotLight( const D3DXVECTOR4& positionAndRadius, const D3DXVECTOR3& lookAt, DWORD color )
{
static unsigned uShadowCastingSpotLightCounter = 0;

assert( uShadowCastingSpotLightCounter < TiledLighting11::MAX_NUM_SHADOWCASTING_SPOTS );

D3DXVECTOR3 eye( positionAndRadius );
D3DXVECTOR3 dir = lookAt - eye;
D3DXVec3Normalize( &dir, &dir );

D3DXVECTOR3 boundingSpherePos = eye + (dir * positionAndRadius.w);

g_ShadowCastingSpotLightDataArrayCenterAndRadius[ uShadowCastingSpotLightCounter ] = D3DXVECTOR4( boundingSpherePos, positionAndRadius.w );
g_ShadowCastingSpotLightDataArrayColor[ uShadowCastingSpotLightCounter ] = color;

// cosine of cone angle is cosine(35.26438968 degrees) = 0.816496580927726
g_ShadowCastingSpotLightDataArraySpotParams[ uShadowCastingSpotLightCounter ] = PackSpotParams( dir, 0.816496580927726f, positionAndRadius.w * 1.33333333f );

}

// Curtain spot
AddShadowCastingSpotLight( D3DXVECTOR4(  -772.0f, 254.0f, -503.0f, 800.0f ), D3DXVECTOR3( -814.0f, 180.0f, -250.0f ), COLOR( 255, 255, 255 ) );

// Lion spots
AddShadowCastingSpotLight( D3DXVECTOR4(  1130.0f, 378.0f, 40.0f, 500.0f ), D3DXVECTOR3( 1150.0f, 290.0f, 40.0f ), COLOR( 200, 200, 100 ) );
AddShadowCastingSpotLight( D3DXVECTOR4( -1260.0f, 378.0f, 40.0f, 500.0f ), D3DXVECTOR3( -1280.0f, 290.0f, 40.0f ), COLOR( 200, 200, 100 ) );

// Gallery spots
AddShadowCastingSpotLight( D3DXVECTOR4( -115.0f, 660.0f, -100.0f, 800.0f ), D3DXVECTOR3( -115.0f, 630.0f, 0.0f ), COLOR( 200, 200, 200 ) );
AddShadowCastingSpotLight( D3DXVECTOR4( -115.0f, 660.0f,  100.0f, 800.0f ), D3DXVECTOR3( -115.0f, 630.0f, -100.0f ), COLOR( 200, 200, 200 ) );

AddShadowCastingSpotLight( D3DXVECTOR4( -770.0f, 660.0f, -100.0f, 800.0f ), D3DXVECTOR3( -770.0f, 630.0f, 0.0f ), COLOR( 200, 200, 200 ) );
AddShadowCastingSpotLight( D3DXVECTOR4( -770.0f, 660.0f,  100.0f, 800.0f ), D3DXVECTOR3( -770.0f, 630.0f, -100.0f ), COLOR( 200, 200, 200 ) );

AddShadowCastingSpotLight( D3DXVECTOR4( 500.0f, 660.0f, -100.0f, 800.0f ), D3DXVECTOR3( 500.0f, 630.0f, 0.0f ), COLOR( 200, 200, 200 ) );
AddShadowCastingSpotLight( D3DXVECTOR4( 500.0f, 660.0f,  100.0f, 800.0f ), D3DXVECTOR3( 500.0f, 630.0f, -100.0f ), COLOR( 200, 200, 200 ) );

// Red corner spots
AddShadowCastingSpotLight( D3DXVECTOR4( -1240.0f, 90.0f, -70.0f, 700.0f ), D3DXVECTOR3( -1240.0f, 140.0f, -405.0f ), COLOR( 200, 0, 0 ) );
AddShadowCastingSpotLight( D3DXVECTOR4( -1000.0f, 90.0f, -260.0f, 700.0f ), D3DXVECTOR3( -1240.0f, 140.0f, -405.0f ), COLOR( 200, 0, 0 ) );

// Green corner spot
AddShadowCastingSpotLight( D3DXVECTOR4( -900.0f, 60.0f, 340.0f, 700.0f ), D3DXVECTOR3( -1360.0f, 255.0f, 555.0f ), COLOR( 100, 200, 100 ) );
*/