#include "WeatherParam.h"


Weather::WeatherParam::WeatherParam(float meter) : meter(meter)
{
	InitWind();
	InitCloud();
	InitRain();
	cloud_gray_scale = 1.0f;
}

Weather::WeatherParam::~WeatherParam()
{
	wind_state.clear();
	cloud_state.clear();
}

double Weather::WeatherParam::random(void)
{
	return (((double)rand() / ((double)(RAND_MAX))));
}

uint8_t Weather::WeatherParam::random(double probability)
{
	if (probability == 0.0f)return 0;

	int32_t p = 1.0 / probability;
	if(!(rand() % p)) return 1;
	else return 0;
}

void Weather::WeatherParam::InitWind(void)
{
	wind_cur_state = 0;
	wind_next_state = 1;
	wind_min_time_interval = 5;//5;
	wind_max_time_interval = 30;//30;
	wind_cur_inter_time = 0.0;
	wind_inter_time = wind_min_time_interval + random() * (wind_max_time_interval - wind_min_time_interval);

	wind_state.push_back(glm::vec3(-0.1, -0.5, 0));
	wind_state.push_back(glm::vec3(-0.4, -0.5, 0.04));
	wind_state.push_back(glm::vec3(-0.2, -0.5, -0.4));
	wind_state.push_back(glm::vec3(0.0, -0.5, -0.02));
	wind_state.push_back(glm::vec3(0.0, -0.5, -0.02));
	wind_state.push_back(glm::vec3(0.1, -0.5, 0.4));
	wind_state.push_back(glm::vec3(-0.1, -0.5, 0));
	
	for (register uint32_t i = 0; i < 10; i++) 
	{
		glm::vec3 wind;
		wind.y = -(random() * 0.8f) - 0.2f;
		wind.x = glm::clamp(float(random()*0.5f) - 0.5f, -0.4f, 0.4f);
		wind.z = glm::clamp(float(random()*0.5f) - 0.5f, -0.4f, 0.4f);
		wind_state.push_back(wind);
	}

}

void Weather::WeatherParam::LerpWind(void)
{
	glm::vec3 cur_wind = wind_state[wind_cur_state];
	glm::vec3 nex_wind = wind_state[wind_next_state];

	float lerp_v = wind_cur_inter_time / wind_inter_time;
	now_wind = cur_wind + (nex_wind - cur_wind) * lerp_v;
}

void Weather::WeatherParam::UpdateWind(double dt_ms)
{
	wind_cur_inter_time += dt_ms;

	if (wind_cur_inter_time >= wind_inter_time) 
	{
		wind_cur_state = wind_next_state;
		wind_next_state = random() * (wind_state.size() - 1);
		wind_cur_inter_time = 0.0f;
		wind_inter_time = wind_min_time_interval + random() * (wind_max_time_interval - wind_min_time_interval);
	}

	LerpWind();
}

void Weather::WeatherParam::InitCloud(void)
{
	cloud_cur_state = 0;
	cloud_next_state = 1;
	cloud_gray_scale_now = 1.0f;
	cloud_gray_scale_next = 1.0f;
	cloud_min_time_interval = 5;//5;
	cloud_max_time_interval = 30;//30;
	cloud_cur_inter_time = 0.0;
	cloud_inter_time = cloud_min_time_interval + random() * (cloud_max_time_interval - cloud_min_time_interval);

	cloud_state.push_back(1.0f);

	for (register uint32_t i = 0; i < 40; i++)
	{
		cloud_state.push_back(random());
	}
}

void Weather::WeatherParam::UpdateCloud(double dt_s)
{
	cloud_cur_inter_time += dt_s;

	if (cloud_cur_inter_time >= cloud_inter_time)
	{
		cloud_cur_state = cloud_next_state;
		if(!rain_comming) cloud_next_state = random() * (cloud_state.size() - 1);
		else cloud_next_state = 0;
		cloud_gray_scale_now = cloud_gray_scale_next;
		cloud_gray_scale_next = glm::clamp((float)random() + 0.3f, 0.0f, 1.0f);
		cloud_cur_inter_time = 0.0f;
		cloud_inter_time = cloud_min_time_interval + random() * (cloud_max_time_interval - cloud_min_time_interval);
	}

	LerpCloud();
}

void Weather::WeatherParam::LerpCloud(void)
{
	float cloud_cur = cloud_state[cloud_cur_state];
	float cloud_next = cloud_state[cloud_next_state];

	float lerp_v = cloud_cur_inter_time / cloud_inter_time;
	cloud_now = cloud_cur + (cloud_next - cloud_cur) * lerp_v;

	cloud_gray_scale = cloud_gray_scale_now + (cloud_gray_scale_next - cloud_gray_scale_now) * lerp_v;
	cloud_gray_scale = glm::clamp(glm::pow(glm::clamp((cloud_gray_scale + 0.1f) / (cloud_now + 0.1f), 0.0f, 1.0f), 0.8f), 0.1f, 1.0f);
}

void Weather::WeatherParam::InitRain(void)
{
	/*//при достижении cloud_now - 0.8f (вероятность начала дождя - 0.001), при достижении 0.98(вероятность начала дождя - 0.9) при 1.0(вероятность начала дождя - 1.0)
		//если дождь пошел то следующее состояние(cloud_state) 0 на время пока идет дождь 
		double rain_min_time_interval, rain_max_time_interval;//cloud_max_time_interval - не меньше 2 минут
		double rain_cur_inter_time, rain_inter_time;
		bool rain_comming;//пошел ли дождь
		std::vector<float> rain_state;//плотность частиц дождя, 0 состаяние 0 частиц
		float rain_now;//при rain_comming == false => = 0.0f*/
	rain_comming = false;
	wait_time = 0.0f;
	rain_now = 0.0f;
	rain_min_time_interval = 20;//5;
	rain_max_time_interval = 400;//30;
	rain_cur_inter_time = 0.0;
	rain_inter_time = rain_min_time_interval + random() * (rain_max_time_interval - rain_min_time_interval);
}

void Weather::WeatherParam::UpdateRain(double dt_s)
{
	rain_cur_inter_time += dt_s;

	if (rain_comming) 
	{
		if (rain_cur_inter_time >= rain_inter_time)
		{
			rain_comming = false;
		//	float coeff = glm::clamp(cloud_now - 0.8f, 0.0f, 0.2f) / 0.2f;
			//rain_comming = (random(sqrt(coeff))*random(cloud_gray_scale));
			rain_cur_inter_time = 0.0f;
			wait_time = rain_min_time_interval + random() * (rain_max_time_interval - rain_min_time_interval);
		}
	}
	else 
	{
		if (rain_cur_inter_time >= wait_time)
		{
			float coeff = glm::clamp(cloud_now - 0.8f, 0.0f, 0.2f) / 0.2f;
			rain_comming = (random(sqrt(coeff))*random(cloud_gray_scale));

			if (rain_comming)
			{
				rain_cur_inter_time = 0.0f;
				rain_inter_time = rain_min_time_interval + random() * (rain_max_time_interval - rain_min_time_interval);
				rain_current = 0.0f;//glm::clamp((float)random()*0.01f + 0.1f, 0.0f, 1.0f);
				rain_next = 0.8f;// glm::clamp((float)random()*20.0f + 0.3f + rain_current, 0.0f, 1.0f);
			}
		}
	}

	if(rain_comming)LerpRain();
}

void Weather::WeatherParam::LerpRain(void) 
{
	float lerp_v = (2.0f*rain_cur_inter_time) / rain_inter_time;
	if(lerp_v < 1.0f)rain_now = rain_current + (rain_next - rain_current) * lerp_v;
	else rain_now = rain_current + (rain_next - rain_current) * (2.0f - lerp_v);
}

void Weather::WeatherParam::Update(double dt_ms)
{
	double ds = dt_ms * 0.001;
	UpdateWind(ds);
	UpdateCloud(ds);
	UpdateRain(ds);
}

float Weather::WeatherParam::GetCloudDensity(void)
{
	return cloud_now;
}

float Weather::WeatherParam::GetCloudGrayScale(void)
{
	return cloud_gray_scale;
}

float Weather::WeatherParam::GetRainIntensity(void)
{
	if (rain_comming)return rain_now;
	else return 0.0f;
}

glm::vec3 Weather::WeatherParam::GetWind(void)
{
	return now_wind*meter;
	//glm::vec3(0.2f, -0.5f, 0.0f)*meter;//
	//если не умножать на meter и использовать тот же подход что и для рисования дожня то получаеться неплохой резальтат для частиц пыли в воздухе
}