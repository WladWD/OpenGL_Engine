#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/constants.hpp>
#include <cmath>
#include <vector>

#pragma once
namespace Weather
{
	class WeatherParam
	{
		/////////////////////////////////////////////////////////////////////////////////////////////////////
		const float meter;
		/////////////////////////////////////////////////////////////////////////////////////////////////////
		//Wind Param
		std::vector<glm::vec3> wind_state;//xyz - направление ветра, length - сила ветра
		uint32_t wind_cur_state, wind_next_state;//[0; state_count-1]
		double wind_cur_inter_time, wind_inter_time;//wind_cur_inter_time - текущее время интерполяции, wind_inter_time - общее время интерполяции
		double wind_min_time_interval, wind_max_time_interval;//in second
		glm::vec3 now_wind;
		/////////////////////////////////////////////////////////////////////////////////////////////////////
		double cloud_min_time_interval, cloud_max_time_interval;//cloud_max_time_interval - не меньше 3 минут
		double cloud_cur_inter_time, cloud_inter_time;
		std::vector<float> cloud_state;// - плотность облаков, 0 состояние 1.0 плотность
		uint32_t cloud_cur_state, cloud_next_state;
		float cloud_gray_scale_now, cloud_gray_scale_next;
		float cloud_now;//- текущая плотность облаков
		float cloud_gray_scale;
		/////////////////////////////////////////////////////////////////////////////////////////////////////
		//при достижении cloud_now - 0.8f (вероятность начала дождя - 0.001), при достижении 0.98(вероятность начала дождя - 0.9) при 1.0(вероятность начала дождя - 1.0)
		//если дождь пошел то следующее состояние(cloud_state) 0 на время пока идет дождь 
		double rain_min_time_interval, rain_max_time_interval;//cloud_max_time_interval - не меньше 2 минут
		double rain_cur_inter_time, rain_inter_time;
		float rain_current, rain_next;
		float wait_time;
		bool rain_comming;//пошел ли дождь
		float rain_now;//при rain_comming == false => = 0.0f
		/////////////////////////////////////////////////////////////////////////////////////////////////////
		double random(void);//return random number - [0;1]
		uint8_t random(double probability);//return random number - [0;1] with probability
		/////////////////////////////////////////////////////////////////////////////////////////////////////
		void UpdateWind(double dt_ms);
		void InitWind(void);
		void LerpWind(void);
		/////////////////////////////////////////////////////////////////////////////////////////////////////
		void InitCloud(void);
		void UpdateCloud(double dt_s);
		void LerpCloud(void);
		/////////////////////////////////////////////////////////////////////////////////////////////////////
		void InitRain(void);
		void UpdateRain(double dt_s);
		void LerpRain(void);
	public:
		WeatherParam(float meter);
		~WeatherParam();

		void Update(double dt_ms);
		glm::vec3 GetWind(void);
		float GetCloudDensity(void);
		float GetCloudGrayScale(void);
		float GetRainIntensity(void);
	};
	//1)Симуляция ветра
	//2)Симуляция плотности облаков
	//3)Симуляция плотности дождя
}

