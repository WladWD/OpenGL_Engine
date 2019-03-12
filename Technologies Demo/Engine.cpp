#include "Engine.h"

MEngine::Engine::Engine(Time::Timer_Query *timer_, MCamera::Camera *camera_) : timer(timer_), camera(camera_)
{
	srand(time(NULL));
	SetUpOpenGLParameter();

	weather_param = new Weather::WeatherParam(camera_->GetMeter());

	MInputParam param;
	ReadInputFile(param);

	LoadModel *model_load = new LoadModel(param.input_file);
	//"C:\\Users\\User\\Desktop\\debug\\sponz.obj");//

	//forward_draw = new ForwardRendering::ForwardDraw(model_load->GetResource(), model_load->GetVNTMesh(), model_load->GetVNTTMesh(), camera_);

	mDrawDecale = new Decale::DrawDecale(new Resource::LoadTexture(model_load->GetResource()->texture_map), "resource\\decale\\decale_.tga");

	light_manager = new ComputeTileShading::LightManager();
	ComputeTileShading::LightInit::SInit(light_manager);

	draw_g_buffer = new DeferredRendering::DrawGBuffer(camera_, model_load->GetResource(), model_load->GetVNTMesh(), model_load->GetObjectListVNT(),
		model_load->GetVNTTMesh(), model_load->GetObjectListVNTT(), camera_->GetHeight(), camera_->GetWeight());
	atmosphere_parameter = new Atmosphere::AtmosphereParameter();

	render_resources = new RenderResource::RenderResources<2, 1>(camera->GetWeight(), camera->GetHeight());
	draw_sky = new Atmosphere::DrawSky(atmosphere_parameter, model_load->GetResource()->texture_map);
	draw_grass = new Atmosphere::DrawGrass(camera, model_load->GetResource()->texture_map);
	draw_cloud = new Atmosphere::DrawCloud();
	draw_particles = new Particle::DrawParticles(camera, model_load->GetResource()->texture_map);

	light_compute = new ComputeTileShading::LightCompute(light_manager, camera, render_resources, draw_g_buffer->GetBufferData());

	mFullScreenQuad = new Draw::FullScreenQuad();
	mDrawSSR = new Draw::DrawSSR(mFullScreenQuad, camera);
	mFinalFrame = new Render::FinalFrame(draw_g_buffer->GetBufferData().g_buffer_depth, camera_->GetWeight(), camera_->GetHeight());
	drawSSAO = std::make_unique<SSAO::DrawSSAO>(camera);

	delete model_load;
}

MEngine::Engine::~Engine() 
{
	//delete forward_draw;
	delete draw_g_buffer;
	delete atmosphere_parameter;
	delete light_compute;
	delete render_resources;
	delete light_manager;

	delete draw_sky;
	delete draw_grass;
	delete draw_cloud;
	delete draw_particles;
	delete weather_param;

	delete mFullScreenQuad;
	delete mDrawSSR;
	delete mFinalFrame;
	delete mDrawDecale;
}

void MEngine::Engine::ReadInputFile(MInputParam &param)
{
	std::ifstream data_file("resource\\parameters.txt");
	if (data_file) {
		data_file >> param.input_file;
		data_file.close();
	}
	else
	{
		MessageBox(NULL, L"�� ������ ���� � �����������", L"", MB_OK);
		throw 1;
	}
}

void MEngine::Engine::SetUpOpenGLParameter(void)
{
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_TEXTURE_3D);
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
}

void MEngine::Engine::DrawSpotLightShadow(void)
{
	const glm::mat4 *matrix;
	Shader::ShaderSpotLightShadow *smShader;
	uint32_t count;

	light_manager->DrawSpotLightShadow(matrix, smShader, count);

	draw_g_buffer->DrawSpotShadow(matrix, count, smShader);

	light_manager->EndDrawShadowSpot();
}

void MEngine::Engine::DrawPointLightShadow(void)
{
	const glm::mat4 *matrix;
	Shader::ShaderPointLightShadow *smShader;
	uint32_t count;

	light_manager->DrawPointLightShadow(matrix, smShader, count);

	draw_g_buffer->DrawShadow(matrix, count, smShader);

	light_manager->EndDrawShadowPoint();
}

void MEngine::Engine::DrawShadow(void)
{
	DrawSpotLightShadow();
	DrawPointLightShadow();
}

void MEngine::Engine::Draw(float delta_ms)
{
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	atmosphere_parameter->UpdateTime(delta_ms);
	weather_param->Update(delta_ms);
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);//������������� ������ ������ �� ��������(�������� ������)
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);//������������� ������ ��������� �� ��������(�������� ������)

	//glReadBuffer(GL_NONE);//������������� ������ ������, ��� �� �� �������������
	glDrawBuffer(GL_BACK);//������������� ������ ��������� �� ��������(�������� ������� ������ ������)

	while (glGetError() == GL_INVALID_OPERATION);
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClearDepthf(1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//Shadow
	static bool one_draw_shadow = true;
	if(one_draw_shadow) DrawShadow();
	one_draw_shadow = false;
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//forward_draw->Draw(delta_ms, atmosphere_parameter->GetGlobalLightingParameter());
	draw_g_buffer->Draw();

	mDrawDecale->Draw(camera, draw_g_buffer->GetBufferData());

	glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	glDrawBuffer(GL_BACK);

	const DeferredRendering::GBufferData mdraw = draw_g_buffer->GetBufferData();
	drawSSAO->computeSSAO(mdraw.g_buffer_depth, mdraw.g_buffer_textures[0]);

	light_compute->Draw(atmosphere_parameter->GetGlobalLightingParameterMoon(), atmosphere_parameter->GetGlobalLightingParameter(), drawSSAO->getSSAOTexture());
	mFinalFrame->BingFramebuffer();

	mDrawSSR->DrawSSRData(mdraw.g_buffer_depth, render_resources->GetDiffuseTexture(),
		mdraw.g_buffer_depth, mdraw.g_buffer_textures[0], render_resources->GetDiffuseTexture(), render_resources->GetSpecularTexture(),
		camera->GetProjViewMatrix());
	draw_particles->Draw(weather_param->GetWind(),
		delta_ms / 1000.0l, &atmosphere_parameter->GetGlobalLightingParameter(), &atmosphere_parameter->GetGlobalLightingParameterMoon(), weather_param->GetRainIntensity());
	draw_sky->Draw(camera->GetProjViewMatrix(), camera->GetPosition(), &atmosphere_parameter->GetGlobalLightingParameter());
	draw_cloud->Draw(timer->GetTimeMilliSecond(), camera->GetProjViewMatrix(),
		&atmosphere_parameter->GetGlobalLightingParameter(), &atmosphere_parameter->GetGlobalLightingParameterMoon(),
		weather_param->GetCloudDensity(), weather_param->GetCloudGrayScale());

	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	glBindFramebuffer(GL_READ_FRAMEBUFFER, mFinalFrame->GetCurrentFramebuffer());
	glDrawBuffer(GL_BACK);
	glReadBuffer(GL_COLOR_ATTACHMENT0);
	glBlitFramebuffer(0, 0, camera->GetWeight(), camera->GetHeight(), 0, 0, camera->GetWeight(), camera->GetHeight(), GL_COLOR_BUFFER_BIT, GL_LINEAR);
	/*glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	glBindFramebuffer(GL_READ_FRAMEBUFFER, render_resources->GetFramebuffer());
	glDrawBuffer(GL_BACK);
	glReadBuffer(GL_COLOR_ATTACHMENT0);
	glBlitFramebuffer(0, 0, camera->GetWeight(), camera->GetHeight(), 0, 0, camera->GetWeight(), camera->GetHeight(), GL_COLOR_BUFFER_BIT, GL_LINEAR);*/

	/*glBindFramebuffer(GL_READ_FRAMEBUFFER, 1);
	glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, light_compute->GetFrameBuffer(), 0);*/

	/*//draw_sky->Draw(camera->GetProjViewMatrix(), camera->GetPosition(), &atmosphere_parameter->GetGlobalLightingParameter());
	//draw_grass->Draw(&atmosphere_parameter->GetGlobalLightingParameter());
	/*draw_cloud->Draw(timer->GetTimeMilliSecond(), camera->GetProjViewMatrix(),
	&atmosphere_parameter->GetGlobalLightingParameter(), &atmosphere_parameter->GetGlobalLightingParameterMoon(),
	weather_param->GetCloudDensity(), weather_param->GetCloudGrayScale());

	draw_particles->Draw(weather_param->GetWind(),
	delta_ms /1000.0l, &atmosphere_parameter->GetGlobalLightingParameter(), &atmosphere_parameter->GetGlobalLightingParameterMoon(), weather_param->GetRainIntensity());
	*/
}

void MEngine::Engine::Click(void)
{
	mDrawDecale->AppendDecale(camera);
}
//������� ����� ��� ��������� �� �������� ������������� ����� ��������� � ����� ����������
//��������� ��� ������������� ����� ��������� � ������� ����� ������� ����������� ���������
