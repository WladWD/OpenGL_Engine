#include "LoadTexture.h"

Resource::LoadTexture::LoadTexture(MapResource * map_resource) : map_resource(map_resource)
{
}

Resource::LoadTexture::~LoadTexture()
{
}

Resource::GLTextureResource Resource::LoadTexture::LoadMapFromFileFreeImage(string texture_name)
{
	cur_use_alpha = false;
	
	FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;
	FIBITMAP *fib(0);
	/*#include <cstdlib>

// wchar_t to char 
char* wtoc(const wchar_t* w, size_t max)
{
	char* c = new char[max];
	wcstombs(c, w, max);
	return c;
}

// char to wchar_t 
wchar_t* ctow(const char* c, size_t max)
{
	wchar_t* w = new wchar_t[max];
	mbstowcs(w, c, max);
	return w;
}*/
	fif = FreeImage_GetFileType(texture_name.c_str());
	if (fif == FIF_UNKNOWN)
	{
		MessageBoxA(NULL, texture_name.c_str(), "Unknow file format", MB_OK);
		throw 1;
	}

	if (FreeImage_FIFSupportsReading(fif))fib = FreeImage_Load(fif, texture_name.c_str());

	if (!fib)
	{
		MessageBoxA(NULL, texture_name.c_str(), "Unknow file format", MB_OK);
		throw 1;
	}

	BYTE *data_pointer = FreeImage_GetBits(fib);
	int width = FreeImage_GetWidth(fib);
	int height = FreeImage_GetHeight(fib);
	int BPP = FreeImage_GetBPP(fib);
	
	if (!width || !height || !BPP)
	{
		MessageBox(NULL, L"Ошибка чтения файла", L"Unknow file format", MB_OK);
		throw 1;
	}

	cur_use_alpha = FreeImage_IsTransparent(fib);

	/*uint32_t size_m = texture_name.length() + 1;
	wchar_t* w_name = new wchar_t[size_m];
	mbstowcs(w_name, texture_name.c_str(), size_m);
	w_name[size_m - 1] = 0;
	delete[] w_name;*/

	HRESULT hr;
	Resource::GLTextureResource texture_resource;

	GLuint format;
	GLuint texture;

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	texture_resource.texture = texture;
	texture_resource.texture_target = GL_TEXTURE_2D;

	GLenum internal_format;
	GLenum type_pixel;
	type_pixel = GL_UNSIGNED_BYTE;

	switch (BPP)
	{
	case 32: {internal_format = GL_RGBA; format = GL_RGBA8; }break;
	case 24: {internal_format = GL_BGR; format = GL_RGB8; }break;
	case 16: {internal_format = GL_RG; format = GL_RG8; }break;
	case 8: {internal_format = GL_R; format = GL_R8; }break;
	default:
	{
		MessageBox(NULL, L"Ошибка чтения файла", L"Unknow file format", MB_OK);
		throw 1;
	}; break;
	}
	

	glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, internal_format, type_pixel, data_pointer);
	glGenerateMipmap(GL_TEXTURE_2D);

	GLint g_nMaxAnisotropy;
	glGetIntegerv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &g_nMaxAnisotropy);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT,
		g_nMaxAnisotropy - 0.1);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);


	FreeImage_Unload(fib);
	glBindTexture(GL_TEXTURE_2D, 0);
	return texture_resource;
}

Resource::GLTextureResource Resource::LoadTexture::LoadMapFromFileDirectXTex(string texture_name) 
{
	cur_use_alpha = false;

	wchar_t* w_texture_name = new wchar_t[texture_name.length() + 1];
	mbstowcs(w_texture_name, texture_name.c_str(), texture_name.length() + 1);

	string sub3 = texture_name.substr(texture_name.size() - 3, 3);
	DirectX::ScratchImage image;
	DirectX::TexMetadata metadata;
	HRESULT hr;
	if (sub3 == "tga")
	{
		hr = LoadFromTGAFile(w_texture_name, nullptr, image);
	}
	else if (sub3 == "dds")
	{
		hr = LoadFromDDSFile(w_texture_name, DirectX::DDS_FLAGS_NONE, nullptr, image);
	}
	else
	{
		hr = LoadFromWICFile(w_texture_name, DirectX::WIC_FLAGS_NONE, nullptr, image);
	}
	if (FAILED(hr))
	{
		MessageBox(NULL, w_texture_name, L"Ошибка Файл не найден", MB_OK);
		throw L"Incorrect File Format";
	}

	delete[] w_texture_name;
	Resource::GLTextureResource texture_resource;
	metadata = image.GetMetadata();

	GLuint format;
	GLuint texture;
	glGenTextures(1, &texture);
	texture_resource.texture = texture;

	GLenum internal_format;
	GLenum type_pixel;
	GetInteralFormat(metadata.format, internal_format, type_pixel);
	format = GetExtermalFormat(metadata.format);

	cur_use_alpha = !image.IsAlphaAllOpaque();//alpha
	//cur_use_alpha - это значит используеться ли альфа, если еспользуеться то true иначе false
	switch (metadata.dimension)
	{
	case  DirectX::TEX_DIMENSION_TEXTURE1D: 
	{
		glBindTexture(GL_TEXTURE_1D, texture);
		glTexImage1D(GL_TEXTURE_1D, 0, format, metadata.width, 0, internal_format, type_pixel, image.GetImages()->pixels);
		texture_resource.texture_target = GL_TEXTURE_1D;

		glGenerateMipmap(GL_TEXTURE_1D);
	}break;
	case  DirectX::TEX_DIMENSION_TEXTURE2D:
	{
		//type_pixel = GL_UNSIGNED_BYTE;

		//internal_format = GL_RGB; format = GL_RGB8;
		if (!metadata.IsCubemap()) 
		{
			glBindTexture(GL_TEXTURE_2D, texture);
			glTexImage2D(GL_TEXTURE_2D, 0, format, metadata.width, metadata.height, 0, internal_format, type_pixel, image.GetImages()->pixels);
			texture_resource.texture_target = GL_TEXTURE_2D;

			glGenerateMipmap(GL_TEXTURE_2D);

			GLint g_nMaxAnisotropy;
			glGetIntegerv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &g_nMaxAnisotropy);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT,
				g_nMaxAnisotropy - 0.1);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		}
		else 
		{
			/*Cubemaps are a texture type, using the type GL_TEXTURE_CUBE_MAP.

If OpenGL 4.3 or ARB_texture_storage is available, then immutable storage for a cubemap texture can be allocated with glTexStorage2D​. The width and height must be identical, but need not be powers of two.

To allocate mutable storage for the 6 faces of a cubemap's mipmap chain, bind the texture to GL_TEXTURE_CUBE_MAP. Then call glTexImage2D​ 6 times, using the same size, mipmap level​, and internalformat​. The target​ parameter is not GL_TEXTURE_CUBE_MAP; instead, it specifies which of the 6 faces of the cubemap to specify. These faces are:

    GL_TEXTURE_CUBE_MAP_POSITIVE_X
    GL_TEXTURE_CUBE_MAP_NEGATIVE_X
    GL_TEXTURE_CUBE_MAP_POSITIVE_Y
    GL_TEXTURE_CUBE_MAP_NEGATIVE_Y
    GL_TEXTURE_CUBE_MAP_POSITIVE_Z
    GL_TEXTURE_CUBE_MAP_NEGATIVE_Z

Cubemaps may have mipmaps, but each face must have the same number of mipmaps. Cubemaps can use any of the filtering modes and other texture parameters. */
			//загрузить кубическую текстуру
			glBindTexture(GL_TEXTURE_CUBE_MAP, texture);

			uint32_t depth_pitch = image.GetImages()->slicePitch;
			uint32_t depth_pitch_lp = image.GetPixelsSize() / 6;
			for (register int i = 0; i < 6; i++)
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, format, metadata.width, metadata.height, 0, internal_format, type_pixel, image.GetImages()->pixels + i * depth_pitch_lp);
			
			texture_resource.texture_target = GL_TEXTURE_CUBE_MAP;

			GLint g_nMaxAnisotropy;
			glGetIntegerv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &g_nMaxAnisotropy);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAX_ANISOTROPY_EXT,
				g_nMaxAnisotropy - 0.1);

			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_BORDER);

			glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

			glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
		}
	}break;
	case  DirectX::TEX_DIMENSION_TEXTURE3D:
	{
		glBindTexture(GL_TEXTURE_3D, texture);
		glTexImage3D(GL_TEXTURE_3D, 0, format, metadata.width, metadata.height, metadata.depth, 0, internal_format, type_pixel, image.GetImages()->pixels);
		texture_resource.texture_target = GL_TEXTURE_3D;

		glGenerateMipmap(GL_TEXTURE_3D);
	}break;
	}

	image.Release();
	return texture_resource;
	//http://www.gamedev.ru/code/forum/?id=93718
	//https://www.opengl.org/wiki/Framebuffer_Object    - cubmap render
}

Resource::GLTextureResource Resource::LoadTexture::LoadTexture2DArrayFromFileDirectXTex(string texture_name, uint32_t max_count)
{
	cur_use_alpha = false;

	uint32_t name_x = 0;
	Resource::GLTextureResource texture_resource;
	GLuint texture;
	glGenTextures(1, &texture);
	texture_resource.texture = texture;
	texture_resource.texture_target = GL_TEXTURE_2D_ARRAY;

	glBindTexture(GL_TEXTURE_2D_ARRAY, texture);
	//glTexStorage3D(GL_TEXTURE_2D_ARRAY, 1, GL_R8, 16, 526, 370);
	//glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_R8, 16, 526, 370, 0, GL_R8, GL_UNSIGNED_BYTE, NULL);
	GLenum internal_format;
	GLenum type_pixel;
	GLuint format;
	GLuint width, height;


	uint8_t *pixels;// = new uint8_t[16 * 526 * 370];
	bool init = false;
	while (name_x < max_count) 
	{
		wchar_t* w_texture_name = new wchar_t[texture_name.length() + 1];
		mbstowcs(w_texture_name, texture_name.c_str(), texture_name.length() + 1);

		wchar_t c[10];
		wsprintf(c, L"%04d", name_x);

		wstring name_s = w_texture_name;
		name_s = name_s.substr(0, name_s.size() - 4) + c + name_s.substr(name_s.size() - 4, 4);
		const wchar_t *str_m = name_s.c_str();

		string sub3 = texture_name.substr(texture_name.size() - 3, 3);
		DirectX::ScratchImage image;
		DirectX::TexMetadata metadata;
		HRESULT hr;
		if (sub3 == "tga")
		{
			hr = LoadFromTGAFile(str_m, nullptr, image);
		}
		else if (sub3 == "dds")
		{
			hr = LoadFromDDSFile(str_m, DirectX::DDS_FLAGS_NONE, nullptr, image);
		}
		else
		{
			hr = LoadFromWICFile(str_m, DirectX::WIC_FLAGS_NONE, nullptr, image);
		}
		if (FAILED(hr))
		{
			MessageBox(NULL, w_texture_name, L"Ошибка Файл не найден", MB_OK);
			throw L"Incorrect File Format";
		}

		delete[] w_texture_name;
		metadata = image.GetMetadata();


		GetInteralFormat(metadata.format, internal_format, type_pixel);
		format = GetExtermalFormat(metadata.format);
		width = metadata.width;
		height = metadata.height;
		uint32_t bpp = 1;// - only for r8 texture format 

		if (!init) 
		{
			pixels = new uint8_t[height*width*max_count*bpp];
			init = true;
		}
		memcpy(pixels + height*width*name_x*bpp, image.GetImages()->pixels, height*width * sizeof(uint8_t)*bpp);

		//glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, format, metadata.width, metadata.height, name_x, 0, internal_format, type_pixel, image.GetImages()->pixels);
		//glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, name_x, metadata.width, metadata.height, 1, internal_format, type_pixel, image.GetImages()->pixels);

		cur_use_alpha = !image.IsAlphaAllOpaque();
		image.Release();
		name_x++;
	}

	glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, format, width, height, max_count, 0, internal_format, type_pixel, pixels);
	delete[] pixels;

	glGenerateMipmap(GL_TEXTURE_2D_ARRAY);
	GLint g_nMaxAnisotropy;
	glGetIntegerv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &g_nMaxAnisotropy);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAX_ANISOTROPY_EXT,
		g_nMaxAnisotropy - 0.1);

	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_REPEAT);

	return texture_resource;
}

GLenum Resource::LoadTexture::GetExtermalFormat(DXGI_FORMAT dxgi_format)
{
	switch (dxgi_format)
	{
	case DXGI_FORMAT_R32G32B32A32_TYPELESS: {return GL_RGBA32F; }
	case DXGI_FORMAT_R32G32B32A32_FLOAT: {return GL_RGBA32F; }
	case DXGI_FORMAT_R32G32B32A32_UINT: {return GL_RGBA32UI; }
	case DXGI_FORMAT_R32G32B32A32_SINT: {return GL_RGBA32I; }

	case DXGI_FORMAT_R32G32B32_TYPELESS: {return GL_RGB32F; }
	case DXGI_FORMAT_R32G32B32_FLOAT: {return GL_RGB32F; }
	case DXGI_FORMAT_R32G32B32_UINT: {return GL_RGB32UI; }
	case DXGI_FORMAT_R32G32B32_SINT: {return GL_RGB32I; }

	case DXGI_FORMAT_R16G16B16A16_TYPELESS: {return GL_RGBA16F; }
	case DXGI_FORMAT_R16G16B16A16_FLOAT: {return GL_RGBA16F; }
	case DXGI_FORMAT_R16G16B16A16_UINT: {return GL_RGBA32UI; }
	case DXGI_FORMAT_R16G16B16A16_SNORM: {return GL_RGBA16_SNORM; }
	case DXGI_FORMAT_R16G16B16A16_SINT: {return GL_RGBA32I; }

	case DXGI_FORMAT_R32G32_TYPELESS: {return GL_RG32F; }
	case DXGI_FORMAT_R32G32_FLOAT: {return GL_RG32F; }
	case DXGI_FORMAT_R32G32_UINT: {return GL_RG32UI; }
	case DXGI_FORMAT_R32G32_SINT: {return GL_RG32I; }

	case DXGI_FORMAT_R10G10B10A2_TYPELESS: {return GL_RGB10_A2; }
	case DXGI_FORMAT_R10G10B10A2_UNORM: {return GL_RGB10_A2; }
	case DXGI_FORMAT_R10G10B10A2_UINT: {return GL_RGB10_A2UI; }
	case DXGI_FORMAT_R11G11B10_FLOAT: {return GL_R11F_G11F_B10F; }

	case DXGI_FORMAT_R8G8B8A8_TYPELESS: {return GL_RGBA8; }
	case DXGI_FORMAT_R8G8B8A8_UNORM: {return GL_RGBA8; }//GL_RGBA8
	case DXGI_FORMAT_R8G8B8A8_UNORM_SRGB: {return GL_RGBA8; }//GL_RGBA8_SNORM
	case DXGI_FORMAT_R8G8B8A8_UINT: {return GL_RGBA8; }//GL_RGBA8UI
	case DXGI_FORMAT_R8G8B8A8_SNORM: {return GL_RGBA8; }//GL_RGBA8_SNORM
	case DXGI_FORMAT_R8G8B8A8_SINT: {return GL_RGBA8; }//GL_RGBA8I

	case DXGI_FORMAT_R16G16_TYPELESS: {return GL_RGBA16F; }
	case DXGI_FORMAT_R16G16_FLOAT: {return GL_RGBA16F; }
	case DXGI_FORMAT_R16G16_UNORM: {return GL_RGBA16; }
	case DXGI_FORMAT_R16G16_UINT: {return GL_RGBA16UI; }
	case DXGI_FORMAT_R16G16_SNORM: {return GL_RGBA16_SNORM; }
	case DXGI_FORMAT_R16G16_SINT: {return GL_RGBA16I; }

	case DXGI_FORMAT_R32_TYPELESS: {return GL_R32F; }
	case DXGI_FORMAT_R32_FLOAT: {return GL_R32F; }
	case DXGI_FORMAT_R32_UINT: {return GL_R32UI; }
	case DXGI_FORMAT_R32_SINT: {return GL_R32I; }

	case DXGI_FORMAT_R8G8_TYPELESS: {return GL_RG8; }
	case DXGI_FORMAT_R8G8_UNORM: {return GL_RG8; }
	case DXGI_FORMAT_R8G8_UINT: {return GL_RG8UI; }
	case DXGI_FORMAT_R8G8_SNORM: {return GL_RG8_SNORM; }
	case DXGI_FORMAT_R8G8_SINT: {return GL_RG8I; }

	case DXGI_FORMAT_R16_TYPELESS: {return GL_R16; }//GL_R16F
	case DXGI_FORMAT_R16_FLOAT: {return GL_R16; }//GL_R16F
	case DXGI_FORMAT_R16_UNORM: {return GL_R16; }//GL_R16
	case DXGI_FORMAT_R16_UINT: {return GL_R8; }//GL_R16UI
	case DXGI_FORMAT_R16_SNORM: {return GL_R8; }//GL_R16_SNORM
	case DXGI_FORMAT_R16_SINT: {return GL_R8; }//GL_R16I
	case DXGI_FORMAT_R8_TYPELESS: {return GL_R8; }//GL_R8
	case DXGI_FORMAT_R8_UNORM: {return GL_R8; }//GL_R8
	case DXGI_FORMAT_R8_UINT: {return GL_R8; }//GL_R8UI
	case DXGI_FORMAT_R8_SNORM: {return GL_R8; }//GL_R8_SNORM
	case DXGI_FORMAT_R8_SINT: {return GL_R8; }//GL_R8I

	case DXGI_FORMAT_R9G9B9E5_SHAREDEXP: {return GL_RGB9_E5; }
	case DXGI_FORMAT_B8G8R8A8_UNORM: {return GL_RGBA; }
	case DXGI_FORMAT_B8G8R8X8_UNORM: {return GL_RGBA; }//GL_RGB8 GL_BGR
	case DXGI_FORMAT_B8G8R8A8_TYPELESS: {return GL_RGBA; }
	case DXGI_FORMAT_B8G8R8A8_UNORM_SRGB: {return GL_RGBA; }
	case DXGI_FORMAT_B8G8R8X8_TYPELESS: {return GL_RGBA; }
	case DXGI_FORMAT_B8G8R8X8_UNORM_SRGB: {return GL_RGBA; }
	}
	throw L"Formal no Found!";
}

void Resource::LoadTexture::GetInteralFormat(DXGI_FORMAT dxgi_format, GLenum &format, GLenum &type_pixel)
{
	switch (dxgi_format)
	{
	case DXGI_FORMAT_R32G32B32A32_TYPELESS: { format = GL_RGBA;  type_pixel = GL_FLOAT; }return;
	case DXGI_FORMAT_R32G32B32A32_FLOAT: { format = GL_RGBA;  type_pixel = GL_FLOAT; }return;
	case DXGI_FORMAT_R32G32B32A32_UINT: { format = GL_RGBA;  type_pixel = GL_UNSIGNED_INT; }return;
	case DXGI_FORMAT_R32G32B32A32_SINT: { format = GL_RGBA;  type_pixel = GL_INT; }return;

	case DXGI_FORMAT_R32G32B32_TYPELESS: { format = GL_RGB;  type_pixel = GL_FLOAT; }return;
	case DXGI_FORMAT_R32G32B32_FLOAT: { format = GL_RGB;  type_pixel = GL_FLOAT; }return;
	case DXGI_FORMAT_R32G32B32_UINT: { format = GL_RGB;  type_pixel = GL_UNSIGNED_INT; }return;
	case DXGI_FORMAT_R32G32B32_SINT: { format = GL_RGB;  type_pixel = GL_INT; }return;

	case DXGI_FORMAT_R16G16B16A16_TYPELESS: { format = GL_RGBA;  type_pixel = GL_FLOAT; }return;
	case DXGI_FORMAT_R16G16B16A16_FLOAT: { format = GL_RGBA;  type_pixel = GL_FLOAT; }return;
	case DXGI_FORMAT_R16G16B16A16_UINT: { format = GL_RGBA;  type_pixel = GL_UNSIGNED_INT; }return;
	case DXGI_FORMAT_R16G16B16A16_SNORM: { format = GL_RGBA;  type_pixel = GL_FLOAT; }return;
	case DXGI_FORMAT_R16G16B16A16_SINT: { format = GL_RGBA;  type_pixel = GL_INT; }return;

	case DXGI_FORMAT_R32G32_TYPELESS: { format = GL_RG;  type_pixel = GL_FLOAT; }return;
	case DXGI_FORMAT_R32G32_FLOAT: { format = GL_RG;  type_pixel = GL_FLOAT; }return;
	case DXGI_FORMAT_R32G32_UINT: { format = GL_RG;  type_pixel = GL_FLOAT; }return;
	case DXGI_FORMAT_R32G32_SINT: { format = GL_RG;  type_pixel = GL_FLOAT; }return;

	case DXGI_FORMAT_R10G10B10A2_TYPELESS: {throw L"Formal no Found!"; }return;
	case DXGI_FORMAT_R10G10B10A2_UNORM: { throw L"Formal no Found!"; }return;
	case DXGI_FORMAT_R10G10B10A2_UINT: { throw L"Formal no Found!"; }return;
	case DXGI_FORMAT_R11G11B10_FLOAT: { throw L"Formal no Found!"; }return;

	case DXGI_FORMAT_R8G8B8A8_TYPELESS: { format = GL_RGBA;  type_pixel = GL_UNSIGNED_BYTE; }return;
	case DXGI_FORMAT_R8G8B8A8_UNORM: { format = GL_RGBA;  type_pixel = GL_UNSIGNED_BYTE; }return;
	case DXGI_FORMAT_R8G8B8A8_UNORM_SRGB: { format = GL_RGBA;  type_pixel = GL_BYTE; }return;
	case DXGI_FORMAT_R8G8B8A8_UINT: { format = GL_RGBA;  type_pixel = GL_UNSIGNED_BYTE; }return;
	case DXGI_FORMAT_R8G8B8A8_SNORM: { format = GL_RGBA;  type_pixel = GL_BYTE; }return;
	case DXGI_FORMAT_R8G8B8A8_SINT: {format = GL_RGBA;  type_pixel = GL_BYTE; }return;

	case DXGI_FORMAT_R16G16_TYPELESS: { format = GL_RG;  type_pixel = GL_UNSIGNED_SHORT; }return;
	case DXGI_FORMAT_R16G16_FLOAT: { format = GL_RG;  type_pixel = GL_UNSIGNED_SHORT; }return;
	case DXGI_FORMAT_R16G16_UNORM: { format = GL_RG;  type_pixel = GL_UNSIGNED_SHORT; }return;
	case DXGI_FORMAT_R16G16_UINT: { format = GL_RG;  type_pixel = GL_UNSIGNED_SHORT; }return;
	case DXGI_FORMAT_R16G16_SNORM: { format = GL_RG;  type_pixel = GL_SHORT; }return;
	case DXGI_FORMAT_R16G16_SINT: { format = GL_RG;  type_pixel = GL_SHORT; }return;

	case DXGI_FORMAT_R32_TYPELESS: { format = GL_RED;  type_pixel = GL_FLOAT; }return;
	case DXGI_FORMAT_R32_FLOAT: { format = GL_RED;  type_pixel = GL_FLOAT; }return;
	case DXGI_FORMAT_R32_UINT: { format = GL_RED;  type_pixel = GL_INT; }return;
	case DXGI_FORMAT_R32_SINT: { format = GL_RED;  type_pixel = GL_UNSIGNED_INT; }return;

	case DXGI_FORMAT_R8G8_TYPELESS: { format = GL_RG;  type_pixel = GL_UNSIGNED_BYTE; }return;
	case DXGI_FORMAT_R8G8_UNORM: { format = GL_RG;  type_pixel = GL_UNSIGNED_BYTE; }return;
	case DXGI_FORMAT_R8G8_UINT: { format = GL_RG;  type_pixel = GL_UNSIGNED_BYTE; }return;
	case DXGI_FORMAT_R8G8_SNORM: { format = GL_RG;  type_pixel = GL_BYTE; }return;
	case DXGI_FORMAT_R8G8_SINT: { format = GL_RG;  type_pixel = GL_BYTE; }return;

	case DXGI_FORMAT_R16_TYPELESS: { format = GL_RED;  type_pixel = GL_UNSIGNED_SHORT; }return;
	case DXGI_FORMAT_R16_FLOAT: { format = GL_RED;  type_pixel = GL_UNSIGNED_SHORT; }return;
	case DXGI_FORMAT_R16_UNORM: { format = GL_RED;  type_pixel = GL_UNSIGNED_SHORT; }return;
	case DXGI_FORMAT_R16_UINT: { format = GL_RED;  type_pixel = GL_UNSIGNED_SHORT; }return;
	case DXGI_FORMAT_R16_SNORM: { format = GL_RED;  type_pixel = GL_SHORT; }return;
	case DXGI_FORMAT_R16_SINT: { format = GL_RED;  type_pixel = GL_SHORT; }return;

	case DXGI_FORMAT_R8_TYPELESS: { format = GL_RED;  type_pixel = GL_UNSIGNED_BYTE; }return;
	case DXGI_FORMAT_R8_UNORM: { format = GL_RED;  type_pixel = GL_UNSIGNED_BYTE; }return;//GL_R GL_RED
	case DXGI_FORMAT_R8_UINT: { format = GL_RED;  type_pixel = GL_UNSIGNED_BYTE; }return;
	case DXGI_FORMAT_R8_SNORM: { format = GL_RED;  type_pixel = GL_BYTE; }return;
	case DXGI_FORMAT_R8_SINT: { format = GL_RED;  type_pixel = GL_BYTE; }return;

	case DXGI_FORMAT_R9G9B9E5_SHAREDEXP: { throw L"Formal no Found!"; }return;
	case DXGI_FORMAT_B8G8R8A8_UNORM: { format = GL_BGRA;  type_pixel = GL_UNSIGNED_BYTE; }return;
	case DXGI_FORMAT_B8G8R8X8_UNORM: { format = GL_BGRA;  type_pixel = GL_UNSIGNED_BYTE; }return;
	case DXGI_FORMAT_B8G8R8A8_TYPELESS: { format = GL_BGRA;  type_pixel = GL_UNSIGNED_BYTE; }return;
	case DXGI_FORMAT_B8G8R8A8_UNORM_SRGB: { format = GL_BGRA;  type_pixel = GL_BYTE; }return;
	case DXGI_FORMAT_B8G8R8X8_TYPELESS: { format = GL_BGRA;  type_pixel = GL_UNSIGNED_BYTE; }return;
	case DXGI_FORMAT_B8G8R8X8_UNORM_SRGB: { format = GL_BGRA;  type_pixel = GL_BYTE; }return;
	}
	throw L"Formal no Found!";

	/*
	GL_COLOR_INDEX,
	GL_RED,
	GL_GREEN,
	GL_BLUE,
	GL_ALPHA,
	GL_RGB,
	GL_BGR,
	GL_RGBA,
	GL_BGRA,
	GL_LUMINANCE, and
	GL_LUMINANCE_ALPHA.
	*/
	/*
	GL_UNSIGNED_BYTE,
	GL_BYTE,
	GL_BITMAP,
	GL_UNSIGNED_SHORT,
	GL_SHORT,
	GL_UNSIGNED_INT,
	GL_INT,
	GL_FLOAT,
	*/
	/*
	GL_UNSIGNED_BYTE_3_3_2,
	GL_UNSIGNED_BYTE_2_3_3_REV,
	GL_UNSIGNED_SHORT_5_6_5,
	GL_UNSIGNED_SHORT_5_6_5_REV,
	GL_UNSIGNED_SHORT_4_4_4_4,
	GL_UNSIGNED_SHORT_4_4_4_4_REV,
	GL_UNSIGNED_SHORT_5_5_5_1,
	GL_UNSIGNED_SHORT_1_5_5_5_REV,
	GL_UNSIGNED_INT_8_8_8_8,
	GL_UNSIGNED_INT_8_8_8_8_REV,
	GL_UNSIGNED_INT_10_10_10_2, and
	GL_UNSIGNED_INT_2_10_10_10_REV.
	*/
}

GLuint Resource::LoadTexture::LoadResource(string texture_name)
{
	unsigned long resource_id;
	if (resource_id = map_resource->MapFindResource(texture_name.c_str()))
	{
		map_resource->AddReference(resource_id);
		return resource_id;
	}
	else
	{
		Resource::GLTextureResource texture_resource = LoadMapFromFileDirectXTex(texture_name);//LoadMapFromFileFreeImage  LoadMapFromFileDirectXTex
		return map_resource->AppendResource(texture_resource.texture, texture_resource.texture_target, texture_name.c_str(), cur_use_alpha);
	}
}

GLuint Resource::LoadTexture::LoadTexture2DArray(string texture_name, uint32_t max_count)
{
	unsigned long resource_id;
	if (resource_id = map_resource->MapFindResource(texture_name.c_str()))
	{
		map_resource->AddReference(resource_id);
		return resource_id;
	}
	else
	{
		Resource::GLTextureResource texture_resource = LoadTexture2DArrayFromFileDirectXTex(texture_name, max_count);//LoadMapFromFileFreeImage  LoadMapFromFileDirectXTex
		return map_resource->AppendResource(texture_resource.texture, texture_resource.texture_target, texture_name.c_str(), cur_use_alpha);
	}
}


Resource::MapResource * Resource::LoadTexture::GetMapResource()
{
	return map_resource;
}

