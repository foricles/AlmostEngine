#include "alm_file.hpp"
#include "alm_flsystem.hpp"
#include <fstream>

using namespace alme;
#define SAFE_DELETE_DATA(x) if(x){delete [] x; x = nullptr; }

std::vector<std::wstring> io::AlmFile::s_filepath;

io::AlmFile::AlmFile()
	: m_data(nullptr)
	, m_size(0)
	, m_pathIndex(-1)
{
}

io::AlmFile::AlmFile(const std::wstring & filePath)
	: AlmFile()
{
	m_pathIndex = s_filepath.size();
	s_filepath.push_back(filePath);
}

io::AlmFile::AlmFile(const std::string & filePath)
	: AlmFile(io::AlmFileSystem::StrToWStr(filePath))
{
}

io::AlmFile::AlmFile(AlmFile && rhv) noexcept
	: m_data(rhv.m_data)
	, m_size(0)
	, m_pathIndex(-1)
{
	rhv.m_data = nullptr;
	std::swap(m_size, rhv.m_size);
	std::swap(m_pathIndex, rhv.m_pathIndex);
}

io::AlmFile & alme::io::AlmFile::operator=(AlmFile && rhv) noexcept
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
	std::ifstream infile(GetFullPath());
	return infile.good();
}

io::AlmFile & io::AlmFile::Load(bool bin)
{
	uint32_t o = !bin;
	SAFE_DELETE_DATA(m_data);
	auto mode = (bin) ? (std::ios::binary) : (std::ios::in);
	std::ifstream file(GetFullPath(), mode | std::ios::ate);
	std::streamsize size = file.tellg();
	m_size = static_cast<uint32_t>(size) + o;
	file.seekg(0, std::ios::beg);
	m_data = new uint8_t[m_size]{ 0 };
	file.read((char*)m_data, m_size - o);
	return *this;
}

io::AlmFile & io::AlmFile::Load(const std::wstring &filepath, bool bin)
{
	if (m_pathIndex < 0)
	{
		m_pathIndex = s_filepath.size();
		s_filepath.push_back(filepath);
		return Load(bin);
	}
	s_filepath[m_pathIndex] = filepath;
	return Load(bin);
}

io::AlmFile & io::AlmFile::Load(const std::string &filepath, bool bin)
{
	return Load(io::AlmFileSystem::StrToWStr(filepath), bin);
}

void io::AlmFile::Write(const uint8_t* data, uint32_t size, bool append)
{
	if (!data) return;
	if (!append || !m_data)
	{
		SAFE_DELETE_DATA(m_data);
		m_data = new uint8_t[size];
		memcpy_s(m_data, size, data, size);
		m_size = size;
	}
	else
	{
		uint8_t* buffer = new uint8_t[m_size + size];
		memcpy(buffer, m_data, m_size);
		memcpy(buffer + m_size, data, size);
		SAFE_DELETE_DATA(m_data);
		m_data = buffer;
		m_size += size;
	}
}

void io::AlmFile::Save(bool bin)
{
	auto mode = std::ios::out | (bin ? std::ios::binary : 0);
	std::ofstream myfile(GetFullPath(), mode);
	myfile.write((char*)m_data, m_size);
	myfile.close();
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

void alme::io::AlmFile::c_read(uint8_t*& rhv)
{
	rhv = m_data;
	m_data = nullptr;
}