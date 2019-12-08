#include "alm_file.hpp"
#include "alm_flsystem.hpp"
#include <cstdio>

using namespace alme;
#define SAFE_DELETE_DATA(x) if(x){delete [] x; x = nullptr; }

std::vector<std::wstring> io::AlmFile::s_filepath;

io::AlmFile::AlmFile()
	: m_data(nullptr)
	, m_pathIndex(-1)
{
}

io::AlmFile::AlmFile(const std::wstring & filePath)
	: m_data(nullptr)
	, m_pathIndex(-1)
{
	m_pathIndex = s_filepath.size();
	s_filepath.push_back(filePath);
}

io::AlmFile::AlmFile(const std::string & filePath)
	: AlmFile(StrToWStr(filePath))
{
}

io::AlmFile::AlmFile(AlmFile && rhv)
	: m_data(rhv.m_data)
{
	rhv.m_data = nullptr;
	std::swap(m_pathIndex, rhv.m_pathIndex);
}

io::AlmFile & alme::io::AlmFile::operator=(AlmFile && rhv)
{
	m_data = rhv.m_data;
	std::swap(m_pathIndex, rhv.m_pathIndex);
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
	return io::AlmFileSystem::getRootPath() + s_filepath[m_pathIndex];
}

const std::wstring & io::AlmFile::GetPath() const
{
	return s_filepath[m_pathIndex];
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



io::AlmFile & io::AlmFile::Load()
{
	SAFE_DELETE_DATA(m_data);
	FILE *file = NULL;
	_wfopen_s(&file, GetFullPath().c_str(), L"rb");
	fseek(file, 0, SEEK_END);
	m_size = ftell(file);
	m_data = new uint8_t[m_size];
	fseek(file, 0, SEEK_SET);
	fread(m_data, sizeof(uint8_t), m_size, file);
	fclose(file);

	return *this;
}

io::AlmFile & io::AlmFile::Load(const std::wstring &filepath)
{
	if (m_pathIndex < 0)
	{
		m_pathIndex = s_filepath.size();
		s_filepath.push_back(filepath);
		return Load();
	}
	s_filepath[m_pathIndex] = filepath;
	return Load();
}

io::AlmFile & io::AlmFile::Load(const std::string &filepath)
{
	return Load(StrToWStr(filepath));
}

void io::AlmFile::Write(const uint8_t * data, uint32_t size)
{
	if (!data) return;
	SAFE_DELETE_DATA(m_data);
	m_data = new uint8_t[size+1];
	memcpy_s(m_data, size, data, size);
	m_size = size;
}

void io::AlmFile::Save()
{
	FILE *file = NULL;
	_wfopen_s(&file, GetFullPath().c_str(), L"wb");
	if (file)
	{
		fwrite(m_data, sizeof(char), m_size, file);
		fclose(file);
	}
}

std::vector<uint8_t> io::AlmFile::asBin() const
{
	std::vector<uint8_t> ret;
	ret.assign(m_data, m_data + m_size);
	return std::move(ret);
}

std::string io::AlmFile::asString() const
{
	return std::move(std::string((char*)m_data));
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