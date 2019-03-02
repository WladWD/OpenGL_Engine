#include "LoadModel.h"

MEngine::LoadModel::LoadModel(const char *file_name) 
{
	Assimp::Importer Importer;

	VNT_object = new MObjects();
	VNTT_object = new MObjects();

	mresource = new MResource();

	mresource->texture_map = new Resource::MapResource();
	load_texture = new Resource::LoadTexture(mresource->texture_map);
	//aiProcess_FixInfacingNormals | aiProcess_GenSmoothNormals
	const aiScene* pScene = Importer.ReadFile(file_name, aiProcess_Triangulate | aiProcess_CalcTangentSpace | aiProcess_FlipUVs);// | aiProcess_GenSmoothNormals);// | aiProcess_OptimizeMeshes);

	if (pScene) InitScene(pScene, file_name);
	else MessageBox(NULL, L"File Not Load", L"", MB_OK);

	Importer.FreeScene();
	delete load_texture;

	mesh_VNT.m_index.clear();
	mesh_VNT.m_vertex.clear();

	mesh_VNTT.m_index.clear();
	mesh_VNTT.m_vertex.clear();
}

MEngine::LoadModel::~LoadModel() 
{
}

void MEngine::LoadModel::InitScene(const aiScene* pScene, const char *file_name)
{
	uint32_t mesh_count_VNT = 0, mesh_count_VNTT = 0;
	uint32_t index_count_VNT = 0, index_count_VNTT = 0;
	uint32_t vertex_count_VNT = 0, vertex_count_VNTT = 0;

	uint32_t base_vertex_VNT = 0, base_vertex_VNTT = 0;
	//uint32_t vertex_count = 0;

	uint32_t base_index_VNT = 0, base_index_VNTT = 0;
	//uint32_t index_count = 0;

	std::string FILE_NAME = file_name;
	int str_pos = -1;
	for (register int i = 0; file_name[i]; i++)if (file_name[i] == '\\')str_pos = i;
	//std::string::size_type SlashIndex = FILE_NAME.find_last_of('\\');
	std::string Dir;

	if (str_pos == (-1)) {
		Dir = ".";
	}
	else if (str_pos == 0) {
		Dir = "/";
	}
	else {
		Dir = FILE_NAME.substr(0, str_pos);
	}

	mresource->mMaterial.resize(pScene->mNumMaterials);

	//Ns - коэффициет отражения для бликов
	//Ni - коеффициет преломлени
	for (int i = 0; i < pScene->mNumMaterials; i++)
	{
		aiMaterial *pMaterial = pScene->mMaterials[i];
		MeshLoad::ObjectMaterial mCurentMaterial;

		aiColor4D mcol;
		aiGetMaterialColor(pMaterial, AI_MATKEY_COLOR_SPECULAR, &mcol);//specular
		mCurentMaterial.material.specular_color = vec4(mcol.r, mcol.g, mcol.b, 1.0f);

		aiGetMaterialColor(pMaterial, AI_MATKEY_SHININESS, &mcol);//Ns - по неизввесным причинам Assimp загружает значение Ns * 4, поэтому считанное значение необходимо делить на 4
		mCurentMaterial.material.specular_color.a = mcol.r * 0.25f;

		/*
		aiGetMaterialColor(pMaterial, AI_MATKEY_REFLECTIVITY, &mcol);//величина отражения света

		aiGetMaterialColor(pMaterial, AI_MATKEY_REFRACTI, &mcol);//величина преломления света

		aiGetMaterialColor(pMaterial, AI_MATKEY_OPACITY, &mcol);//величина прозрачности материала
		*/

		aiGetMaterialColor(pMaterial, AI_MATKEY_COLOR_DIFFUSE, &mcol);//diffuse
		mCurentMaterial.material.diffuse_color = vec4(mcol.r, mcol.g, mcol.b, 1.0f);

		aiGetMaterialColor(pMaterial, AI_MATKEY_COLOR_AMBIENT, &mcol);//ambient
		mCurentMaterial.material.diffuse_color.a = pow((mcol.r / mCurentMaterial.material.diffuse_color.r)*0.8f, 2.5f);

		aiGetMaterialColor(pMaterial, AI_MATKEY_COLOR_EMISSIVE, &mcol);//Ke - emissive-материал способлый к свечению(для реализации использовать рассчет освещени на шейдерах, а затем размытие)
		mCurentMaterial.material.emissive_color = vec4(mcol.r, mcol.g, mcol.b, 1.0f);

		mCurentMaterial.diffuse_map_id = 0;
		mCurentMaterial.bump_map_id = 0;
		mCurentMaterial.reflaction_map_id = 0;
		mCurentMaterial.specular_map_id = 0;

		if (pMaterial->GetTextureCount(aiTextureType_DIFFUSE) > 0)
		{
			aiString Path;

			if (pMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &Path,
				NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS)
			{
				std::string FullPath = Dir + "\\" + Path.data;

				mCurentMaterial.diffuse_map_id = load_texture->LoadResource(FullPath);
			}
		}

		if (pMaterial->GetTextureCount(aiTextureType_NORMALS) > 0)
		{
			aiString Path;

			if (pMaterial->GetTexture(aiTextureType_NORMALS, 0, &Path,
				NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS)
			{
				std::string FullPath = Dir + "\\" + Path.data;

				mCurentMaterial.bump_map_id = load_texture->LoadResource(FullPath);
			}
		}

		if (pMaterial->GetTextureCount(aiTextureType_SPECULAR) > 0)
		{
			aiString Path;

			if (pMaterial->GetTexture(aiTextureType_SPECULAR, 0, &Path,
				NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS)
			{
				std::string FullPath = Dir + "\\" + Path.data;

				mCurentMaterial.specular_map_id = load_texture->LoadResource(FullPath);
			}
		}

		if (pMaterial->GetTextureCount(aiTextureType_EMISSIVE) > 0)//aiTextureType_SPECULAR
		{
			aiString Path;

			if (pMaterial->GetTexture(aiTextureType_EMISSIVE, 0, &Path,
				NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS)
			{
				//здесть указано aiTextureType_EMISSIVE но это aiTextureType_REFLECTION
				//т.к. assimp не видит лексему refl
				std::string FullPath = Dir + "\\" + Path.data;

				mCurentMaterial.reflaction_map_id = load_texture->LoadResource(FullPath);
			}
		}

		//mresource->mMaterial.push_back(mCurentMaterial);
		mresource->mMaterial[i] = mCurentMaterial;
	}

	for (register uint32_t i = 0; i < pScene->mNumMeshes; i++) 
	{
		if (pScene->mMeshes[i]->HasTextureCoords(0))
		{
			if (mresource->mMaterial[pScene->mMeshes[i]->mMaterialIndex].bump_map_id)
			{
				mesh_count_VNTT++;
				index_count_VNTT += pScene->mMeshes[i]->mNumFaces;
				vertex_count_VNTT += pScene->mMeshes[i]->mNumVertices;
			}
			else 
			{
				mesh_count_VNT++;
				index_count_VNT += pScene->mMeshes[i]->mNumFaces;
				vertex_count_VNT += pScene->mMeshes[i]->mNumVertices;
			}
		}
	}

	index_count_VNT *= 3;
	index_count_VNTT *= 3;

	VNT_object->mEntries.resize(mesh_count_VNT);
	VNTT_object->mEntries.resize(mesh_count_VNTT);

	mesh_VNT.m_vertex.resize(vertex_count_VNT);
	mesh_VNT.m_index.resize(index_count_VNT);

	mesh_VNTT.m_vertex.resize(vertex_count_VNTT);
	mesh_VNTT.m_index.resize(index_count_VNTT);

	register uint32_t i = 0, j = 0, k = 0;
	for (; i < pScene->mNumMeshes; i++) 
	{
		if (pScene->mMeshes[i]->HasTextureCoords(0))
		{
			if (mresource->mMaterial[pScene->mMeshes[i]->mMaterialIndex].bump_map_id)
			{
				const aiMesh *paiMesh = pScene->mMeshes[i];

				VNTT_object->mEntries[k].SetObjectName(paiMesh->mName.C_Str(), paiMesh->mName.length);
				VNTT_object->mEntries[k].SetVertexCount(paiMesh->mNumVertices);
				VNTT_object->mEntries[k].SetVertexStart(base_vertex_VNTT);
				VNTT_object->mEntries[k].SetIndexStart(base_index_VNTT);
				VNTT_object->mEntries[k].SetIndexCount(paiMesh->mNumFaces * 3);
				VNTT_object->mEntries[k].SetFaceCount(paiMesh->mNumFaces);
				VNTT_object->mEntries[k].SetType(MeshLoad::ID_Face::TRIANGLE_TNTEX);
				VNTT_object->mEntries[k].SetMaterialID(paiMesh->mMaterialIndex);

				InitMeshVNTT(base_vertex_VNTT, base_index_VNTT, paiMesh);

				base_vertex_VNTT += paiMesh->mNumVertices;
				base_index_VNTT += paiMesh->mNumFaces * 3;

				MeshLoad::ObjectMaterial material = mresource->mMaterial[paiMesh->mMaterialIndex];

				bool use_reflect = material.reflaction_map_id && material.specular_map_id && material.diffuse_map_id ? true : false;
				bool use_diffuse = material.diffuse_map_id ? true : false;
				bool use_specular = material.diffuse_map_id && material.specular_map_id ? true : false;
				bool use_height_alpha = mresource->texture_map->GetAlphaMap(material.bump_map_id) ? true : false;
				bool use_clip_alpha = (mresource->texture_map->GetAlphaMap(material.diffuse_map_id)) && material.diffuse_map_id ? true : false;
				//bool use_alpha = (mresource->texture_map->GetAlphaMap(material.diffuse_map_id)) && material.diffuse_map_id ? true : false;

				VNTT_object->mEntries[k].SetUseReflaction(use_reflect);
				VNTT_object->mEntries[k].SetUseDiffuse(use_diffuse);
				VNTT_object->mEntries[k].SetUseSpecular(use_specular);
				VNTT_object->mEntries[k].SetClipOpacity(use_clip_alpha);
				VNTT_object->mEntries[k].SetUseAlpha(false);
				VNTT_object->mEntries[k].SetUseBump(true);
				VNTT_object->mEntries[k].SetUseHeightAlpha(use_height_alpha);

				k++;
			}
			else 
			{
				const aiMesh *paiMesh = pScene->mMeshes[i];

				VNT_object->mEntries[j].SetObjectName(paiMesh->mName.C_Str(), paiMesh->mName.length);
				VNT_object->mEntries[j].SetVertexCount(paiMesh->mNumVertices);
				VNT_object->mEntries[j].SetVertexStart(base_vertex_VNT);
				VNT_object->mEntries[j].SetIndexStart(base_index_VNT);
				VNT_object->mEntries[j].SetIndexCount(paiMesh->mNumFaces * 3);
				VNT_object->mEntries[j].SetFaceCount(paiMesh->mNumFaces);
				VNT_object->mEntries[j].SetType(MeshLoad::ID_Face::TRIANGLE_NTEX);
				VNT_object->mEntries[j].SetMaterialID(paiMesh->mMaterialIndex);

				InitMeshVNT(base_vertex_VNT, base_index_VNT, paiMesh);

				base_vertex_VNT += paiMesh->mNumVertices;
				base_index_VNT += paiMesh->mNumFaces * 3;

				MeshLoad::ObjectMaterial material = mresource->mMaterial[paiMesh->mMaterialIndex];

				bool use_reflect = material.reflaction_map_id && material.specular_map_id && material.diffuse_map_id ? true : false;
				bool use_diffuse = material.diffuse_map_id ? true : false;
				bool use_specular = material.diffuse_map_id && material.specular_map_id ? true : false;
				bool use_clip_alpha = (mresource->texture_map->GetAlphaMap(material.diffuse_map_id)) && material.diffuse_map_id ? true : false;
				//bool use_alpha = (mresource->texture_map->GetAlphaMap(material.diffuse_map_id)) && material.diffuse_map_id ? true : false;

				VNT_object->mEntries[j].SetUseReflaction(use_reflect);
				VNT_object->mEntries[j].SetUseDiffuse(use_diffuse);
				VNT_object->mEntries[j].SetUseSpecular(use_specular);
				VNT_object->mEntries[j].SetClipOpacity(use_clip_alpha);
				VNT_object->mEntries[j].SetUseAlpha(false);
				VNT_object->mEntries[j].SetUseBump(false);
				VNT_object->mEntries[j].SetUseHeightAlpha(false);

				j++;
			}
		}
	}


	SortVNTObject();
	SortVNTTObject();

	MCreateBufferVNT();
	MCreateBufferVNTT();
}

void MEngine::LoadModel::InitMeshVNT(uint32_t base_vertex, uint32_t base_index, const aiMesh *paiMesh)
{
	//здесь собираеться 1 обьект, так что при необходимости можно его выделить отсюда в отдельный буффер(например если обьект динамичен), (для скелетной анимации)

	for (register uint32_t i = 0; i < paiMesh->mNumVertices; i++) 
	{
		aiVector3D vert_pos = paiMesh->mVertices[i];
		aiVector3D vert_norm = paiMesh->mNormals[i];
		aiVector3D vert_tex = paiMesh->mTextureCoords[0][i];

		mesh_VNT.m_vertex[base_vertex + i].l_pos = glm::vec3(vert_pos.x, vert_pos.y, vert_pos.z);
		mesh_VNT.m_vertex[base_vertex + i].l_norm = normalize(glm::vec3(vert_norm.x, vert_norm.y, vert_norm.z));
		mesh_VNT.m_vertex[base_vertex + i].l_tex = glm::vec2(vert_tex.x, vert_tex.y);//1.0f - vert_tex.y);
	}

	for (register uint32_t i = 0; i < paiMesh->mNumFaces; i++)
	{
		const aiFace& Face = paiMesh->mFaces[i];
		assert(Face.mNumIndices == 3);

		mesh_VNT.m_index[base_index + i * 3 + 0] = Face.mIndices[0];
		mesh_VNT.m_index[base_index + i * 3 + 1] = Face.mIndices[1];
		mesh_VNT.m_index[base_index + i * 3 + 2] = Face.mIndices[2];
	}
}

void MEngine::LoadModel::InitMeshVNTT(uint32_t base_vertex, uint32_t base_index, const aiMesh *paiMesh)
{
	//здесь собираеться 1 обьект, так что при необходимости можно его выделить отсюда в отдельный буффер(например если обьект динамичен), (для скелетной анимации)

	for (register uint32_t i = 0; i < paiMesh->mNumVertices; i++)
	{
		aiVector3D vert_pos = paiMesh->mVertices[i];
		aiVector3D vert_norm = paiMesh->mNormals[i];
		aiVector3D vert_tangent = paiMesh->mTangents[i];
		aiVector3D vert_tex = paiMesh->mTextureCoords[0][i];

		vec3 mtang = normalize(glm::vec3(vert_tangent.x, vert_tangent.y, vert_tangent.z));

		mesh_VNTT.m_vertex[base_vertex + i].l_pos = glm::vec3(vert_pos.x, vert_pos.y, vert_pos.z);
		mesh_VNTT.m_vertex[base_vertex + i].l_norm = normalize(glm::vec3(vert_norm.x, vert_norm.y, vert_norm.z));
		mesh_VNTT.m_vertex[base_vertex + i].l_tex = glm::vec2(vert_tex.x, vert_tex.y); //1.0f - vert_tex.y);
		mesh_VNTT.m_vertex[base_vertex + i].l_tangent = glm::vec3(mtang.x, mtang.y, mtang.z);//-mtang.z, mtang.y);
		//l_tangent - glm::vec3(mtang.x, -mtang.z, mtang.y);  -- ok
	}

	for (register uint32_t i = 0; i < paiMesh->mNumFaces; i++)
	{
		const aiFace& Face = paiMesh->mFaces[i];
		assert(Face.mNumIndices == 3);

		mesh_VNTT.m_index[base_index + i * 3 + 0] = Face.mIndices[0];
		mesh_VNTT.m_index[base_index + i * 3 + 1] = Face.mIndices[1];
		mesh_VNTT.m_index[base_index + i * 3 + 2] = Face.mIndices[2];
	}
}

void MEngine::LoadModel::MCreateBufferVNT(void) 
{
	VNT_object->IB = 0;
	VNT_object->VB = 0;
	VNT_object->VAO = 0;

	uint32_t index_count = mesh_VNT.m_index.size();
	uint32_t vertex_count = mesh_VNT.m_vertex.size();

	if (!index_count || !vertex_count)return;

	glCreateBuffers(1, &VNT_object->IB);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VNT_object->IB);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh_VNT.m_index.size() * sizeof(uint32_t), &mesh_VNT.m_index[0], GL_STATIC_DRAW);

	glGenVertexArrays(1, &VNT_object->VAO);
	glBindVertexArray(VNT_object->VAO);

	glCreateBuffers(1, &VNT_object->VB);
	glBindBuffer(GL_ARRAY_BUFFER, VNT_object->VB);
	glBufferData(GL_ARRAY_BUFFER, mesh_VNT.m_vertex.size() * sizeof(MeshLoad::VERTEX), &mesh_VNT.m_vertex[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void MEngine::LoadModel::MCreateBufferVNTT(void) 
{
	VNTT_object->IB = 0;
	VNTT_object->VB = 0;
	VNTT_object->VAO = 0;

	uint32_t index_count = mesh_VNT.m_index.size();
	uint32_t vertex_count = mesh_VNT.m_vertex.size();

	if (!index_count || !vertex_count)return;

	glCreateBuffers(1, &VNTT_object->IB);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VNTT_object->IB);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh_VNTT.m_index.size() * sizeof(uint32_t), &mesh_VNTT.m_index[0], GL_STATIC_DRAW);

	glGenVertexArrays(1, &VNTT_object->VAO);
	glBindVertexArray(VNTT_object->VAO);

	glCreateBuffers(1, &VNTT_object->VB);
	glBindBuffer(GL_ARRAY_BUFFER, VNTT_object->VB);
	glBufferData(GL_ARRAY_BUFFER, mesh_VNTT.m_vertex.size() * sizeof(MeshLoad::MVertexBump), &mesh_VNTT.m_vertex[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void MEngine::LoadModel::SortVNTObject(void)
{
	//uint32_t no_texture_offset = (-1);
	//uint32_t diff_texture_offset = (-1);
	//VNT_object->mEntries[i].IsClipOpacity();
	/*for (register uint32_t i = 0; i < VNT_object->mEntries.size(); i++) 
	{
		
		if (VNT_object->mEntries[i].IsUseDiffuse() && !VNT_object->mEntries[i].IsUseReflaction())
		{
			diff_texture_offset++;
			swap(VNT_object->mEntries[diff_texture_offset], VNT_object->mEntries[i]);
		}
		else if(!VNT_object->mEntries[i].IsUseReflaction() && !VNT_object->mEntries[i].IsUseDiffuse())
		{
			diff_texture_offset++;
			no_texture_offset++;

			swap(VNT_object->mEntries[diff_texture_offset], VNT_object->mEntries[i]);
			swap(VNT_object->mEntries[diff_texture_offset], VNT_object->mEntries[no_texture_offset]);
		}
	}*/
	uint32_t clip_offset = (-1);
	for (register uint32_t i = 0; i < VNT_object->mEntries.size(); i++)
	{
		if (!VNT_object->mEntries[i].IsClipOpacity())
		{
			clip_offset++;
			swap(VNT_object->mEntries[clip_offset], VNT_object->mEntries[i]);
		}
	}
	clip_offset++;
	
	uint32_t c1, c2, c3, c4, c5, c0, c6, c7;
	SortVNTObject(0, clip_offset, c0, c1, c2, c3);
	SortVNTObject(clip_offset, VNT_object->mEntries.size(), c4, c5, c6, c7);

	/*uint32_t last_index = 0;
	for (register uint32_t i = 0; i < VNT_object->mEntries.size() && !(VNT_object->mEntries[i].IsUseDiffuse()); i++, last_index++);
	c1 = last_index;
	for (register uint32_t i = last_index; i < VNT_object->mEntries.size() && (!VNT_object->mEntries[i].IsUseReflaction()) && VNT_object->mEntries[i].IsClipOpacity(); i++, last_index++);
	c2 = last_index;
	//clip_offset
	//[0;c1) [c1; c2) [c2; clip_offset)
	//[clip_offset;c3) [c3; c4) [c4; VNT_object->mEntries.size())
	//bool mpl = VNT_object->mEntries[138].IsClipOpacity();
	for (register uint32_t i = last_index; i < VNT_object->mEntries.size() && (!VNT_object->mEntries[i].IsUseDiffuse()); i++, last_index++);
	c3 = last_index;
	for (register uint32_t i = last_index; i < VNT_object->mEntries.size() && (!VNT_object->mEntries[i].IsUseReflaction()); i++, last_index++);
	c4 = last_index;
	c = VNT_object->mEntries.size();*/

	//[0;c0)
	//[c0;c1)
	//[c1;c2)
	//[c2;c3)

	//[c3;c4)
	//[c4;c5)
	//[c5;c6)
	//[c6;c7)
	VNT_object_list.c0 = c0;
	VNT_object_list.c1 = c1;
	VNT_object_list.c2 = c2;
	VNT_object_list.c3 = c3;
	VNT_object_list.c4 = c4;
	VNT_object_list.c5 = c5;
	VNT_object_list.c6 = c6;
	VNT_object_list.c7 = c7;
	VNT_object_list.clip_offset = clip_offset;
}

void MEngine::LoadModel::SortVNTObject(uint32_t start, uint32_t end, uint32_t &c1, uint32_t &c2, uint32_t &c3, uint32_t &c4)
{
	/*uint32_t no_texture_offset = (-1) + start;
	uint32_t diff_texture_offset = (-1) + start;
	//VNT_object->mEntries[i].IsClipOpacity();
	for (register uint32_t i = start; i < end; i++)
	{
		if (VNT_object->mEntries[i].IsUseDiffuse() && !VNT_object->mEntries[i].IsUseReflaction())
		{
			diff_texture_offset++;
			swap(VNT_object->mEntries[diff_texture_offset], VNT_object->mEntries[i]);
		}
		else if (!VNT_object->mEntries[i].IsUseReflaction() && !VNT_object->mEntries[i].IsUseDiffuse())
		{
			diff_texture_offset++;
			no_texture_offset++;

			swap(VNT_object->mEntries[diff_texture_offset], VNT_object->mEntries[i]);
			swap(VNT_object->mEntries[diff_texture_offset], VNT_object->mEntries[no_texture_offset]);
		}
	}*/

	int32_t ai = (-1) + start;//categori 1)
	int32_t bi = (-1) + start;//categori 2)
	int32_t ci = (-1) + start;//categori 3)
	int32_t di = (-1) + start;//categori 4)
	for (register int32_t i = start; i < end; i++)
	{
		if (!VNT_object->mEntries[i].IsUseDiffuse())
		{
			ai++;
			bi++;
			ci++;
			di++;
			swap(VNT_object->mEntries[bi], VNT_object->mEntries[ai]);
			swap(VNT_object->mEntries[ci], VNT_object->mEntries[ai]);
			swap(VNT_object->mEntries[i], VNT_object->mEntries[ai]);
		}
		else if (VNT_object->mEntries[i].IsUseDiffuse() && !VNT_object->mEntries[i].IsUseReflaction() && !VNT_object->mEntries[i].IsUseSpecular())
		{
			bi++;
			ci++;
			di++;
			swap(VNT_object->mEntries[ci], VNT_object->mEntries[bi]);
			swap(VNT_object->mEntries[i], VNT_object->mEntries[bi]);
		}
		else if (VNT_object->mEntries[i].IsUseDiffuse() && !VNT_object->mEntries[i].IsUseReflaction() && VNT_object->mEntries[i].IsUseSpecular())
		{
			ci++;
			di++;
			swap(VNT_object->mEntries[i], VNT_object->mEntries[ci]);
		}
		else if(VNT_object->mEntries[i].IsUseDiffuse() && VNT_object->mEntries[i].IsUseReflaction() && !VNT_object->mEntries[i].IsUseSpecular()) di++;
		else MessageBox(NULL, L"Error", L"Errors in sorting meshs", MB_ICONERROR | MB_OK);
	}

	c1 = ai + 1;
	c2 = bi + 1;
	c3 = ci + 1;
	c4 = di + 1;
}

void MEngine::LoadModel::SortVNTTObject(void)
{
	/*uint32_t no_texture_offset = (-1);
	uint32_t diff_texture_offset = (-1);

	for (register uint32_t i = 0; i < VNTT_object->mEntries.size(); i++)
	{
		if (VNTT_object->mEntries[i].IsUseDiffuse() && !VNTT_object->mEntries[i].IsUseReflaction())
		{
			diff_texture_offset++;
			swap(VNTT_object->mEntries[diff_texture_offset], VNTT_object->mEntries[i]);
		}
		else if (!VNTT_object->mEntries[i].IsUseReflaction() && !VNTT_object->mEntries[i].IsUseDiffuse())
		{
			diff_texture_offset++;
			no_texture_offset++;

			swap(VNTT_object->mEntries[diff_texture_offset], VNTT_object->mEntries[i]);
			swap(VNTT_object->mEntries[diff_texture_offset], VNTT_object->mEntries[no_texture_offset]);
		}
	}*/

	uint32_t clip_offset = (-1);
	for (register uint32_t i = 0; i < VNTT_object->mEntries.size(); i++)
	{
		if (!VNTT_object->mEntries[i].IsClipOpacity())
		{
			clip_offset++;
			swap(VNTT_object->mEntries[clip_offset], VNTT_object->mEntries[i]);
		}
	}
	clip_offset++;

	uint32_t c1, c2, c3, c4, c5, c0, c6, c7;
	SortVNTTObject(0, clip_offset, c0, c1, c2);
	SortVNTTObject(clip_offset, VNTT_object->mEntries.size(), c3, c4, c5);

	/*uint32_t last_index = 0;
	for (register uint32_t i = 0; i < VNTT_object->mEntries.size() && !(VNTT_object->mEntries[i].IsUseDiffuse()); i++, last_index++);
	c1 = last_index;
	for (register uint32_t i = last_index; i < VNTT_object->mEntries.size() && (!VNTT_object->mEntries[i].IsUseReflaction()) && VNTT_object->mEntries[i].IsClipOpacity(); i++, last_index++);
	c2 = last_index;
	//clip_offset
	//[0;c1) [c1; c2) [c2; clip_offset)
	//[clip_offset;c3) [c3; c4) [c4; VNT_object->mEntries.size())
	//bool mpl = VNT_object->mEntries[138].IsClipOpacity();
	for (register uint32_t i = last_index; i < VNTT_object->mEntries.size() && (!VNTT_object->mEntries[i].IsUseDiffuse()); i++, last_index++);
	c3 = last_index;
	for (register uint32_t i = last_index; i < VNTT_object->mEntries.size() && (!VNTT_object->mEntries[i].IsUseReflaction()); i++, last_index++);
	c4 = last_index;
	c = VNTT_object->mEntries.size();*/

	VNTT_object_list.c0 = c0;
	VNTT_object_list.c1 = c1;
	VNTT_object_list.c2 = c2;
	VNTT_object_list.c3 = c3;
	VNTT_object_list.c4 = c4;
	VNTT_object_list.c5 = c5;
	VNTT_object_list.c6 = 0;
	VNTT_object_list.c7 = 0;
	VNTT_object_list.clip_offset = clip_offset;
}

void MEngine::LoadModel::SortVNTTObject(uint32_t start, uint32_t end, uint32_t &c1, uint32_t &c2, uint32_t &c3)
{
	//переделать так как не может быть варианта не использующего диффузную текстуру
	/*uint32_t no_texture_offset = (-1) + start;
	uint32_t diff_texture_offset = (-1) + start;
	//VNT_object->mEntries[i].IsClipOpacity();
	for (register uint32_t i = start; i < end; i++)
	{
		if (VNTT_object->mEntries[i].IsUseDiffuse() && !VNTT_object->mEntries[i].IsUseReflaction())
		{
			diff_texture_offset++;
			swap(VNTT_object->mEntries[diff_texture_offset], VNTT_object->mEntries[i]);
		}
		else if (!VNTT_object->mEntries[i].IsUseReflaction() && !VNTT_object->mEntries[i].IsUseDiffuse())
		{
			diff_texture_offset++;
			no_texture_offset++;

			swap(VNTT_object->mEntries[diff_texture_offset], VNTT_object->mEntries[i]);
			swap(VNTT_object->mEntries[diff_texture_offset], VNTT_object->mEntries[no_texture_offset]);
		}
	}*/

	int32_t ai = (-1) + start;//categori 1)
	int32_t bi = (-1) + start;//categori 2)
	int32_t ci = (-1) + start;//categori 3)
	for (register int32_t i = start; i < end; i++)
	{
		if (!VNTT_object->mEntries[i].IsUseReflaction() && !VNTT_object->mEntries[i].IsUseSpecular() && VNTT_object->mEntries[i].IsUseDiffuse())
		{
			ai++;
			bi++;
			ci++;
			swap(VNTT_object->mEntries[bi], VNTT_object->mEntries[ai]);
			swap(VNTT_object->mEntries[i], VNTT_object->mEntries[ai]);
		}
		else if (!VNTT_object->mEntries[i].IsUseReflaction() && VNTT_object->mEntries[i].IsUseSpecular() && VNTT_object->mEntries[i].IsUseDiffuse())
		{
			bi++;
			ci++;
			swap(VNTT_object->mEntries[i], VNTT_object->mEntries[bi]);
		}
		else if(VNTT_object->mEntries[i].IsUseReflaction() && VNTT_object->mEntries[i].IsUseSpecular() && VNTT_object->mEntries[i].IsUseDiffuse())ci++;
		else MessageBox(NULL, L"Error", L"Errors in sorting meshs", MB_ICONERROR | MB_OK);
	}

	c1 = ai + 1;
	c2 = bi + 1;
	c3 = ci + 1;
}

MEngine::MDrawList MEngine::LoadModel::GetObjectListVNT(void)
{
	return VNT_object_list;
}

MEngine::MDrawList MEngine::LoadModel::GetObjectListVNTT(void)
{
	return VNTT_object_list;
}

MEngine::MResource *MEngine::LoadModel::GetResource(void)
{
	return mresource;
}

MEngine::MObjects *MEngine::LoadModel::GetVNTMesh(void)
{
	return VNT_object;
}

MEngine::MObjects *MEngine::LoadModel::GetVNTTMesh(void) 
{
	return VNTT_object;
}