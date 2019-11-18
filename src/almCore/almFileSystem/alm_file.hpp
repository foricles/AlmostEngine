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
	AlmFile(const std::almstring &filePath);
	AlmFile &operator=(AlmFile &&rhv);

	AlmFile(const AlmFile &rhv) = delete;
	AlmFile &operator=(const AlmFile &rhv) = delete;

	virtual ~AlmFile();


public:
	virtual AlmFile *Copy();

	bool Exist() const;
	std::almstring GetFullPath() const;
	const std::almstring & GetPath() const;

	const char * Load();
	const char * Load(const std::almstring &filepath);

	AlmFile & LoadAsync();
	AlmFile & LoadAsync(const std::almstring &filepath);
	const char *GetContentAsync();

	void Write(const char *data, uint32_t size);
	void Save();
	void SaveAsync();

private:
	char *m_data;
	uint32_t m_size;
	std::almstring m_filepath;
	std::future<void> m_future;
};

}
}

#endif // !_ALM_FILE_HPP_