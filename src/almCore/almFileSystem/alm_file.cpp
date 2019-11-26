#include "alm_file.hpp"
#include <cstdio>

using namespace alme;
#define SAFE_DELETE_DATA(x) if(x){delete [] x; x = nullptr; }

io::AlmFile::AlmFile()
{
}

io::AlmFile::AlmFile(const std::wstring & filePath)
	: m_data(nullptr)
	, m_filepath(filePath)
{
}

io::AlmFile::AlmFile(const std::string & filePath)
	: AlmFile(StrToWStr(filePath))
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

std::wstring io::AlmFile::GetFullPath() const
{
	return io::AlmFileSystem::getRootPath() + m_filepath;
}

const std::wstring & io::AlmFile::GetPath() const
{
	return m_filepath;
}

bool io::AlmFile::Exist() const
{
	bool exist = false;
	FILE *file = NULL;
	_wfopen_s(&file, GetFullPath().c_str(), L"rt");
	exist = file;
	if (exist) fclose(file);
	return exist;
}



const char * io::AlmFile::Load()
{
	FILE *file = NULL;
	_wfopen_s(&file, GetFullPath().c_str(), L"rt");
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
		fread(m_data, sizeof(char), size, file);
		fclose(file);
		m_size = size;
	}

	return m_data;
}

const char * io::AlmFile::Load(const std::wstring &filepath)
{
	m_filepath = filepath;
	return Load();
}

const char * io::AlmFile::Load(const std::string &filepath)
{
	return Load(StrToWStr(filepath));
}


io::AlmFile & io::AlmFile::LoadAsync()
{
	SAFE_DELETE_DATA(m_data);
	m_future = std::async(std::launch::async, [this]() mutable { Load(); });
	return *this;
}

io::AlmFile & io::AlmFile::LoadAsync(const std::wstring & filepath)
{
	m_filepath = filepath;
	return LoadAsync();
}

io::AlmFile & io::AlmFile::LoadAsync(const std::string & filepath)
{
	return LoadAsync(StrToWStr(filepath));
}

const char * io::AlmFile::GetContentAsync()
{
	if (m_filepath.size() <= 0) return nullptr;
	if (!m_data) m_future.get();
	return m_data;
}

void io::AlmFile::Write(const char * data, uint32_t size)
{
	if (!data) return;
	SAFE_DELETE_DATA(m_data);
	m_data = new char[size+1];
	memcpy_s(m_data, size, data, size);
	m_size = size;
}

void io::AlmFile::Save()
{
	FILE *file = NULL;
	_wfopen_s(&file, GetFullPath().c_str(), L"w");
	if (file)
	{
		fwrite(m_data, sizeof(char), m_size, file);
		fclose(file);
	}
}

void io::AlmFile::SaveAsync()
{
	std::async(std::launch::async, [this]() { Save(); });
}

std::wstring io::AlmFile::StrToWStr(const std::string & source)
{
	std::wstring ret;
	std::mbstate_t state = std::mbstate_t();
	const char *originals = source.data();
	ret.resize(std::mbsrtowcs(NULL, &originals, 0, &state));
	wchar_t * dest = const_cast<wchar_t*>(ret.data());
	std::mbsrtowcs(dest, &originals, ret.size(), &state);
	return std::move(ret);
}