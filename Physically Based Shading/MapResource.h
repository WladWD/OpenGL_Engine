#include "LoadStructure.h"

#pragma once
using namespace std;
namespace Resource
{
	class MapResource
	{
		vector<GLTextureResource> map_resource;//список ресурсов, 0 элемент пустой
		vector<ResourceName> map_resource_name;//список имен ресурсов, 0 элемент L""
		vector<unsigned long> resource_reference_count;//количество указателей на ресурс

		vector<bool> alpha_map;//для каждой текстуры содержит информацию о том используеться ли а-канал
	public:
		MapResource();
		~MapResource();

		unsigned long MapFindResource(const char resource_name[MAX_PATH]);//функция возвращает ID найденного ресурса или 0 строки которые подаються на вход длжны быть 0-байтными

		GLuint AddReference(unsigned long resource_ID);
		unsigned long ReleaseResource(unsigned long resource_ID);

		unsigned long AppendResource(GLuint resource_map, GLuint resource_target, const char resource_name[MAX_PATH], bool use_alpha);//вставляет ресурс и возвращает его id  строки которые подаються на вход длжны быть 0-байтными
		GLTextureResource GetResourceByID(unsigned long resource_id);

		bool GetAlphaMap(unsigned long resource_id);
	};
}