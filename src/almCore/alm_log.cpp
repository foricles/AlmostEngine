#include "alm_log.hpp"
#include <chrono>
#include <iostream>
#include <iomanip>
#include <map>
#include <cstdarg>
#include "almFileSystem/alm_file.hpp"

using namespace alme;

///===============================
///	Platform dependense section
///===============================

#ifdef ALM_OS_WINDOWS
	enum ConsoleColor
	{
		Black = 0,
		LightGreen = 10,
		LightRed = 12,
		Yellow = 14,
		White = 15
	};
#else

#endif // ALM_OS_WINDOWS

///===============================

namespace 
{
	const std::map<AlmostLog::Type, std::string> OutPrefix = {
		std::pair<AlmostLog::Type, std::string>(AlmostLog::Type::Text,		": "),
		std::pair<AlmostLog::Type, std::string>(AlmostLog::Type::Info,		"Info: "),
		std::pair<AlmostLog::Type, std::string>(AlmostLog::Type::Error,		"ERROR: "),
		std::pair<AlmostLog::Type, std::string>(AlmostLog::Type::Warning,	"Warning: "),
	};
	const std::map<AlmostLog::Type, std::string> HTMLColor = {
		std::pair<AlmostLog::Type, std::string>(AlmostLog::Type::Text,		"#000000"),
		std::pair<AlmostLog::Type, std::string>(AlmostLog::Type::Info,		"#0aa827"),
		std::pair<AlmostLog::Type, std::string>(AlmostLog::Type::Error,		"#c60017"),
		std::pair<AlmostLog::Type, std::string>(AlmostLog::Type::Warning,	"#c1a503"),
	};
}

AlmostLog *AlmostLog::m_LogInstance = nullptr;
AlmostLog &AlmostLog::get()
{
	if (m_LogInstance == nullptr)
		m_LogInstance = new AlmostLog();
	return *m_LogInstance;
}

AlmostLog::AlmostLog()
{
	//TODO: automatization
	Settings.printText = true;
	Settings.writeFile = true;
	Settings.printInfo = true;
	Settings.printErrors = true;
	Settings.enableOutput = true;
	Settings.writeConsole = true;
	Settings.printWarnings = true;
	Settings.writeGameConsole = true;

	m_outHtml << "<!DOCTYPE html>\n<html>\n<head>\n\t<title>" << m_appName << " log </title>\n</head>\n<body>\n";
}

AlmostLog::~AlmostLog()
{
	pushIntoFile();
}




void AlmostLog::print(const std::string & mess, Type type)
{
	if (Settings.writeConsole) writeInConsole(mess, type);
	if (Settings.writeGameConsole) writeInGameConsole(mess, type);

	if (Settings.writeFile) pushToHistory(getTimeAsString() + OutPrefix.at(type), mess, type);
}

inline bool AlmostLog::checkForWrite(Type type)
{
	if (!Settings.printInfo		&& type == Type::Info)	  return false;
	if (!Settings.printText		&& type == Type::Text)	  return false;
	if (!Settings.printErrors	&& type == Type::Error)	  return false;
	if (!Settings.printWarnings && type == Type::Warning) return false;

	return true;
}

inline std::string AlmostLog::getTimeAsString()
{
	using namespace std::chrono;
	std::ostringstream sstream;
	time_t now = system_clock::to_time_t(system_clock::now());
	auto tm = std::localtime(&now);
	sstream << std::put_time(tm, "[%d.%m.%Y|%H:%M:%S]");
	return sstream.str();
}

inline void AlmostLog::writeInConsole(const std::string &mess, Type type)
{
	if (checkForWrite(type))
	{
#ifdef ALM_OS_WINDOWS
		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

		switch (type)
		{
		case Type::Text:
			SetConsoleTextAttribute(hConsole, (WORD)((ConsoleColor::Black << 4) | ConsoleColor::White)); break;
		case Type::Info:
			SetConsoleTextAttribute(hConsole, (WORD)((ConsoleColor::Black << 4) | ConsoleColor::LightGreen)); break;
		case Type::Warning:
			SetConsoleTextAttribute(hConsole, (WORD)((ConsoleColor::Black << 4) | ConsoleColor::Yellow)); break;
		case Type::Error:
			SetConsoleTextAttribute(hConsole, (WORD)((ConsoleColor::Black << 4) | ConsoleColor::LightRed)); break;
		}
#else
#endif // ALM_OS_WINDOWS


		std::cout << getTimeAsString() << OutPrefix.at(type) << mess << std::endl;


#ifdef ALM_OS_WINDOWS
		SetConsoleTextAttribute(hConsole, (WORD)((ConsoleColor::Black << 4) | ConsoleColor::White));
#else
#endif // ALM_OS_WINDOWS

	}
}

inline void AlmostLog::writeInGameConsole(const std::string &mess, Type type)
{
	if (checkForWrite(type))
	{
		//TODO: provie game console
	}
}

void AlmostLog::pushToHistory(const std::string & prefix, const std::string & mess, Type type)
{
	if (checkForWrite(type))
	{
		m_outHtml << "\t<p><span style=\"color:" << HTMLColor.at(type) << "\">" << prefix << "</span>" << mess << "</p>\n";
	}
}

void AlmostLog::pushIntoFile()
{
	m_outHtml << "</body>\n</html>";
	if (Settings.writeFile)
	{
		std::string out = m_outHtml.str();
		io::AlmFile toSave(L"AlmostLog.html");
		toSave.Write((uint8_t*)out.c_str(), out.size());
		toSave.Save();
	}
}

void AlmostLog::saveSettings(const std::string & path)
{
}

void AlmostLog::loadSettings(const std::string & path)
{
}