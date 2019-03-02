#include "LightInit.h"


ComputeTileShading::LightInit::LightInit(LightManager *light_data)
{
	Init(light_data);
}

ComputeTileShading::LightInit::~LightInit()
{
}

float MRand(void) 
{
	return (float(rand()) / float(RAND_MAX));
}

void ComputeTileShading::LightInit::Init(LightManager *light_data) 
{
	light_data->AddPointLight(glm::vec4(-620.0f, 136.0f, 218.0f, 450.0f), glm::vec4(1.0f, 0.5f, 0.0f, 0.4f));
	light_data->AddPointLight(glm::vec4(-620.0f, 136.0f, -140.0f, 450.0f), glm::vec4(1.0f, 0.5f, 0.0f, 0.4f));
	light_data->AddPointLight(glm::vec4(490.0f, 136.0f, 218.0f, 450.0f), glm::vec4(1.0f, 0.5f, 0.0f, 0.4f));
	light_data->AddPointLight(glm::vec4(490.0f, 136.0f, -140.0f, 450.0f), glm::vec4(1.0f, 0.5f, 0.0f, 0.4f));

	light_data->AddPointLight(glm::vec4(-1280.0f, 120.0f, -300.0f, 500.0f), glm::vec4(1.0f, 0.5f, 0.70f, 0.4f));
	light_data->AddPointLight(glm::vec4(-1280.0f, 200.0f, 430.0f, 600.0f), glm::vec4(0.8f, 0.5f, 0.0f, 0.4f));
	light_data->AddPointLight(glm::vec4(1030.0f, 200.0f, 545.0f, 500.0f), glm::vec4(0.4f, 0.5f, 0.60f, 0.4f));
	light_data->AddPointLight(glm::vec4(1180.0f, 220.0f, -390.0f, 500.0f), glm::vec4(0.0f, 0.5f, 0.20f, 0.4f));

	light_data->AddPointLight(glm::vec4(-65.0f, 100.0f, 220.0f, 500.0f), glm::vec4(1.0f, 1.0f, 1.0f, 0.4f));
	light_data->AddPointLight(glm::vec4(-65.0f, 100.0f, -140.0f, 500.0f), glm::vec4(1.0f, 1.0f, 1.0f, 0.4f));
	light_data->AddPointLight(glm::vec4(600.0f, 660.0f, -30.0f, 800.0f), glm::vec4(0.5f, 0.5f, 0.5f, 0.4f));
	light_data->AddPointLight(glm::vec4(-700.0f, 660.0f, 80.0f, 800.0f), glm::vec4(0.5f, 0.5f, 0.5f, 0.4f));

	///////////////////////////////////////////////////////////////////////////////////////////
	light_data->SetPointLightToBuffer();
}

void ComputeTileShading::LightInit::SInit(LightManager *light_data)
{
	const float mPointLightPower = 1.2f;
	const float mSpotLightPower = 1.1f;

	light_data->AddPointLight(glm::vec4(-620.0f, 136.0f, 218.0f, 450.0f), glm::vec4(0.0f, 0.5f, 0.0f, 4.0f) * mPointLightPower);
	light_data->AddPointLight(glm::vec4(-620.0f, 136.0f, -140.0f, 450.0f), glm::vec4(0.2f, 0.5f, 0.2f, 4.0f) * mPointLightPower);
	light_data->AddPointLight(glm::vec4(490.0f, 136.0f, 218.0f, 450.0f), glm::vec4(0.0f, 0.5f, 0.8f, 4.0f) * mPointLightPower);
	light_data->AddPointLight(glm::vec4(490.0f, 136.0f, -140.0f, 450.0f), glm::vec4(0.8f, 0.5f, 0.0f, 4.0f) * mPointLightPower);

	light_data->AddPointLight(glm::vec4(-1280.0f, 120.0f, -300.0f, 500.0f), glm::vec4(0.0f, 0.5f, 0.70f, 4.0f) * mPointLightPower);
	light_data->AddPointLight(glm::vec4(-1280.0f, 200.0f, 430.0f, 600.0f), glm::vec4(0.8f, 0.5f, 0.0f, 4.0f) * mPointLightPower);
	light_data->AddPointLight(glm::vec4(1030.0f, 200.0f, 545.0f, 500.0f), glm::vec4(0.4f, 0.5f, 0.60f, 4.0f) * mPointLightPower);
	light_data->AddPointLight(glm::vec4(1180.0f, 220.0f, -390.0f, 500.0f), glm::vec4(0.0f, 0.5f, 0.20f, 4.0f) * mPointLightPower);

	light_data->AddPointLight(glm::vec4(-65.0f, 100.0f, 220.0f, 500.0f), glm::vec4(1.0f, 1.0f, 1.0f, 4.0f) * mPointLightPower);
	light_data->AddPointLight(glm::vec4(-65.0f, 100.0f, -140.0f, 500.0f), glm::vec4(1.0f, 1.0f, 1.0f, 4.0f) * mPointLightPower);
	light_data->AddPointLight(glm::vec4(600.0f, 660.0f, -30.0f, 800.0f), glm::vec4(0.5f, 0.5f, 0.8f, 4.0f) * mPointLightPower);
	light_data->AddPointLight(glm::vec4(-700.0f, 660.0f, 80.0f, 800.0f), glm::vec4(0.8f, 0.5f, 0.5f, 4.0f) * mPointLightPower);

	///////////////////////////////////////////////////////////////////////////////////////////
	light_data->SetPointLightToBuffer();
	///////////////////////////////////////////////////////////////////////////////////////////

	float vAngles[12];
	glm::vec4 color[12];
	for (register uint32_t i = 0; i < 12; i++) 
	{
		vAngles[i] = 0.785f;//((float(rand()) / float(RAND_MAX))* 0.8f + 0.2f) * glm::pi<float>() * 0.45f;
		color[i] = glm::vec4(MRand(), MRand(), MRand(), 0.4f);
	}

	light_data->AddSpotLight(glm::vec4(-772.0f, 254.0f, -503.0f, 800.0f), glm::vec3(-814.0f, 180.0f, -250.0f), vAngles[0], glm::vec4(1.0f, 1.0f, 1.0f, 0.041f) * mSpotLightPower);
	light_data->AddSpotLight(glm::vec4(1130.0f, 378.0f, 40.0f, 500.0f), glm::vec3(1150.0f, 290.0f, 40.0f), vAngles[1], glm::vec4(0.784f, 0.784f, 0.392f, 0.041f) * mSpotLightPower);
	light_data->AddSpotLight(glm::vec4(-1260.0f, 378.0f, 40.0f, 500.0f), glm::vec3(-1280.0f, 290.0f, 40.0f), vAngles[2], glm::vec4(0.784f, 0.784f, 0.392f, 0.041f) * mSpotLightPower);
	light_data->AddSpotLight(glm::vec4(-115.0f, 660.0f, -100.0f, 800.0f), glm::vec3(-115.0f, 630.0f, 0.0f), vAngles[3], glm::vec4(0.784f, 0.484f, 0.784f, 0.041f) * mSpotLightPower);

	light_data->AddSpotLight(glm::vec4(-115.0f, 660.0f, 100.0f, 800.0f), glm::vec3(-115.0f, 630.0f, -100.0f), vAngles[4], glm::vec4(0.184f, 0.784f, 0.584f, 0.041f) * mSpotLightPower);
	light_data->AddSpotLight(glm::vec4(-770.0f, 660.0f, -100.0f, 800.0f), glm::vec3(-770.0f, 630.0f, 0.0f), vAngles[5], glm::vec4(0.384f, 0.684f, 0.784f, 0.041f) * mSpotLightPower);
	light_data->AddSpotLight(glm::vec4(-770.0f, 660.0f, 100.0f, 800.0f), glm::vec3(-770.0f, 630.0f, -100.0f), vAngles[6], glm::vec4(0.784f, 0.784f, 0.784f, 0.041f) * mSpotLightPower);
	light_data->AddSpotLight(glm::vec4(500.0f, 660.0f, -100.0f, 800.0f), glm::vec3(600.0f, 630.0f, -100.0f), vAngles[7], glm::vec4(0.084f, 0.084f, 0.784f, 0.041f) * mSpotLightPower);

	light_data->AddSpotLight(glm::vec4(500.0f, 660.0f, 100.0f, 800.0f), glm::vec3(-1240.0f, 140.0f, -405.0f), vAngles[8], glm::vec4(0.784f, 0.0f, 0.0f, 0.041f) * mSpotLightPower);
	light_data->AddSpotLight(glm::vec4(-1240.0f, 90.0f, -70.0f, 700.0f), glm::vec3(-1240.0f, 140.0f, -405.0f), vAngles[9], glm::vec4(1.0f, 0.0f, 0.0f, 0.041f) * mSpotLightPower);
	light_data->AddSpotLight(glm::vec4(-1000.0f, 90.0f, -260.0f, 700.0f), glm::vec3(-814.0f, 180.0f, -250.0f), vAngles[10], glm::vec4(1.0f, 1.0f, 1.0f, 0.041f) * mSpotLightPower);
	light_data->AddSpotLight(glm::vec4(-900.0f, 60.0f, 340.0f, 700.0f), glm::vec3(500.0f, 630.0f, 0.0f), vAngles[11], glm::vec4(0.9392f, 0.392f, 0.9392f, 0.041f) * mSpotLightPower);

	///////////////////////////////////////////////////////////////////////////////////////////
	light_data->SetSpotLightToBuffer();
}
