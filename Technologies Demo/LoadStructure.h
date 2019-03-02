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
	//TRIANGLE_NTEX - треугольник с нормалью и текстурными координатами
	//TRIANGLE_TNTEX - треугольник с нормалью и касательным вектором и текстурными координатами
	//TRIANGLE_N - треугольник только с нормалью

	//QUAD_NTEX - квад с нормалью и текстурными координатами
	//QUAD_TNTEX - квад с нормалью и касательным вектором и текстурными координатами
	//QUAD_N - квад только с нормалью


	struct Material
	{
		glm::vec4 diffuse_color;//в a находиться отношение ambient / diffuse
		glm::vec4 specular_color;//a - spec_pow(shininess)
		//прочие свойства будут определены позже
		glm::vec4 emissive_color;//эмиссионная составляющая
	};

	struct ObjectMaterial
	{
		Material material;
		uint32_t diffuse_map_id;//будет список изображений где в 0 слоте будет находиться значание NULL, поэтому если diffuse_map_id == 0 то диффузная текстура не используеться
		uint32_t reflaction_map_id;//здесь может находиться псевдо отражение(подготовленное заранее), кубическая карта
		uint32_t specular_map_id;//карта отражательной способности материала, используеться только r канал -> формат текстуры R8
		uint32_t bump_map_id;

		//причем если есть reflaction_map_id(specular_map_id)то должна быть и specular_map_id(reflaction_map_id)
	};

	struct DrawParam 
	{
		uint32_t vectex_buffer_start;//содержит позицию 1 вершины в вершинном буффере
		uint32_t index_buffer_start;//содержит позицию 1 идекса а индексном буффере
		uint32_t index_count;//содержит количество индексов объекта
	};

}