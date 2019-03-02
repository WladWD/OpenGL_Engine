#include "FinalFramebuffer.h"


Render::FinalFrame::FinalFrame(GLuint mDepth, uint32_t mDimX, uint32_t mDimY) : mDepth(mDepth), mDimX(mDimX), mDimY(mDimY)
{
	InitFramebuffer();
}

Render::FinalFrame::~FinalFrame(void) 
{
	glDeleteTextures(1, &mImage);
	glDeleteFramebuffers(1, &mFramebuffer);
}

void Render::FinalFrame::InitFramebuffer(void) 
{
	////////////////////////////////////////////////////////////////////////////////////////////////////
	glGenTextures(1, &mImage);
	glBindTexture(GL_TEXTURE_2D, mImage);
	////////////////////////////////////////////////////////////////////////////////////////////////////
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, mDimX, mDimY, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
	////////////////////////////////////////////////////////////////////////////////////////////////////
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
	////////////////////////////////////////////////////////////////////////////////////////////////////
	glGenFramebuffers(1, &mFramebuffer);

	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, mFramebuffer);
	glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mImage, 0);
	//glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, mDepth, 0);

	GLenum res_val;
	if ((res_val = glCheckFramebufferStatus(GL_DRAW_FRAMEBUFFER)) != GL_FRAMEBUFFER_COMPLETE)
		MessageBox(NULL, L"Framebuffer is not complette", L"", MB_OK);

	if (GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT == res_val)
		MessageBox(NULL, L"Framebuffer is not complette", L"", MB_OK);

	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
}

void Render::FinalFrame::Resize(GLuint mDepth, uint32_t mDimX, uint32_t mDimY)
{
	this->mDepth = mDepth;
	this->mDimX = mDimX;
	this->mDimY = mDimY;

	glDeleteTextures(1, &mImage);
	glDeleteFramebuffers(1, &mFramebuffer);

	InitFramebuffer();
}
		
void Render::FinalFrame::BingFramebuffer(void) 
{
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, mFramebuffer);

	GLenum draw_buffers[1] = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, draw_buffers);

	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	//glClearDepth(1.0f);
	//glClear(GL_COLOR_BUFFER_BIT);
}
	
GLuint Render::FinalFrame::GetCurrentFramebuffer(void) 
{
	return mFramebuffer;
}