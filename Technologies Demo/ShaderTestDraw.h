#include "Shader_Resource_Compiler.h"

#pragma once
namespace Shader
{
	class ShaderTestDraw
	{
		GLuint shaders[2];
		GLuint program;
		////////////////////////////////////////////////////
		GLint proj_view_world;
		GLint g_cam_pos;
		////////////////////////////////////////////////////
		GLint diffuse;
		GLint specular;
		GLint emissive;
		////////////////////////////////////////////////////
		GLint g_diffuse;
		////////////////////////////////////////////////////
		GLint g_lightDir;//mast be normalize
		GLint g_lightAmbient;
		GLint g_lightDiffuse;
		GLint g_lightSpecular;
		////////////////////////////////////////////////////
	public:
		ShaderTestDraw();
		~ShaderTestDraw();
		////////////////////////////////////////////////////
		GLuint GetProgram();
		////////////////////////////////////////////////////
		void SetProjViewWorld(glm::mat4 matrix);
		void SetCamPos(glm::vec3 vector);
		////////////////////////////////////////////////////
		void SetMaterialDiffuse(glm::vec4 color);
		void SetMaterialSpecular(glm::vec4 color);
		void SetMaterialEmissive(glm::vec4 color);
		////////////////////////////////////////////////////
		void BindTextureDiffuse(GLenum target, GLuint texture);
		////////////////////////////////////////////////////
		void SetLightDir(glm::vec3 vector);
		void SetLightAmbient(glm::vec4 color);
		void SetLightDiffuse(glm::vec4 color);
		void SetLightSpecular(glm::vec4 color);
	};
}