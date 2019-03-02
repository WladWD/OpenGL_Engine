#include "MapResource.h"


Resource::MapResource::MapResource()
{
	const unsigned long resize_val = 256;
	//resource_real_count = 1;
	map_resource.reserve(resize_val);
	map_resource_name.reserve(resize_val);
	resource_reference_count.reserve(resize_val);

	ResourceName name_resource;
	memset(name_resource.resource_name, 0, MAX_PATH);
	GLTextureResource texture_target = { 0 };

	map_resource.push_back(texture_target);
	map_resource_name.push_back(name_resource);
	resource_reference_count.push_back(0);
	alpha_map.push_back(false);
}

Resource::MapResource::~MapResource()
{
	map_resource_name.clear();

	uint32_t tex_to_delete = 0, mi = 0;
	for (register unsigned long i = 0; i < map_resource.size(); i++) if (resource_reference_count[i]) tex_to_delete++;

	GLuint *texture = new GLuint[tex_to_delete];
	for (register unsigned long i = 0; i < map_resource.size(); i++)
	{
		//glDeleteTextures(1, &(map_resource[i]));
		if (resource_reference_count[i]) texture[mi++] = map_resource[i].texture;
	}

	glDeleteTextures(tex_to_delete, texture);
	delete[] texture;


	map_resource.clear();
	alpha_map.clear();
	resource_reference_count.clear();
}

unsigned long Resource::MapResource::MapFindResource(const char resource_name[MAX_PATH])
{
	for (register unsigned long i = 0; i < map_resource_name.size(); i++)
	{
		if (!strcmp(resource_name, map_resource_name[i].resource_name))
		{
			return i;
		}
	}
	return 0;
}

GLuint Resource::MapResource::AddReference(unsigned long resource_ID)
{
	if (!resource_reference_count[resource_ID])
	{
		throw L"Эта ссылка на текстуру уже удалена";
	}

	resource_reference_count[resource_ID]++;
	return resource_ID;
}

unsigned long Resource::MapResource::ReleaseResource(unsigned long resource_ID)
{
	unsigned long res_val = resource_reference_count[resource_ID];
	if (!res_val)
	{
		throw L"Эта ссылка на текстуру уже удалена";
	}

	res_val = resource_reference_count[resource_ID]--;
	if (!res_val)
	{
		//vector<GLuint>::iterator resource_iterator = map_resource.begin() + resource_ID;
		//vector<ResourceName>::iterator resource_name_iterator = map_resource_name.begin() + resource_ID;
		//vector<unsigned long>::iterator resource_reference_count_iterator = resource_reference_count.begin() + resource_ID;
		glDeleteTextures(1, &(map_resource[resource_ID].texture));
		map_resource[resource_ID].texture = map_resource[resource_ID].texture_target = 0;
		memset(map_resource_name[resource_ID].resource_name, 0, MAX_PATH);
		alpha_map[resource_ID] = false;
		//map_resource.erase(resource_iterator);
		//map_resource_name.erase(resource_name_iterator);
		//resource_reference_count.erase(resource_reference_count_iterator);
	}
	return res_val;
}

unsigned long Resource::MapResource::AppendResource(GLuint resource_map, GLuint resource_target, const char resource_name[MAX_PATH], bool use_alpha)
{
	ResourceName name_resource;
	memcpy(name_resource.resource_name, resource_name, MAX_PATH);

	GLTextureResource texture_target = { resource_map, resource_target };

	unsigned long resource_id = map_resource.size();
	map_resource.push_back(texture_target);
	map_resource_name.push_back(name_resource);
	resource_reference_count.push_back(1);
	alpha_map.push_back(use_alpha);
	return resource_id;
}

Resource::GLTextureResource Resource::MapResource::GetResourceByID(unsigned long resource_id)
{
	if (!resource_id)
	{
		throw L"Ошибка использоввания несуществующего ID";
	}

	return map_resource[resource_id];
}

bool Resource::MapResource::GetAlphaMap(unsigned long resource_id)
{
	return alpha_map[resource_id];
}