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

	const char * Load();
	const char * Load(const std::string &filepath);
	const char * Load(const std::wstring &filepath);

	AlmFile & LoadAsync();
	AlmFile & LoadAsync(const std::string &filepath);
	AlmFile & LoadAsync(const std::wstring &filepath);
	const char *GetContentAsync();

	void Write(const char *data, uint32_t size);
	void Save();
	void SaveAsync();

public:
	std::wstring StrToWStr(const std::string &source);

private:
	char *m_data;
	uint32_t m_size;
	std::wstring m_filepath;
	std::future<void> m_future;
};

}
}

#endif // !_ALM_FILE_HPP_