#include "alm_imageloader.hpp"
#include <libpng/png.h>
#include <jpeg/jpeglib.h>
#include "../src/almCore/alm_log.hpp"
#include "../src/almCore/almFileSystem/alm_file.hpp"

namespace alme
{
	void png_cexcept_error(png_structp png_ptr, png_const_charp msg)
	{
		ALM_LOG_ERROR("PNG:", msg);
	}

	struct alm_png_source_file
	{
		io::AlmFile* src;
		png_size_t offset;
	};
	void ReadPNGDataFromFile(png_structp png_ptr, png_bytep raw_data, png_size_t read_length)
	{
		auto src = (alm_png_source_file*)png_get_io_ptr(png_ptr);
		const png_byte* png_src = src->src->c_arr() + src->offset;
		memcpy(raw_data, png_src, read_length);
		src->offset += read_length;
	}

	void WritePNGDataFromFile(png_structp png_ptr, png_bytep data, png_size_t length)
	{
		auto src = (io::AlmFile*)png_get_io_ptr(png_ptr);
		src->Write(data, length, true);
	}
	void FlushPNG(png_structp png_ptr)
	{
		//auto src = (alm_png_source_file*)png_get_io_ptr(png_ptr);
		//src->src->Save(true);
	}
}

using namespace alme;


image::AlmImage * image::AlmImageLoader::Create(uint32_t w, uint32_t h, image::eAlmImage type)
{
	if (type == eNul)
	{
		ALM_LOG_ERROR("File type not defined!");
		return nullptr;
	}
	auto img = new AlmImage();
	img->m_width = w;
	img->m_height = h;
	img->m_type = type;
	img->m_data.resize(w * h);
	memset(img->m_data.data(), 0, img->GetSize());
	return img;
}

image::AlmImage * image::AlmImageLoader::Create(const std::string &filepath)
{
	if (!io::AlmFile(filepath).Exist())
	{
		ALM_LOG_ERROR("File type not exist:", filepath);
		return nullptr;
	}
	return CreatePNG(filepath);
}

void image::AlmImageLoader::SaveImage(image::AlmImage * img, const std::string& filepath)
{
	if (img->GetImageType() == ePng)
		SaveImagePNG(img, filepath);
	else if (img->GetImageType() == eJpg)
		SaveImageJPG(img, filepath);
	else
		ALM_LOG_ERROR("File type not defined:", filepath);
}


image::AlmImage * image::AlmImageLoader::CreatePNG(const std::string& filepath)
{
	png_structp png = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, (png_error_ptr)png_cexcept_error, (png_error_ptr)NULL);
	if (!png)
	{
		ALM_LOG_ERROR("Error at create reader:", filepath);
		return nullptr;
	};

	png_infop info = png_create_info_struct(png);
	if (!png)
	{
		ALM_LOG_ERROR("Error at create reader:", filepath);
		return nullptr;
	};

	if (setjmp(png_jmpbuf(png)))
	{
		ALM_LOG_ERROR("Error at create reader:", filepath);
		return nullptr;
	};

	io::AlmFile file;
	file.Load(filepath, true);
	alm_png_source_file source{ &file, 0 };
	png_set_read_fn(png, &source, ReadPNGDataFromFile);

	png_read_info(png, info);

	int width = png_get_image_width(png, info);
	int height = png_get_image_height(png, info);
	png_byte color_type = png_get_color_type(png, info);
	png_byte bit_depth = png_get_bit_depth(png, info);

	if (bit_depth == 16)
		png_set_strip_16(png);

	if (color_type == PNG_COLOR_TYPE_PALETTE)
		png_set_palette_to_rgb(png);

	if (color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8)
		png_set_expand_gray_1_2_4_to_8(png);

	if (png_get_valid(png, info, PNG_INFO_tRNS))
		png_set_tRNS_to_alpha(png);

	if (color_type == PNG_COLOR_TYPE_RGB ||
		color_type == PNG_COLOR_TYPE_GRAY ||
		color_type == PNG_COLOR_TYPE_PALETTE)
		png_set_filler(png, 0xFF, PNG_FILLER_AFTER);

	if (color_type == PNG_COLOR_TYPE_GRAY ||
		color_type == PNG_COLOR_TYPE_GRAY_ALPHA)
		png_set_gray_to_rgb(png);

	png_read_update_info(png, info);

	png_bytep* row_pointers = (png_bytep*)malloc(sizeof(png_bytep) * height);
	for (int y = 0; y < height; y++)
		row_pointers[y] = (png_byte*)malloc(png_get_rowbytes(png, info));

	png_read_image(png, row_pointers);
	png_destroy_read_struct(&png, &info, NULL);

	auto ret = new AlmImage();
	ret->m_width = width;
	ret->m_height = height;
	ret->m_type = ePng;
	ret->m_data.resize(width * height);

	for (int y = 0; y < height; y++) {
		png_bytep row = row_pointers[y];
		for (int x = 0; x < width; x++) {
			png_bytep px = &(row[x * 4]);
			ret->m_data[x * height + y].x = px[0];
			ret->m_data[x * height + y].y = px[1];
			ret->m_data[x * height + y].z = px[2];
			ret->m_data[x * height + y].w = px[3];
		}
	}

	for (int y = 0; y < height; y++)
		free(row_pointers[y]);
	free(row_pointers);

	return ret;
}

void image::AlmImageLoader::SaveImagePNG(image::AlmImage* img, const std::string& filepath)
{
	png_structp png = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, (png_error_ptr)png_cexcept_error, (png_error_ptr)NULL);
	if (!png)
	{
		ALM_LOG_ERROR("Error at create reader:", filepath);
		return;
	};

	png_infop info = png_create_info_struct(png);
	if (!png)
	{
		ALM_LOG_ERROR("Error at create reader:", filepath);
		return;
	};

	if (setjmp(png_jmpbuf(png)))
	{
		ALM_LOG_ERROR("Error at create reader:", filepath);
		return;
	};

	io::AlmFile file(filepath);
	png_set_write_fn(png, &file, WritePNGDataFromFile, FlushPNG);

	png_set_IHDR(
		png, info,
		img->GetWight(), img->GetHeight(),
		8,
		PNG_COLOR_TYPE_RGBA,
		PNG_INTERLACE_NONE,
		PNG_COMPRESSION_TYPE_DEFAULT,
		PNG_FILTER_TYPE_DEFAULT
	);
	png_write_info(png, info);

	auto row_pointers = (png_byte*)malloc(png_get_rowbytes(png, info));
	for (int x = 0; x < img->GetHeight(); x++) {
		for (int y = 0; y < img->GetWight(); y++) {
			png_bytep px = &(row_pointers[y * 4]);
			px[0] = static_cast<png_byte>(img->m_data[y * img->GetHeight() + x].x);
			px[1] = static_cast<png_byte>(img->m_data[y * img->GetHeight() + x].y);
			px[2] = static_cast<png_byte>(img->m_data[y * img->GetHeight() + x].z);
			px[3] = static_cast<png_byte>(img->m_data[y * img->GetHeight() + x].w);
		}
		png_write_row(png, row_pointers);
	}

	free(row_pointers);
	png_write_end(png, info);
	png_destroy_write_struct(&png, &info);
	file.Save(true);
}

image::AlmImage* image::AlmImageLoader::CreateJPG(const std::string& filepath)
{
	return nullptr;
}


void image::AlmImageLoader::SaveImageJPG(image::AlmImage* img, const std::string& filepath)
{

}
