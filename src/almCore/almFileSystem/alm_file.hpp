#ifndef _ALM_FILE_HPP_
#define _ALM_FILE_HPP_

#include "../src/almCore/alm_platform.hpp"

namespace alme
{
namespace io
{

class AlmFile
{
public:
	AlmFile();
	AlmFile(AlmFile &&rhv) noexcept;
	AlmFile(const std::string &filePath);
	AlmFile(const std::wstring &filePath);
	AlmFile &operator=(AlmFile &&rhv) noexcept;

	AlmFile(const AlmFile &rhv) = delete;
	AlmFile &operator=(const AlmFile &rhv) = delete;

	virtual ~AlmFile();


public:
	virtual AlmFile *Copy();

	bool Exist() const;
	std::wstring GetFullPath() const;
	const std::wstring & GetPath() const;

	AlmFile & Load(bool bin = false);
	AlmFile & Load(const std::string &filepath, bool bin = false);
	AlmFile & Load(const std::wstring &filepath, bool bin = false);

	void Write(const uint8_t *data, uint32_t size, bool append = false);
	void Save(bool bin = false);

	std::vector<uint8_t> asBin() const;
	std::string asString() const;

	void c_read(uint8_t *& rhv);
	const uint8_t* c_arr() { return m_data; };

private:
	uint8_t *m_data;
	uint32_t m_size;
	int32_t  m_pathIndex;

	static std::vector<std::wstring> s_filepath;
};

}
}

#endif // !_ALM_FILE_HPP_