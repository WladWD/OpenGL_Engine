#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/constants.hpp>
#include <cmath>

#pragma once
namespace Atmosphere
{
	struct GlobalLightingParam 
	{
		glm::vec3 light_dir;
		glm::vec4 light_diffuse_color;
		glm::vec4 light_specurar_color;

		bool need_update_atmosphere;
		float star_intensity;
		float Phi, Thetta;
	};

	struct MoonLightingParam
	{
		glm::vec3 light_dir;
		glm::vec4 light_diffuse_color;
		glm::vec4 light_specurar_color;
	};

	struct ScatteringParam 
	{
		glm::vec3 waveLengths;
		glm::vec3 invWaveLengths;
		glm::vec3 waveLengthsMie;
	};

	class AtmosphereParameter
	{
		//real ms -> to game local minute
		const float mconst;
		const float mull_const, offset_const;
		const float real_time_to_lacal_time;//константа для преобразования интервала в игровой интервал времени
		float curret_time_minutes;//текущее время суток в минутах [0; 1440]
		////////////////////////////////////////////////////////////////////////
		float Phi, Thetta;
		///
		float phi, thetta;
		////////////////////////////////////////////////////////////////////////
		ScatteringParam scattering_param;
		////////////////////////////////////////////////////////////////////////
		GlobalLightingParam light_param;
		MoonLightingParam light_param_moon;
		////////////////////////////////////////////////////////////////////////
		glm::vec3 GetLightDirection(float fPhi, float fThetta);
		glm::vec4 GetLihgtDiffuseColor(float fMThetta, int nTurbidity);
		glm::vec4 GetLihgtDiffuseColor(int nTurbidity);

		glm::vec4 GetDiffuseColor(float fThetta, int nTurbidity);
		void InitScatteringParam(void);
	public:
		AtmosphereParameter(void);
		~AtmosphereParameter();
		////////////////////////////////////////////////////////////////////////
		void UpdateTime(float delta_ms);
		void UpdateTimet(float delta_ms);
		////////////////////////////////////////////////////////////////////////
		const ScatteringParam *GetScatteringParameter(void);
		////////////////////////////////////////////////////////////////////////
		const GlobalLightingParam &GetGlobalLightingParameter(void);
		const MoonLightingParam &GetGlobalLightingParameterMoon(void);
		////////////////////////////////////////////////////////////////////////
	};
}

