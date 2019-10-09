#include "alm_file.hpp"
#include <cstdio>

using namespace alme;

io::AlmFile::AlmFile()
{
}

io::AlmFile::AlmFile(const std::almstring & filePath)
	: m_data(nullptr)
	, m_size(0)
	, m_filepath(filePath)
{
}

io::AlmFile::AlmFile(AlmFile && rhv)
	: m_data(rhv.m_data)
	, m_size(rhv.m_size)
{
	rhv.m_size = 0;
	rhv.m_data = nullptr;
	std::swap(m_filepath, rhv.m_filepath);
}

io::AlmFile & alme::io::AlmFile::operator=(AlmFile && rhv)
{
	m_data = rhv.m_data;
	std::swap(m_size, rhv.m_size);
	std::swap(m_filepath, rhv.m_filepath);
	rhv.m_data = nullptr;
	return *this;
}

io::AlmFile::~AlmFile()
{
	if (m_data) 
		delete[] m_data;
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
#ifdef ALM_UNICODE
	auto file = _wfopen(GetFullPath().c_str(), L"rt");
#else
	auto file = fopen(GetFullPath().c_str(), "rt");
#endif // ALM_UNICODE
	exist = file;
	if (exist) fclose(file);
	return exist;
}



const char * io::AlmFile::Load()
{
#ifdef ALM_UNICODE
	auto file = _wfopen(GetFullPath().c_str(), L"rt");
#else
	auto file = fopen(GetFullPath().c_str(), "rt");
#endif // ALM_UNICODE
	if (!file)
	{
		m_size = 0;
		if (m_data)
		{
			delete[] m_data;
			m_data = nullptr;
		}
	}
	else
	{
		fseek(file, 0, SEEK_END);
		m_size = ftell(file);
		m_data = new char[m_size + 1];
		fseek(file, 0, SEEK_SET);
		fread(m_data, m_size, 1, file);
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
