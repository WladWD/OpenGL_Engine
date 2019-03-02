#include "AtmosphereParameter.h"


//30 мин -> 24 часа
//48 / 1000 -> так как интервал передаеться в ms
Atmosphere::AtmosphereParameter::AtmosphereParameter() : real_time_to_lacal_time((24.0f * 60) / 10000.0f),//30000.0f), 1800000.0f
	mull_const(2.0f * glm::pi<float>()), offset_const(((glm::pi<float>() * 149.0f) / 160.0f) + glm::pi<float>() * 0.006125f), mconst(1.0f/1440.0f)
{
	//offset_const(glm::pi<float>() * 0.0f + glm::pi<float>() * 0.45f) - закат
	//offset_const(glm::pi<float>() * 0.5f + glm::pi<float>() * 0.945f) - рассвет
	//offset_const(((glm::pi<float>() * 149.0f) / 160.0f) + glm::pi<float>() * 0.0125f) - 0 часов ночи
	//to debug 0.50125f
	light_param.light_diffuse_color = glm::vec4(1.0f, 1.0f, 1.0f, 0.6f);
	light_param.light_specurar_color = glm::vec4(0.8f, 0.8f, 0.8f, 1.0f);
	light_param.light_dir = glm::vec3(0.0f, -1.0f, 0.0f);

	InitScatteringParam();

	Phi = glm::pi<float>() * 0.5f;// 2.0f;
	Thetta = 0.0f;
	curret_time_minutes = 100; //1110.0f;//1135.0f;//0.0f;

	phi = glm::pi<float>() * 0.5f;
	thetta = 0.0f;

	UpdateTime(0.0f);

	glm::vec4 p = GetDiffuseColor(0, 1);
	int dbg = 0;
}

Atmosphere::AtmosphereParameter::~AtmosphereParameter()
{
}

void Atmosphere::AtmosphereParameter::UpdateTimet(float delta_ms)
{
	static const float twopi = glm::pi<float>() * 2.0f;
	static const float mpidiv2 = glm::pi<float>() * 0.5f;
	static const float m3pidiv2 = glm::pi<float>() * 0.5f * 3.0f;

	static const float dvup = glm::pi<float>() / 5.0f;
	static const float dvdown = glm::pi<float>() / 100.0f;
	static const float mс1 = (glm::pi<float>() * 0.5f) - dvup;
	static const float mс2 = (glm::pi<float>() * 0.5f) + dvdown;
	static const float mс3 = (glm::pi<float>() * 0.5f * 3.0f) + dvup;
	static const float mс4 = (glm::pi<float>() * 0.5f * 3.0f) - dvdown;
	static const float mс = (1.0f / (dvdown + dvup));// *glm::pi<float>() * 0.5f;


	float NPhi = phi, NThetta;

	curret_time_minutes += delta_ms * real_time_to_lacal_time;
	if (curret_time_minutes >= 1440.0f)curret_time_minutes = curret_time_minutes - 1440.0f;

	NThetta = curret_time_minutes * mconst * mull_const;

	//real angle = mpidiv2 - NThetta;

	if (NThetta != thetta || NPhi != phi) light_param.need_update_atmosphere = true;
	else light_param.need_update_atmosphere = false;

	thetta = NThetta;
	phi = NPhi;

	light_param.star_intensity = 1.0f;

	if (thetta < mс1 || thetta > mс3) light_param.star_intensity = 0.0f;
	if (thetta >= mс1 && thetta <= mс2)light_param.star_intensity = (float)glm::clamp(pow(((thetta - mс1) * mс), 0.80f), 0.0f, 1.0f);//закат солнца sin mс
	if (thetta >= mс4 && thetta <= mс3)light_param.star_intensity = (float)glm::clamp(pow(1.0f - ((thetta - mс4) * mс), 0.80f), 0.0f, 1.0f);//восход солнца cos mс 1.0f -
	//else light_param.star_intensity = 1.0f;


	light_param.light_dir = -GetLightDirection(phi, thetta);
	light_param.light_diffuse_color = GetDiffuseColor(thetta, 4);// *  pow(glm::clamp(-light_param.light_dir.y + 0.2f, 0.0f, 1.0f), 0.68f);//glm::vec4(0.77f) * pow(glm::clamp(-light_param.light_dir.y + 0.2f, 0.0f, 1.0f), 0.68f);//pow(1.0f - light_param.star_intensity, 0.089f);//GetDiffuseColor(thetta, 4);// *(1.0f - light_param.star_intensity);//GetLihgtDiffuseColor(2);
	light_param.light_specurar_color = light_param.light_diffuse_color * 0.8f;
	light_param.Phi = phi;
	light_param.Thetta = mpidiv2 - thetta;



	//NThetta = (thetta > mpidiv2 && thetta > m3pidiv2) ? : ;
	//if (thetta > mpidiv2 && thetta < glm::pi<float>())NThetta -= mpidiv2;
	//else if(thetta > glm::pi<float>() && thetta < m3pidiv2)NThetta += mpidiv2;

	light_param_moon.light_dir = -light_param.light_dir;//GetLightDirection(phi, thetta);
	light_param_moon.light_diffuse_color = glm::vec4(0.47f) * pow(glm::clamp(-light_param_moon.light_dir.y + 0.1f, 0.0f, 1.0f), 0.8f)*0.0f;//pow(light_param.star_intensity, 0.89f);////GetLihgtDiffuseColor(NThetta, 1);//*light_param.star_intensity;
	light_param_moon.light_diffuse_color.a = 0.4f;
	light_param_moon.light_specurar_color = light_param_moon.light_diffuse_color * 0.6f;
}

void Atmosphere::AtmosphereParameter::UpdateTime(float delta_ms)
{
	//UpdateTimet(delta_ms);
	//return;

	static const float twopi = glm::pi<float>() * 2.0f;
	static const float mpidiv2 = glm::pi<float>() * 0.5f;
	static const float m3pidiv2 = glm::pi<float>() * 0.5f * 3.0f;

	static const float dvup = glm::pi<float>() / 80.0f;
	static const float dvdown = glm::pi<float>() / 8.0f;
	static const float mс1 = (glm::pi<float>() * 0.5f) - dvup;
	static const float mс2 = (glm::pi<float>() * 0.5f) + dvdown - dvup;
	static const float mс3 = (glm::pi<float>() * 0.5f * 3.0f) + dvup;
	static const float mс4 = (glm::pi<float>() * 0.5f * 3.0f) - dvdown;
	static const float mс = (glm::pi<float>() * 0.5f) / (dvdown + dvup);

	float NPhi = Phi, NThetta;

	//curret_time_minutes += delta_ms * real_time_to_lacal_time;

	if (curret_time_minutes >= 1440.0f)curret_time_minutes = curret_time_minutes - 1440.0f;

	NThetta = curret_time_minutes * mconst * mull_const;// +offset_const;

	if (NThetta != Thetta || NPhi != Phi) light_param.need_update_atmosphere = true;
	else light_param.need_update_atmosphere = false;

	NThetta = NThetta >= twopi ? NThetta - twopi : NThetta;

	Thetta = NThetta;
	Phi = NPhi;

	if ( Thetta < mс1 || Thetta > mс3 ) light_param.star_intensity = 0.0f;
	else if(Thetta <= mс2)light_param.star_intensity = (float)pow(sin((Thetta - mс1) * mс), 8.0f);//закат солнца
	else if(Thetta >= mс4)light_param.star_intensity = (float)(1.0f - sin((Thetta - mс4) * mс));//восход солнца
	else light_param.star_intensity = 1.0f;

	light_param.light_dir = GetLightDirection(Phi, Thetta);
	light_param.light_diffuse_color = GetLihgtDiffuseColor(2);
	light_param.light_specurar_color = light_param.light_diffuse_color * 0.8f;
	light_param.Phi = Phi;
	light_param.Thetta = Thetta;



	NThetta += 2.0f * mpidiv2;
	NThetta = NThetta >= twopi ? NThetta - twopi : NThetta;

	/*float tmp = Thetta;
	Thetta = (glm::pi<float>() + tmp);
	if (Thetta >= m3pidiv2) Thetta -= m3pidiv2;*/

	light_param_moon.light_dir = GetLightDirection(Phi, NThetta);
	light_param_moon.light_diffuse_color = glm::pow(GetLihgtDiffuseColor(NThetta, 1), glm::vec4(2.6f))*light_param.star_intensity;
	light_param_moon.light_diffuse_color.a = 0.4f;
	light_param_moon.light_specurar_color = light_param_moon.light_diffuse_color * 0.8f;

	//Thetta = tmp;
}

glm::vec3 Atmosphere::AtmosphereParameter::GetLightDirection(float fPhi, float fThetta)
{
	float y = (float)cos((double)fThetta);
	float x = (float)(sin((double)fThetta) * cos(fPhi));
	float z = (float)(sin((double)fThetta) * sin(fPhi));
	float w = 1.0f;

	return -normalize(glm::vec3(x, y, z));
}

void Atmosphere::AtmosphereParameter::InitScatteringParam(void)
{
	scattering_param.waveLengths = glm::vec3(0.65f, 0.57f, 0.475f);

	scattering_param.invWaveLengths.x = 1.0f / (float)pow((double)scattering_param.waveLengths.x, 4.0);
	scattering_param.invWaveLengths.y = 1.0f / (float)pow((double)scattering_param.waveLengths.y, 4.0);
	scattering_param.invWaveLengths.z = 1.0f / (float)pow((double)scattering_param.waveLengths.z, 4.0);

	scattering_param.waveLengthsMie.x = (float)pow((double)scattering_param.waveLengths.x, -0.84);
	scattering_param.waveLengthsMie.y = (float)pow((double)scattering_param.waveLengths.y, -0.84);
	scattering_param.waveLengthsMie.z = (float)pow((double)scattering_param.waveLengths.z, -0.84);
}

const Atmosphere::ScatteringParam *Atmosphere::AtmosphereParameter::GetScatteringParameter(void)
{
	return &scattering_param;
}

glm::vec4 Atmosphere::AtmosphereParameter::GetDiffuseColor(float fThetta, int nTurbidity)
{
	fThetta = -thetta - glm::pi<float>();//-glm::pi<float>()*0.5f - glm::pi<float>();//2.0f* glm::pi<float>();

	float fBeta = 0.04608365822050f * nTurbidity - 0.04586025928522f;
	float fTauR, fTauA;
	float fTau[3];

	/*            float coseno = (float)Math.Cos((double)fTheta + Math.PI);
            double factor = (double)fTheta / Math.PI * 180.0;
            double jarl = Math.Pow(93.885 - factor, -1.253);
            float potencia = (float)jarl;
            float m = 1.0f / (coseno + 0.15f * potencia);*/

	float coseno = (float)cos((double)fThetta + glm::pi<float>());
	double factor = ((double)fThetta / glm::pi<double>()) * 180.0;
	if (93.885 - factor > 0.0f) 
	{
		double jarl = std::pow((93.885 - factor), -1.253);//abs
		float potencia = (float)jarl;
		float m = 1.0f / (coseno + 0.15f * potencia);

		int i;
		float fLambda[3];
		fLambda[0] = scattering_param.waveLengths.x;
		fLambda[1] = scattering_param.waveLengths.y;
		fLambda[2] = scattering_param.waveLengths.z;


		for (i = 0; i < 3; i++)
		{
			potencia = (float)pow((double)fLambda[i], 4.0);
			fTauR = (float)exp((double)(-m * 0.008735f * potencia));

			const float fAlpha = 1.3f;
			potencia = (float)pow((double)fLambda[i], (double)-fAlpha);
			if (m < 0.0f)
				fTau[i] = 0.0f;
			else
			{
				fTauA = (float)exp((double)(-m * fBeta * potencia));
				fTau[i] = fTauR * fTauA;
			}

		}

		glm::vec4 vAttenuation = glm::vec4(fTau[0], fTau[1], fTau[2], 0.7f);
		return vAttenuation;
	}
	else return glm::vec4(0.0f, 0.0f, 0.0f, 0.7f);
}

glm::vec4 Atmosphere::AtmosphereParameter::GetLihgtDiffuseColor(int nTurbidity)
{
	float fBeta = 0.04608365822050f * nTurbidity - 0.04586025928522f;
	float fTauR, fTauA;
	float fTau[3];

	double fThetta;// = Thetta;// -offset_const;//0.5f * Thetta - glm::pi<double>() * 0.5f;

	static const float mpidiv2 = glm::pi<float>() * 0.5f;
	static const float m3pidiv2 = glm::pi<float>() * 0.5f * 3.0f;

	if (Thetta <= mpidiv2 ) fThetta = Thetta;//4.0f * mpidiv2 - Thetta;
	else if (Thetta >= m3pidiv2) fThetta = 4.0f * mpidiv2 - Thetta;//Thetta;//
	else fThetta = 2.0f*mpidiv2;//4.0f * mpidiv2 - Thetta;

	/*fThetta = 0.0;//max
	fThetta = mpidiv2;//min
	*/

	float coseno = (float)cos((double)fThetta);// +glm::pi<double>());
	double factor = ((double)fThetta / glm::pi<double>()) * 180.0;
	double jarl = std::pow(abs(93.885 - factor), -1.253);
	float potencia = (float)jarl;
	float m = 1.0f / (coseno + 0.15f * potencia);

	int i;
	float fLambda[3];
	fLambda[0] = scattering_param.waveLengths.x;
	fLambda[1] = scattering_param.waveLengths.y;
	fLambda[2] = scattering_param.waveLengths.z;


	for (i = 0; i < 3; i++)
	{
		potencia = (float)pow((double)fLambda[i], 4.0);
		fTauR = (float)exp((double)(-m * 0.008735f * potencia));

		const float fAlpha = 1.3f;
		potencia = (float)pow((double)fLambda[i], (double)-fAlpha);
		if (m < 0.0f)
			fTau[i] = 0.0f;
		else
		{
			fTauA = (float)exp((double)(-m * fBeta * potencia));
			fTau[i] = fTauR * fTauA;
		}

	}

	glm::vec4 vAttenuation = glm::vec4(fTau[0], fTau[1], fTau[2], 0.7f);
	return vAttenuation;
}

glm::vec4 Atmosphere::AtmosphereParameter::GetLihgtDiffuseColor(float fMThetta, int nTurbidity)
{
	float fBeta = 0.04608365822050f * nTurbidity - 0.04586025928522f;
	float fTauR, fTauA;
	float fTau[3];

	double fThetta;// = Thetta;// -offset_const;//0.5f * Thetta - glm::pi<double>() * 0.5f;

	static const float mpidiv2 = glm::pi<float>() * 0.5f;
	static const float m3pidiv2 = glm::pi<float>() * 0.5f * 3.0f;

	if (fMThetta <= mpidiv2) fThetta = fMThetta;//4.0f * mpidiv2 - Thetta;
	else if (fMThetta >= m3pidiv2) fThetta = 4.0f * mpidiv2 - fMThetta;//Thetta;//
	else fThetta = mpidiv2;//4.0f * mpidiv2 - Thetta;

						   /*fThetta = 0.0;//max
						   fThetta = mpidiv2;//min
						   */

	float coseno = (float)cos((double)fThetta);// +glm::pi<double>());
	double factor = ((double)fThetta / glm::pi<double>()) * 180.0;
	double jarl = std::pow(abs(93.885 - factor), -1.253);
	float potencia = (float)jarl;
	float m = 1.0f / (coseno + 0.15f * potencia);

	int i;
	float fLambda[3];
	fLambda[0] = scattering_param.waveLengths.x;
	fLambda[1] = scattering_param.waveLengths.y;
	fLambda[2] = scattering_param.waveLengths.z;


	for (i = 0; i < 3; i++)
	{
		potencia = (float)pow((double)fLambda[i], 4.0);
		fTauR = (float)exp((double)(-m * 0.008735f * potencia));

		const float fAlpha = 1.3f;
		potencia = (float)pow((double)fLambda[i], (double)-fAlpha);
		if (m < 0.0f)
			fTau[i] = 0.0f;
		else
		{
			fTauA = (float)exp((double)(-m * fBeta * potencia));
			fTau[i] = fTauR * fTauA;
		}

	}

	glm::vec4 vAttenuation = glm::vec4(fTau[0], fTau[1], fTau[2], 0.7f);
	return vAttenuation;
}

const Atmosphere::MoonLightingParam &Atmosphere::AtmosphereParameter::GetGlobalLightingParameterMoon(void)
{
	return light_param_moon;
}

const Atmosphere::GlobalLightingParam &Atmosphere::AtmosphereParameter::GetGlobalLightingParameter(void)
{
	return light_param;
}