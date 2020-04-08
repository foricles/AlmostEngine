#ifndef _ALM_IMAGE_HPP_
#define _ALM_IMAGE_HPP_

#include "../src/almCore/almMath/vec4.hpp"
#include "../src/almCore/alm_platform.hpp"

namespace alme
{
	namespace image
	{
		using img_arr = std::vector<kmu::vec4>;

		enum eAlmImage
		{
			eNul,
			ePng,
			eJpg
		};

		class AlmImage
		{
		friend class AlmImageLoader;
		public:
			virtual ~AlmImage() {};
			int32_t					GetWight() const { return m_width; };
			int32_t					GetHeight() const { return m_height; };
			eAlmImage				GetImageType() const { return m_type; };
			const img_arr &			GetData() const { return m_data; };
			int32_t					GetSize() const { return m_data.size() * sizeof(kmu::vec4); };
			kmu::vec4				GetPixel(uint32_t i, uint32_t j) const { return m_data[i * m_height + j]; };
			void					SetPixel(uint32_t i, uint32_t j, const kmu::vec4& color) { m_data[i * m_height + j] = color; };
		protected:
			int32_t					m_width;
			int32_t					m_height;
			eAlmImage				m_type;
			img_arr					m_data;
		private:
			AlmImage() : m_width(0), m_height(0), m_type(eNul) { }
		};
	}
}

#endif //!_ALM_IMAGE_HPP_
