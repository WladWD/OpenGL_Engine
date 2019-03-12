#include "MainHeader.h"
#include <memory>
#include <vector>

#pragma once
namespace MCamera {
class Camera;
}

namespace Shader {
class ShaderSSAO;
}

namespace SSAO {
template<
GLint InternalFormat,
GLenum Format,
GLenum Type>
class NoiseTexture2D;
class QuadMesh;
class GaussianBlur;
class Framebuffer;

class DrawSSAO {

public:
	DrawSSAO(const MCamera::Camera* camera);
	~DrawSSAO();

	void computeSSAO(GLuint depth, GLuint normal);
	GLuint getSSAOTexture() const;

private:
	void initializeRandomVector();

	std::unique_ptr<Framebuffer> framebuffer;
	std::unique_ptr<NoiseTexture2D<GL_RGBA16F, GL_RGBA, GL_FLOAT>> noiseTextre;
	std::unique_ptr<QuadMesh> mesh;
	std::vector<glm::vec4> randomVertor;
	const MCamera::Camera* camera;
	std::unique_ptr<SSAO::GaussianBlur> drawBlur;
	std::unique_ptr<Shader::ShaderSSAO> shader;
};
}

