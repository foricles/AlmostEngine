#ifndef _ALM_FILE_HPP_
#define _ALM_FILE_HPP_

#include "alm_flsystem.hpp"
#include <future>

namespace alme
{
namespace io
{

class AlmFile
{
public:
	AlmFile();
	AlmFile(AlmFile &&rhv);
	AlmFile(const std::string &filePath);
	AlmFile(const std::wstring &filePath);
	AlmFile &operator=(AlmFile &&rhv);

	AlmFile(const AlmFile &rhv) = delete;
	AlmFile &operator=(const AlmFile &rhv) = delete;

	virtual ~AlmFile();


public:
	virtual AlmFile *Copy();

	bool Exist() const;
	std::wstring GetFullPath() const;
	const std::wstring & GetPath() const;

	const uint8_t * Load();
	const uint8_t * Load(const std::string &filepath);
	const uint8_t * Load(const std::wstring &filepath);

	AlmFile & LoadAsync();
	AlmFile & LoadAsync(const std::string &filepath);
	AlmFile & LoadAsync(const std::wstring &filepath);
	const uint8_t *GetContentAsync();

	void Write(const uint8_t *data, uint32_t size);
	void Save();
	void SaveAsync();

	std::vector<uint8_t> asBin() const;
	std::string asString() const;

public:
	std::wstring StrToWStr(const std::string &source);

private:
	uint8_t *m_data;
	uint32_t m_size;
	std::wstring m_filepath;
	std::future<void> m_future;
};

}
}

#endif // !_ALM_FILE_HPP_