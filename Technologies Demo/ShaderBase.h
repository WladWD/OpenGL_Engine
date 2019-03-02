#include "Shader_Resource_Compiler.h"

#pragma once
namespace Shader
{
#define to_shader_name(s) #s
#define correct_programm if(current_program == (-1)) MessageBox(NULL, L"Program is not bind!!!", L"Error", MB_OK);
#define mcheck(p) if(p[current_program] >= 0)
	class ShaderBase
	{
	protected:
		const uint32_t program_count;
		const uint32_t shader_count;
		GLuint *program;
		GLuint *shaders;
		////////////////////////////////////////////////////////////////////////////////////////
		GLint current_program;//[-1;0...program_count-1]
		////////////////////////////////////////////////////////////////////////////////////////
		void MCreateProgram(void);
		void InitProgram(GLuint cs, GLuint program);
		void InitProgram(GLuint vs, GLuint ps, GLuint program);
		void InitProgram(GLuint vs, GLuint gs, GLuint ps, GLuint program);
		void CompileShader(Shader_Resource *shaders_compile);
		void PackToShader(Shader_Resource *shaders_compile);
		void MInitShaderVariable(void);
		virtual void InitVariable(GLuint program) = 0;
	public:
		ShaderBase(uint32_t program_count_, uint32_t shader_count);
		virtual ~ShaderBase();
		////////////////////////////////////////////////////////////////////////////////////////
		void ResetProgram(void);//устанавливает current_program в (-1)
	};
}

