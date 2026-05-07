#pragma once
#include <Windows.h>
#include <iostream>
#include <string>
#include <string_view>
#include <fstream>
#include <format>
#include <chrono>


#define LLog Logger::Get().Log


class Logger {
public:
	Logger()
	{
		s_instance = this;
		m_outLogFile.open("log.txt");
	}

	~Logger()
	{
		if (m_outLogFile.is_open()) {
			m_outLogFile.close();
		}
	}

	static inline Logger& Get() { return *s_instance; }

	template<typename... Args>
	void Log(std::string_view fmt, Args&&... args)
	{
		std::string logText = std::vformat(fmt, std::make_format_args(args...));
		if (!logText.ends_with("\n")) logText += '\n';
		std::cout << logText;
		OutputDebugString(logText.c_str());
		auto ms = std::chrono::floor<std::chrono::milliseconds>(std::chrono::system_clock::now());
		auto local = std::chrono::zoned_time{ std::chrono::current_zone(), ms };
		m_outLogFile << std::format("{:%F %T %Z}", local) << " | " << logText;
	}

private:
	static inline Logger* s_instance = nullptr;
	std::ofstream m_outLogFile;

};
