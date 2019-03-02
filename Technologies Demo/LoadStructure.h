#include "ResourceStruct.h"
#include <Importer.hpp>
#include <scene.h>
#include <postprocess.h>
#include <vector>

#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/constants.hpp>

#pragma once
namespace MeshLoad 
{
	struct VERTEX
	{
		glm::vec3 l_pos;
		glm::vec3 l_norm;
		glm::vec2 l_tex;
	};

	struct MVertexBump
	{
		glm::vec3 l_pos;
		glm::vec3 l_norm;
		glm::vec3 l_tangent;
		glm::vec2 l_tex;
	};

	struct MVertexTex
	{
		glm::vec3 l_pos;
		glm::vec2 l_tex;
	};

	struct MVertex
	{
		glm::vec3 l_pos;
		glm::vec3 l_norm;
	};

	struct StandartMesh 
	{
		std::vector<MeshLoad::VERTEX> m_vertex;
		std::vector<uint32_t> m_index;
	};

	struct BaseMesh
	{
		std::vector<MeshLoad::MVertex> m_vertex;
		std::vector<uint32_t> m_index;
	};

	struct BumpMesh
	{
		std::vector<MeshLoad::MVertexBump> m_vertex;
		std::vector<uint32_t> m_index;
	};

	enum ID_Face { TRIANGLE_NTEX, TRIANGLE_TNTEX, TRIANGLE_N, QUAD_NTEX, QUAD_TNTEX, QUAD_N };
	//TRIANGLE_NTEX - ����������� � �������� � ����������� ������������
	//TRIANGLE_TNTEX - ����������� � �������� � ����������� �������� � ����������� ������������
	//TRIANGLE_N - ����������� ������ � ��������

	//QUAD_NTEX - ���� � �������� � ����������� ������������
	//QUAD_TNTEX - ���� � �������� � ����������� �������� � ����������� ������������
	//QUAD_N - ���� ������ � ��������


	struct Material
	{
		glm::vec4 diffuse_color;//� a ���������� ��������� ambient / diffuse
		glm::vec4 specular_color;//a - spec_pow(shininess)
		//������ �������� ����� ���������� �����
		glm::vec4 emissive_color;//����������� ������������
	};

	struct ObjectMaterial
	{
		Material material;
		uint32_t diffuse_map_id;//����� ������ ����������� ��� � 0 ����� ����� ���������� �������� NULL, ������� ���� diffuse_map_id == 0 �� ��������� �������� �� �������������
		uint32_t reflaction_map_id;//����� ����� ���������� ������ ���������(�������������� �������), ���������� �����
		uint32_t specular_map_id;//����� ������������� ����������� ���������, ������������� ������ r ����� -> ������ �������� R8
		uint32_t bump_map_id;

		//������ ���� ���� reflaction_map_id(specular_map_id)�� ������ ���� � specular_map_id(reflaction_map_id)
	};

	struct DrawParam 
	{
		uint32_t vectex_buffer_start;//�������� ������� 1 ������� � ��������� �������
		uint32_t index_buffer_start;//�������� ������� 1 ������ � ��������� �������
		uint32_t index_count;//�������� ���������� �������� �������
	};

}