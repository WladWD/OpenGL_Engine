#include "MTimer.h"
#include "MCamera.h"
#include "LoadModel.h"
//#include "ForwardDraw.h"
#include "DrawGBuffer.h"
#include "DrawSky.h"
#include "DrawGrass.h"
#include "DrawCloud.h"
#include "DrawParticles.h"
#include <time.h>
#include <fstream>
#include "WeatherParam.h"
#include "LightCompute.h"
#include "DrawSSR.h"
#include "FinalFramebuffer.h"
#include "DrawDecale.h"
#include "DrawSSAO.h"

#pragma once
namespace MEngine
{
	struct MInputParam 
	{
		char input_file[281];
	};

	class Engine
	{
		Time::Timer_Query *timer;
		MCamera::Camera *camera;
		///////////////////////////////////////////////
		Atmosphere::AtmosphereParameter *atmosphere_parameter;
		Weather::WeatherParam *weather_param;
		//ForwardRendering::ForwardDraw *forward_draw;
		DeferredRendering::DrawGBuffer *draw_g_buffer;
		ComputeTileShading::LightCompute *light_compute;
		RenderResource::RenderResources<2, 1> *render_resources;

		ComputeTileShading::LightManager *light_manager;
		//ComputeTileShading::LightInit *light
		Render::FinalFrame *mFinalFrame;
		///////////////////////////////////////////////
		Atmosphere::DrawGrass *draw_grass;
		///////////////////////////////////////////////
		Atmosphere::DrawSky *draw_sky;
		Atmosphere::DrawCloud *draw_cloud;
		Particle::DrawParticles *draw_particles;
		///////////////////////////////////////////////
		Draw::FullScreenQuad *mFullScreenQuad;
		Draw::DrawSSR *mDrawSSR;
		std::unique_ptr<SSAO::DrawSSAO> drawSSAO;
		///////////////////////////////////////////////
		Decale::DrawDecale *mDrawDecale;
		///////////////////////////////////////////////
		void ReadInputFile(MInputParam &param);
		void SetUpOpenGLParameter(void);
		///////////////////////////////////////////////
		void DrawSpotLightShadow(void);
		void DrawPointLightShadow(void);
		///////////////////////////////////////////////
		void DrawShadow(void);
	public:
		Engine(Time::Timer_Query *timer_, MCamera::Camera *camera_);
		~Engine();

		void Draw(float delta_ms);
		void Click(void);
	};
}

//http://stackoverflow.com/questions/462721/rendering-to-cube-map
//http://gamedev.stackexchange.com/questions/123631/glviewport-and-single-pass-cubemap-rendering
//https://www.opengl.org/discussion_boards/showthread.php/155441-Single-pass-render-to-cubemap
//http://steps3d.narod.ru/tutorials/texture-swizzle-tutorial.html