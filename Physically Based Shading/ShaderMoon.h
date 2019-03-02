#include "Shader_Resource_Compiler.h"
#include "ShaderBase.h"

#pragma once
namespace Shader
{
	class ShaderMoon : public ShaderBase
	{
#define mprog_count 1
		////////////////////////////////////////////////////
		//static const int mprog_count = 1;//количество программ для текущего шейдера
		////////////////////////////////////////////////////
		GLint g_proj_view_world[mprog_count];
		GLint g_world[mprog_count];
		GLint g_alpha[mprog_count];
		////////////////////////////////////////////////////
		GLint MTexture[mprog_count];
		////////////////////////////////////////////////////
		GLint MTexture_slot;
		////////////////////////////////////////////////////
#undef mprog_count
		////////////////////////////////////////////////////
		void InitVariable(GLuint program);
	public:
		ShaderMoon(void);
		~ShaderMoon();
		////////////////////////////////////////////////////
		GLuint GetProgram(void);
		////////////////////////////////////////////////////
		void SetProjViewWorld(glm::mat4 matrix);
		void SetWorld(glm::mat4 matrix);
		void SetAlpha(float val);
		////////////////////////////////////////////////////
		void SetTexture(void);
		////////////////////////////////////////////////////
		void BindTextureToUnit(void);
	};
}
/*layout (location = 0) uniform mat4 g_proj_view_world;
layout (location = 1) uniform mat4 g_world;
layout (location = 2) uniform float g_alpha;
////////////////////////////////////////////////////
layout (location = 3, binding = 0) uniform sampler2D MTexture;*/