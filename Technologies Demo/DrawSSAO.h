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

class DrawSSAO {

public:
	DrawSSAO(const MCamera::Camera* camera);
	~DrawSSAO();

	void draw(GLuint depth, GLuint normal, GLuint specular, GLuint diffuse);

private:
	void initializeRandomVector();

	std::unique_ptr<NoiseTexture2D<GL_RGBA16F, GL_RGBA, GL_FLOAT>> noiseTextre;
	std::unique_ptr<QuadMesh> mesh;
	std::vector<glm::vec4> randomVertor;
	const MCamera::Camera* camera;
	std::unique_ptr<Shader::ShaderSSAO> shader;
};
}

