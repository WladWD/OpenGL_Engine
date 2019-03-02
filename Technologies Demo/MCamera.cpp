#include "MCamera.h"

MCamera::Camera::Camera(float near_clip_plane, float far_clip_plane, unsigned long width, unsigned long height, float aspect_angle, float meter, float speed) :
	aspect(aspect_angle), width(width), height(height), speed(speed / 1000.0f), meter(meter), far_plane(far_clip_plane), near_plane(near_clip_plane), pix_to_rad(0.0015f)
{
	project = //MPerspective(near_clip_plane * meter, far_clip_plane * meter, aspect, 0.8f);
	glm::perspective(0.8f, aspect, near_clip_plane * meter, far_clip_plane * meter);
	view = glm::mat4(1.0f);
	
	/*glm::mat4 iproj = glm::inverse(project);
	glm::vec4 tv = iproj * glm::vec4(0.0f, 0.0,  0.0f, 1.0f);
	tv.z /= tv.w;

	float z = 0.0f;

	z = iproj[3][2] / (iproj[2][3] * z + iproj[3][3]);
	*/
		/*
	float p = iproj[0][2];*/


	//project[1][1] = -project[1][1];
	//project[2][3] = -project[2][3];
	//project[2][2] = -project[2][2];

	/*Result[2][2] = - (zFar + zNear) / (zFar - zNear);
		Result[2][3] = - static_cast<T>(1);
		Result[3][2] = - (static_cast<T>(2) * zFar * zNear) / (zFar - zNear);*/

	y_angle_quater = x_angle_quater = 0;

	need_update = false;

	position = vec3(0.0f, 0.0f, 0.0f);

	i = vec3(1.0f, 0.0f, 0.0f);
	j = vec3(0.0f, 1.0f, 0.0f);
	k = vec3(0.0f, 0.0f, 1.0f);

	ProjectView = project * view;

	mouseDX = mouseDY = 0;

	dx = dy = dz = 0.0f;
}

MCamera::Camera::~Camera() 
{

}

glm::mat4 MCamera::Camera::MPerspective(float zNear_, float zFar_, float m_ar, float fovY)
{
	const float ar = m_ar;
	const float zNear = zNear_;
	const float zFar = zFar_;
	const float zRange = zNear - zFar;
	const float tanHalfFOV = tanf(fovY * 0.5f);

	glm::mat4 m;

	m[0].x = 1.0f / (tanHalfFOV * ar);
	m[0].y = 0.0f;
	m[0].z = 0.0f;
	m[0].w = 0.0f;

	m[1].x = 0.0f;
	m[1].y = 1.0f / tanHalfFOV;
	m[1].z = 0.0f;
	m[1].w = 0.0f;

	m[2].x = 0.0f;
	m[2].y = 0.0f;
	m[2].z = (-zNear - zFar) / zRange;
	m[2].w = 2.0f * zFar * zNear / zRange;

	m[3].x = 0.0f;
	m[3].y = 0.0f;
	m[3].z = 1.0f;
	m[3].w = 0.0f;

	//m = glm::transpose(m);

	return m;
}

void MCamera::Camera::RotateX(float dx)
{
	mouseDX += (int32_t)dx;

	//x_angle_quater += dx * aspect * pix_to_rad;
	//x_angle_quater = mouseDX * aspect * pix_to_rad;
	x_angle_quater = mouseDX * aspect * 0.0015f;//0.0015f;

	x_angle_quater = min(PIDIV2, max(x_angle_quater, -PIDIV2));

	need_update = true;
}

void MCamera::Camera::RotateY(float dy)
{
	mouseDY += (int32_t)dy;

	//y_angle_quater += dy * pix_to_rad;
	//y_angle_quater = mouseDY * pix_to_rad;
	y_angle_quater = mouseDY * 0.0015f;//0.0015f;

	if (y_angle_quater < 0.0f) y_angle_quater += TWOPI;
	else if (y_angle_quater >= TWOPI) y_angle_quater -= TWOPI;

	need_update = true;
}

void MCamera::Camera::MoveForvard(float delta)
{
	dz = delta;
	need_update = true;
}

void MCamera::Camera::MoveRight(float delta)
{
	dx = delta;
	need_update = true;
}

void MCamera::Camera::MoveUp(float delta)
{
	dy = delta;
	need_update = true;
}

void MCamera::Camera::UpdateCamera(float delta_time_ms)
{
	if (!need_update)return;

	quat x_quat = glm::quat(cos(x_angle_quater), vec3(sin(x_angle_quater), 0.0f, 0.0f));
	vec3 y_vec = glm::mat3_cast(x_quat)[1];

	y_vec *= sin(y_angle_quater);

	quat res_quat = glm::quat(cos(y_angle_quater), y_vec) * x_quat;

	mat3 rotation_matrix = glm::mat3_cast(res_quat);

	rotation_matrix = glm::transpose(rotation_matrix);

	i = rotation_matrix[0];
	j = rotation_matrix[1];
	k = rotation_matrix[2];

 	vec3 delta = (i * dx) + (j * dy) + (-k * dz);
	
	float mlenght = glm::length(delta);
	if (mlenght > 0.0f)
	{
		delta = glm::normalize(delta);
		delta *= min(speed * meter * delta_time_ms, mlenght);
	}

	position += delta;

	vec4 st0 = vec4(1.0f, 0.0f, 0.0f, 0.0f);
	vec4 st1 = vec4(0.0f, 1.0f, 0.0f, 0.0f);
	vec4 st2 = vec4(0.0f, 0.0f, 1.0f, 0.0f);
	vec4 st3 = vec4(-position, 1.0f);

	view = glm::mat4_cast(res_quat) * mat4(st0, st1, st2, st3);

	ProjectView = project * view;

	dx = dy = dz = 0.0f;
}

mat4 MCamera::Camera::GetViewMatrix() const
{
	return view;
}

mat4 MCamera::Camera::GetProjMatrix() const
{
	return project;
}

mat4 MCamera::Camera::GetProjViewMatrix() const
{
	return ProjectView;
}

float MCamera::Camera::GetNearPlane() 
{
	return near_plane;
}

float MCamera::Camera::GetFarPlane() 
{
	return far_plane;
}
					
vec3 MCamera::Camera::GetPosition() const
{
	return position;
}

vec3 MCamera::Camera::GetCamDir() const
{
	return -k;
}

void MCamera::Camera::MoveTo(vec3 pos) 
{
	position = pos;
}

void MCamera::Camera::SetMoveSpeed(float speed) 
{
	this->speed = speed;
}

float MCamera::Camera::GetMeter(void)
{
	return meter;
}

uint32_t MCamera::Camera::GetWeight(void) const
{
	return width;
}

uint32_t MCamera::Camera::GetHeight(void) const
{
	return height;
}