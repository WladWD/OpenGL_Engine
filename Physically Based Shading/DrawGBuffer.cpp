#include "DrawGBuffer.h"


DeferredRendering::DrawGBuffer::DrawGBuffer(MCamera::Camera *camera, MEngine::MResource *mresource, MEngine::MObjects *VNT_object, MEngine::MDrawList VNT_draw_list,
	MEngine::MObjects *VNTT_object, MEngine::MDrawList VNTT_draw_list, uint32_t height, uint32_t width) :
height(height), width(width), mresource(mresource), camera(camera)
{
	Init();

	draw_vnt = new DeferredDrawVNT(mresource, VNT_object, VNT_draw_list, camera);
	draw_vntt = new DeferredDrawVNTT(mresource, VNTT_object, VNTT_draw_list, camera);

	gbuffer_data.g_buffer = g_buffer;
	gbuffer_data.g_buffer_depth = g_buffer_depth;
	gbuffer_data.g_buffer_textures[0] = g_buffer_textures[0];
	gbuffer_data.g_buffer_textures[1] = g_buffer_textures[1];
	gbuffer_data.g_buffer_textures[2] = g_buffer_textures[2];
	gbuffer_data.g_buffer_textures[3] = g_buffer_textures[3];
}

DeferredRendering::DrawGBuffer::~DrawGBuffer()
{
	glDeleteTextures(GBufferTexturesCount, g_buffer_textures);
	glDeleteTextures(1, &g_buffer_depth);
	glDeleteFramebuffers(1, &g_buffer);

	mresource->mMaterial.clear();
	delete mresource->texture_map;
	delete mresource;

	delete draw_vntt;
	delete draw_vntt;
}

void DeferredRendering::DrawGBuffer::Init(void)
{
	glGenTextures(GBufferTexturesCount, g_buffer_textures);
	glGenTextures(1, &g_buffer_depth);
	///////////////////////////////////////////////////////////////////////////////////////////
	glBindTexture(GL_TEXTURE_2D, g_buffer_textures[0]);
	
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, width, height, 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);

	glGenerateMipmap(GL_TEXTURE_2D);
	///////////////////////////////////////////////////////////////////////////////////////////
	glBindTexture(GL_TEXTURE_2D, g_buffer_textures[1]);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);

	glGenerateMipmap(GL_TEXTURE_2D);
	///////////////////////////////////////////////////////////////////////////////////////////
	glBindTexture(GL_TEXTURE_2D, g_buffer_textures[2]);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);

	glGenerateMipmap(GL_TEXTURE_2D);
	///////////////////////////////////////////////////////////////////////////////////////////
	glBindTexture(GL_TEXTURE_2D, g_buffer_textures[3]);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, width, height, 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);

	glGenerateMipmap(GL_TEXTURE_2D);
	///////////////////////////////////////////////////////////////////////////////////////////
	glBindTexture(GL_TEXTURE_2D, g_buffer_depth);
	//GL_DEPTH_COMPONENT32 GL_DEPTH_COMPONENT
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);

	glGenerateMipmap(GL_TEXTURE_2D);
	///////////////////////////////////////////////////////////////////////////////////////////
	glBindTexture(GL_TEXTURE_2D, 0);
	///////////////////////////////////////////////////////////////////////////////////////////
	glGenFramebuffers(1, &g_buffer);

	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, g_buffer);
	glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, g_buffer_textures[0], 0);
	glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, g_buffer_textures[1], 0);
	glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, g_buffer_textures[2], 0);
	glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, GL_TEXTURE_2D, g_buffer_textures[3], 0);
	glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, g_buffer_depth, 0);

	GLenum res_val;
	if ((res_val = glCheckFramebufferStatus(GL_DRAW_FRAMEBUFFER)) != GL_FRAMEBUFFER_COMPLETE)
		MessageBox(NULL, L"Framebuffer is not complette", L"", MB_OK);

	if (GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT == res_val)
		MessageBox(NULL, L"Framebuffer is not complette", L"", MB_OK);

	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
}

void DeferredRendering::DrawGBuffer::SetGBufferDraw(void)
{
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, g_buffer);
	GLenum draw_buffers[4] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
	glDrawBuffers(4, draw_buffers);

	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClearDepth(1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void DeferredRendering::DrawGBuffer::Draw(void)
{
	SetGBufferDraw();
	draw_vnt->Draw();
	draw_vntt->Draw();
}

void DeferredRendering::DrawGBuffer::DrawSpotShadow(const glm::mat4 *matrixes, uint32_t uCount, Shader::ShaderSpotLightShadow *shader)
{
	draw_vnt->DrawShadow(matrixes, uCount, shader);
	draw_vntt->DrawShadow(matrixes, uCount, shader);
}

void DeferredRendering::DrawGBuffer::DrawShadow(const glm::mat4 *matrixes, uint32_t uCount, Shader::ShaderPointLightShadow *shader)
{
	draw_vnt->DrawShadow(matrixes, uCount, shader);
	draw_vntt->DrawShadow(matrixes, uCount, shader);
}

const DeferredRendering::GBufferData &DeferredRendering::DrawGBuffer::GetBufferData(void)
{
	return gbuffer_data;
}
