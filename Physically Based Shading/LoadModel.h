#include "MTimer.h"
#include "MCamera.h"
#include "MeshObject.h"
#include "LoadTexture.h"

#pragma once
namespace MEngine
{
	struct MObjects
	{
		std::vector<MEngine::MeshObject> mEntries;//�������� ������ � ��������

		GLuint VAO, VB;
		GLuint IB;
	};

	struct MResource 
	{
		std::vector<MeshLoad::ObjectMaterial> mMaterial;//�������� ������ ���������� � �������
		Resource::MapResource *texture_map;//�������� ������ ������� �������
	};

	struct MDrawList 
	{
		uint32_t clip_offset;
		uint32_t c0, c1, c2, c3;
		uint32_t c4, c5, c6, c7;
		//[0;c1) [c1; c2) [c2; c3) [c3; clip_offset)
		//[clip_offset;c4) [c4; c5) [c5; c)

		//[0;c0)
		//[c0;c1)
		//[c1;c2)
		//[c2;c3)

		//[c3;c4)
		//[c4;c5)
		//[c5;c6)
		//[c6;c7)
	};
	
	class LoadModel
	{
		/////////////////////////////////////////////////////////
		const uint32_t w, h, mInstanceCount;
		const float mOffsetScale;
		/////////////////////////////////////////////////////////
		MeshLoad::StandartMesh mesh_VNT;//���������� ������ ������ � �������� 
		MeshLoad::BumpMesh mesh_VNTT;//���������� ������ ������ � ��������
		Resource::LoadTexture *load_texture;
		/////////////////////////////////////////////////////////
		MResource *mresource;
		MObjects *VNT_object;
		MObjects *VNTT_object;
		MDrawList VNTT_object_list;
		MDrawList VNT_object_list;
		uint32_t EnvMap;
		/////////////////////////////////////////////////////////
		void SortVNTObject(void);
		void SortVNTObject(uint32_t start, uint32_t end, uint32_t &c1, uint32_t &c2, uint32_t &c3, uint32_t &c4);
		void SortVNTTObject(void);
		void SortVNTTObject(uint32_t start, uint32_t end, uint32_t &c1, uint32_t &c2, uint32_t &c3);
		/////////////////////////////////////////////////////////
		void InitScene(const aiScene* pScene, const char *file_name);
		void InitMeshVNT(uint32_t base_vertex, uint32_t base_index, const aiMesh *paiMesh, float xOffset, float zOffset, float scale = 1.0f);//��������� ������ ���� ���� MeshLoad::ID_Face::TRIANGLE_NTEX
		void InitMeshVNTT(uint32_t base_vertex, uint32_t base_index, const aiMesh *paiMesh, float xOffset, float zOffset);//��������� ������ ���� ���� MeshLoad::ID_Face::TRIANGLE_NTEX
		void MCreateBufferVNT(void);
		void MCreateBufferVNTT(void);
	public:
		LoadModel(const char *file_name);
		~LoadModel();

		MResource *GetResource(void);
		MObjects *GetVNTMesh(void);
		MObjects *GetVNTTMesh(void);

		MDrawList GetObjectListVNT(void);
		MDrawList GetObjectListVNTT(void);

		uint32_t GetEnvTexture(void);
	};

	//������������� FreeImage �� ������ ����������
	//����� �� ���������� ��� �� ��� ������� ����������� ������� ����� ���� ������� � RGB � BRG � ������
	//���������� �������� �� DXTex
	//reflaction map - ��� ���������� �����!!!
	//specular map - ������ �������������� � reflaction map, ������ ���������� ������ red canal; -> R8
	//� �������� ������������ ������� specular map ������������ ��� ����� ����� ��������� ��������
	//��� ���� ������� ��� �������� ����� ���� ������������ �����, ��� ����� ������� ������� specular map ��������� � ������� r8
	//�� ��������� ��������� �������� ������������ ��� �� �������� ����� ����� ��� ��������� ��� ��� ���������
	//���� ��������� ��������� ��������� �������� � ������������� �� ���������� � ������ ���������� ���� ����

	//cubemap render one pass http://cg.siomax.ru/index.php/computer-graphics/10-one-pass-rendering-to-cube-map
	//multi pass http://stackoverflow.com/questions/28845375/rendering-a-dynamic-cubemap-opengl
}

