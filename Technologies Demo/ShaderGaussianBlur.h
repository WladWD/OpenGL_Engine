#include "Shader_Resource_Compiler.h"
#include "ShaderBase.h"
#include <vector>
#include <array>

#pragma once
namespace Shader {
class ShaderGaussianBlur : public ShaderBase {
public:
	static constexpr int kernelSize = 8;

private:
#define mprog_count 1

		GLint mipBias[mprog_count];
		GLint texOffset[mprog_count];
		GLint gaussian[mprog_count];

		GLint gTexture[mprog_count];
		GLint gSourceTexture_slot;

#undef mprog_count

		void InitVariable(GLuint program);
		void bindTextureToSlot();
public:
		ShaderGaussianBlur();
		virtual ~ShaderGaussianBlur();

		GLuint getProgram();

		void setMipBias(float mipBias);
		void setTexOffset(const glm::vec2& texOffset);
		void setGaussian(const std::vector<float>& gaussian);

		void bindSourceTexture(GLuint texture);
	};
}
