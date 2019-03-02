#include "LoadStructure.h"

#pragma once
using namespace std;
namespace Resource
{
	class MapResource
	{
		vector<GLTextureResource> map_resource;//������ ��������, 0 ������� ������
		vector<ResourceName> map_resource_name;//������ ���� ��������, 0 ������� L""
		vector<unsigned long> resource_reference_count;//���������� ���������� �� ������

		vector<bool> alpha_map;//��� ������ �������� �������� ���������� � ��� ������������� �� �-�����
	public:
		MapResource();
		~MapResource();

		unsigned long MapFindResource(const char resource_name[MAX_PATH]);//������� ���������� ID ���������� ������� ��� 0 ������ ������� ��������� �� ���� ����� ���� 0-��������

		GLuint AddReference(unsigned long resource_ID);
		unsigned long ReleaseResource(unsigned long resource_ID);

		unsigned long AppendResource(GLuint resource_map, GLuint resource_target, const char resource_name[MAX_PATH], bool use_alpha);//��������� ������ � ���������� ��� id  ������ ������� ��������� �� ���� ����� ���� 0-��������
		GLTextureResource GetResourceByID(unsigned long resource_id);

		bool GetAlphaMap(unsigned long resource_id);
	};
}