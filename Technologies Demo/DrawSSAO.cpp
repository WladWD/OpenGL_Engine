#include "DrawSSAO.h"
#include "MCamera.h"
#include "ShaderSSAO.h"
#include "NoiseTexture2D.h"
#include "GaussianBlur.h"
#include "QuadMesh.h"
#include "Framebuffer.h"
#include <random>

using namespace SSAO;

SSAO::DrawSSAO::DrawSSAO(const MCamera::Camera * camera): camera(camera) {
	shader = std::make_unique<Shader::ShaderSSAO>();

	std::uniform_real_distribution<float> randomFloats(0.0f, 1.0f);
	std::default_random_engine generator;
	noiseTextre = std::make_unique<NoiseTexture2D<GL_RGBA16F, GL_RGBA, GL_FLOAT>>(4, 4,
		[&generator, &randomFloats] (uint32_t x, uint32_t y) -> glm::vec4 {
		return glm::vec4(
			randomFloats(generator) * 2.0f - 1.0f,
			randomFloats(generator) * 2.0f - 1.0f,
			0.0f,
			1.0f
		);
	});
	framebuffer = std::make_unique<Framebuffer>(camera->GetWeight(), camera->GetHeight(), 
		std::vector<FramebufferTexture::FramebufferTextureFormat>{ {GL_RGBA8, GL_RGBA, GL_UNSIGNED_BYTE} });
	mesh = std::make_unique<QuadMesh>();
	drawBlur = std::make_unique<SSAO::GaussianBlur>(camera->GetWeight(), camera->GetHeight());
	initializeRandomVector();
}

SSAO::DrawSSAO::~DrawSSAO() { }

void SSAO::DrawSSAO::computeSSAO(GLuint depth, GLuint normal) {
	//glDisable(GL_CULL_FACE);
	//glDisable(GL_DEPTH_TEST);
	//glDepthMask(GL_FALSE);
	framebuffer->bind();

	glUseProgram(shader->getProgram());
	mesh->bindVAO();

	glm::mat4 world = glm::mat4(1.0f);

	shader->bindDepthTexture(depth);
	//shader->bindDiffuseTexture(diffuse);
	shader->bindNoiseTexture(noiseTextre->getTexture());
	shader->bindNormalTexture(normal);
	//shader->bindSpecularTexture(specular);
	shader->setInvProj(glm::inverse(camera->GetProjMatrix()));
	shader->setInvProjView(glm::inverse(camera->GetProjViewMatrix()));
	shader->setInvProjViewWorld(glm::inverse(camera->GetProjViewMatrix() * world));
	shader->setInvView(glm::inverse(camera->GetViewMatrix()));
	shader->setInvWorld(glm::inverse(world));
	shader->setNoiseScale(glm::vec2(camera->GetWeight() * 1.0f / noiseTextre->getWidth(), camera->GetHeight() * 1.0f / noiseTextre->getHeight()));
	shader->setProj(camera->GetProjMatrix());
	shader->setProjView(camera->GetProjViewMatrix());
	shader->setRandomVectors(this->randomVertor);
	shader->setView(camera->GetViewMatrix());
	shader->setWorld(world);

	mesh->draw();

	glBindVertexArray(0);
	glUseProgram(0);

	framebuffer->unbind();
	framebuffer->updateMip();

	drawBlur->makeBlur(framebuffer->getTexture(0)->getTexture(), 1.0);
}

GLuint DrawSSAO::getSSAOTexture() const {
	return drawBlur->getBlurTexture();// framebuffer->getTexture(0)->getTexture();
}

void SSAO::DrawSSAO::initializeRandomVector() {
	static const size_t kernelSize = shader->kernelSize;

	std::uniform_real_distribution<float> randomFloats(0.0f, 1.0f);
	std::default_random_engine generator;

	for (size_t i = 0; i < kernelSize; ++i) {
		glm::vec3 sample(
			randomFloats(generator) * 2.0f - 1.0f,
			randomFloats(generator) * 2.0f - 1.0f,
			randomFloats(generator)
		);
		sample = glm::normalize(sample);
		sample *= randomFloats(generator);

		float scale = static_cast<float>(i) / static_cast<float>(kernelSize);
		scale *= scale;
		scale = 0.1f * (1.0f - scale) + 1.0f * scale;
		sample *= scale;

		randomVertor.push_back(glm::vec4(sample, 1.0f));
	}
}
