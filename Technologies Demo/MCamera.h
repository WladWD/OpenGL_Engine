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
		float near_plane, far_plane;//ближняя и дальняя плоскости отсечения
		/////////////////////////////////
		int32_t mouseDX, mouseDY;
		/////////////////////////////////
		const float pix_to_rad;//78125f;//0.005f;//константа для перевода смещений в пикселях в угол в радианах
		const float aspect;//аспектное отношение сторон экрана
		unsigned long width, height;// ширина и высота экрана
		/////////////////////////////////
		float y_angle_quater;//угол вращения вокруг оси x
		float x_angle_quater;//угол вращения вокруг оси y
		/////////////////////////////////
		glm::vec3 position;//позиция камеры
		/////////////////////////////////
		glm::mat4 view;//матрица вида
		glm::mat4 project;//матрица проекции
		glm::mat4 ProjectView;//матрица итогового преобразования
		/////////////////////////////////
		float dx, dy, dz;//смещение камеры по осям i, j, k
		/////////////////////////////////
		vec3 i, j, k;//базисные вектора
		/////////////////////////////////
		float meter;//масштаб
		float speed;//скорость перемещения камеры в метрах (meter/ms)
		/////////////////////////////////
		bool need_update;//булева переменная, определяющая необходимость обновления матриц(true - необходимо обновить)
		/////////////////////////////////
		glm::mat4 MPerspective(float zNear, float zFar, float ar, float fovY);
	public:
		Camera(float near_clip_plane, float far_clip_plane, unsigned long width, unsigned long height, float aspect_angle, float meter, float speed);
		~Camera();
		/////////////////////////////////
		void RotateX(float dx);//повернуть камеру по оси x
		void RotateY(float dy);//повернуть камеру по оси y
		/////////////////////////////////
		void MoveForvard(float delta);//переместить камеру вперед
		void MoveRight(float delta);//переместить камеру вправо
		void MoveUp(float delta);//переместить камеру вверх
		/////////////////////////////////
		void UpdateCamera(float delta_time_ms);//обновить матрица камеры
		/////////////////////////////////
		mat4 GetViewMatrix() const;//получить матрицу вида
		mat4 GetProjMatrix() const;//получить матрицу поэкции
		mat4 GetProjViewMatrix() const;//получить итоговую матрицу проэцирования
		/////////////////////////////////
		float GetNearPlane();//получить ближнюю плоскость отсечения
		float GetFarPlane();//получить дальнюю плоскость отсечения
		/////////////////////////////////
		vec3 GetPosition() const;//получить позицию камеры
		/////////////////////////////////
		vec3 GetCamDir() const;//получить вектор взгляда камеры(вектор k)
		/////////////////////////////////
		void MoveTo(vec3 pos);//переместить позицию камеры на точку pos
		void SetMoveSpeed(float speed);//установить скорость перемешщения камеры
		/////////////////////////////////
		float GetMeter(void);
		/////////////////////////////////
		uint32_t GetWeight(void) const;
		uint32_t GetHeight(void) const;
	};
}