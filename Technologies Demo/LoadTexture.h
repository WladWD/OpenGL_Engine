#include "MapResource.h"
#include <string>
#include <FreeImage.h>
#include "DirectXTex.h"

#pragma once
namespace Resource
{
	class LoadTexture
	{
		MapResource *map_resource;
		bool cur_use_alpha;
		////////////////////////////////////////////////////////////////////////////////
		Resource::GLTextureResource LoadMapFromFileFreeImage(string texture_name);
		Resource::GLTextureResource LoadMapFromFileDirectXTex(string texture_name);
		Resource::GLTextureResource LoadTexture2DArrayFromFileDirectXTex(string texture_name, uint32_t max_count);
		GLenum GetExtermalFormat(DXGI_FORMAT dxgi_format);
		void GetInteralFormat(DXGI_FORMAT dxgi_format, GLenum &format, GLenum &type_pixel);
	public:
		LoadTexture(MapResource *map_resource);
		~LoadTexture();
		////////////////////////////////////////////////////////////////////////////////
		GLuint LoadResource(string texture_name);
		GLuint LoadTexture2DArray(string texture_name, uint32_t max_count);
		////////////////////////////////////////////////////////////////////////////////
		MapResource *GetMapResource();
	};
}
//класс загружаем материалы и сохраняет их в класс MapResource