#define GLEW_STATIC
#include <glew.h>
#include <Windows.h>
#include <windowsx.h>

#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/constants.hpp>
#include <stdint.h>
#pragma once

using namespace glm;
namespace MCamera
{
#define PI 3.1415926535897932384626433832795f
#define PIDIV2 PI * 0.5f
#define TWOPI PI * 2.0f
	class Camera
	{
		/////////////////////////////////
		float near_plane, far_plane;//������� � ������� ��������� ���������
		/////////////////////////////////
		int32_t mouseDX, mouseDY;
		/////////////////////////////////
		const float pix_to_rad;//78125f;//0.005f;//��������� ��� �������� �������� � �������� � ���� � ��������
		const float aspect;//��������� ��������� ������ ������
		unsigned long width, height;// ������ � ������ ������
		/////////////////////////////////
		float y_angle_quater;//���� �������� ������ ��� x
		float x_angle_quater;//���� �������� ������ ��� y
		/////////////////////////////////
		glm::vec3 position;//������� ������
		/////////////////////////////////
		glm::mat4 view;//������� ����
		glm::mat4 project;//������� ��������
		glm::mat4 ProjectView;//������� ��������� ��������������
		/////////////////////////////////
		float dx, dy, dz;//�������� ������ �� ���� i, j, k
		/////////////////////////////////
		vec3 i, j, k;//�������� �������
		/////////////////////////////////
		float meter;//�������
		float speed;//�������� ����������� ������ � ������ (meter/ms)
		/////////////////////////////////
		bool need_update;//������ ����������, ������������ ������������� ���������� ������(true - ���������� ��������)
		/////////////////////////////////
		glm::mat4 MPerspective(float zNear, float zFar, float ar, float fovY);
	public:
		Camera(float near_clip_plane, float far_clip_plane, unsigned long width, unsigned long height, float aspect_angle, float meter, float speed);
		~Camera();
		/////////////////////////////////
		void RotateX(float dx);//��������� ������ �� ��� x
		void RotateY(float dy);//��������� ������ �� ��� y
		/////////////////////////////////
		void MoveForvard(float delta);//����������� ������ ������
		void MoveRight(float delta);//����������� ������ ������
		void MoveUp(float delta);//����������� ������ �����
		/////////////////////////////////
		void UpdateCamera(float delta_time_ms);//�������� ������� ������
		/////////////////////////////////
		mat4 GetViewMatrix() const;//�������� ������� ����
		mat4 GetProjMatrix() const;//�������� ������� �������
		mat4 GetProjViewMatrix() const;//�������� �������� ������� �������������
		/////////////////////////////////
		float GetNearPlane();//�������� ������� ��������� ���������
		float GetFarPlane();//�������� ������� ��������� ���������
		/////////////////////////////////
		vec3 GetPosition() const;//�������� ������� ������
		/////////////////////////////////
		vec3 GetCamDir() const;//�������� ������ ������� ������(������ k)
		/////////////////////////////////
		void MoveTo(vec3 pos);//����������� ������� ������ �� ����� pos
		void SetMoveSpeed(float speed);//���������� �������� ������������ ������
		/////////////////////////////////
		float GetMeter(void);
		/////////////////////////////////
		uint32_t GetWeight(void) const;
		uint32_t GetHeight(void) const;
	};
}