#ifndef _ALM_PNG_IMAGE_HPP_
#define _ALM_PNG_IMAGE_HPP_

#include "alm_image.hpp"

namespace alme
{
	namespace image
	{
		class AlmImageLoader
		{
		public:
			static image::AlmImage*		Create(uint32_t w, uint32_t h, image::eAlmImage type);
			static image::AlmImage*		Create(const std::string& filepath);
			static void					SaveImage(image::AlmImage* img, const std::string& filepath);
		private:
			static image::AlmImage*		CreatePNG(const std::string& filepath);
			static image::AlmImage*		CreateJPG(const std::string& filepath);
			static void					SaveImagePNG(image::AlmImage* img, const std::string& filepath);
			static void					SaveImageJPG(image::AlmImage* img, const std::string& filepath);
		};
	}
}

#endif //!_ALM_PNG_IMAGE_HPP_
