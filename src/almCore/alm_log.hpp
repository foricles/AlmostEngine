#ifndef _ALMOSTLOG_HPP_
#define _ALMOSTLOG_HPP_

#include "alm_platform.hpp"

class AlmostLog
{
public:
	enum class Type
	{
		Info,
		Warning,
		Error,
		Text
	};

private:
	AlmostLog();
	static AlmostLog *m_LogInstance;
public:
	static AlmostLog &get();

	~AlmostLog();

	template <typename ...Args>
	void			Log(Type type, Args ... args);

	void			print(const std::string &mess, Type type);

	void			saveSettings(const std::string &path);
	void			loadSettings(const std::string &path);

	void			pushIntoFile();

private:

	bool			checkForWrite(Type type);
	void			writeInConsole(const std::string &mess, Type type);
	void			writeInGameConsole(const std::string &mess, Type type);
	std::string		getTimeAsString();

	void			pushToHistory(const std::string &prefix, const std::string &mess, Type type);

private:
	template <typename T>
	void			LogProcess(std::ostringstream &out, T t);

	template <typename T, typename ...Args>
	void			LogProcess(std::ostringstream &out, T t, Args... args);

public:
	struct
	{
		bool enableOutput : 1;

		bool writeFile : 1;
		bool writeConsole : 1;
		bool writeGameConsole : 1;

		bool printInfo : 1;
		bool printText : 1;
		bool printErrors : 1;
		bool printWarnings : 1;
	} Settings;

private:
	std::string m_appName;
	std::ostringstream m_outHtml;
};

template<typename T>
inline void AlmostLog::LogProcess(std::ostringstream & out, T t)
{
	out << t << " ";
}

template<typename T, typename ...Args>
inline void AlmostLog::LogProcess(std::ostringstream & out, T t, Args ...args)
{
	LogProcess(out, t);
	LogProcess(out, args...);
}

template<typename ...Args>
inline void AlmostLog::Log(Type type, Args ...args)
{
	std::ostringstream message;
	LogProcess(message, args...);
	print(message.str(), type);
}

#define ALM_LOG_TEXT(...) AlmostLog::get().Log(AlmostLog::Type::Text, __VA_ARGS__);
#define ALM_LOG_INFO(...) AlmostLog::get().Log(AlmostLog::Type::Info, __VA_ARGS__);
#define ALM_LOG_ERROR(...) AlmostLog::get().Log(AlmostLog::Type::Error, __VA_ARGS__); AlmostLog::get().pushIntoFile();
#define ALM_LOG_WARNING(...) AlmostLog::get().Log(AlmostLog::Type::Warning, __VA_ARGS__); AlmostLog::get().pushIntoFile();

#define ALM_LOG_ASSERT(cond, ...) if(!(cond)){AlmostLog::get().Log(AlmostLog::Type::Error, __VA_ARGS__); AlmostLog::get().pushIntoFile(); assert(false);}

#endif //!_ALMOSTLOG_HPP_

