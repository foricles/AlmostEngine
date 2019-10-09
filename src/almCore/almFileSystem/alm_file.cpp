#include "alm_file.hpp"
#include <cstdio>

using namespace alme;
#define SAFE_DELETE_DATA(x) if(x){delete [] x; x = nullptr; }

io::AlmFile::AlmFile()
{
}

io::AlmFile::AlmFile(const std::almstring & filePath)
	: m_data(nullptr)
	, m_filepath(filePath)
{
}

io::AlmFile::AlmFile(AlmFile && rhv)
	: m_data(rhv.m_data)
{
	rhv.m_data = nullptr;
	std::swap(m_filepath, rhv.m_filepath);
}

io::AlmFile & alme::io::AlmFile::operator=(AlmFile && rhv)
{
	m_data = rhv.m_data;
	std::swap(m_filepath, rhv.m_filepath);
	rhv.m_data = nullptr;
	return *this;
}

io::AlmFile::~AlmFile()
{
	SAFE_DELETE_DATA(m_data);
}



io::AlmFile * io::AlmFile::Copy()
{
	return new AlmFile();
}

std::almstring io::AlmFile::GetFullPath() const
{
	return io::AlmFileSystem::getRootPath() + m_filepath;
}

const std::almstring & io::AlmFile::GetPath() const
{
	return m_filepath;
}

bool io::AlmFile::Exist() const
{
	bool exist = false;
	FILE *file = NULL;
#ifdef ALM_UNICODE
	_wfopen_s(&file, GetFullPath().c_str(), L"rt");
#else
	fopen(&file, GetFullPath().c_str(), "rt");
#endif // ALM_UNICODE
	exist = file;
	if (exist) fclose(file);
	return exist;
}



const char * io::AlmFile::Load()
{
	FILE *file = NULL;
#ifdef ALM_UNICODE
	_wfopen_s(&file, GetFullPath().c_str(), L"rt");
#else
	fopen(&file, GetFullPath().c_str(), "rt");
#endif // ALM_UNICODE
	if (!file)
	{
		SAFE_DELETE_DATA(m_data);
	}
	else
	{
		fseek(file, 0, SEEK_END);
		uint32_t size = ftell(file);
		m_data = new char[size + 1];
		fseek(file, 0, SEEK_SET);
		fread(m_data, size, 1, file);
		fclose(file);
	}

	return m_data;
}

const char * io::AlmFile::Load(const std::almstring &filepath)
{
	m_filepath = filepath;
	return Load();
}


io::AlmFile & alme::io::AlmFile::LoadAsync()
{
	SAFE_DELETE_DATA(m_data);
	m_future = std::async(std::launch::async, [this]() mutable { Load(); });
	return *this;
}

io::AlmFile & alme::io::AlmFile::LoadAsync(const std::almstring & filepath)
{
	m_filepath = filepath;
	return LoadAsync();
}

const char * alme::io::AlmFile::GetContentAsync()
{
	if (m_filepath.size() <= 0) return nullptr;
	if (!m_data) m_future.get();
	return m_data;
}

void alme::io::AlmFile::Write(const char * data, uint32_t size)
{
	if (!data) return;
	SAFE_DELETE_DATA(m_data);
	m_data = new char[size+1];
	memcpy_s(m_data, size, data, size);
}

void alme::io::AlmFile::Save()
{
}

void alme::io::AlmFile::SaveAsync()
{
	std::async(std::launch::async, [this]() { Save(); });
}