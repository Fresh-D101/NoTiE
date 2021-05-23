#include "Log.h"

#include <spdlog/sinks/stdout_color_sinks.h>
#include "../Debug/notie_console_sink.h"
#include "../Debug/Console.h"

namespace notie
{
	std::shared_ptr<spdlog::logger> Log::CoreLogger;
	std::shared_ptr<spdlog::logger> Log::ClientLogger;
	std::unique_ptr<Console> Log::LogConsole;
	
	void Log::Init()
	{
		LogConsole = std::make_unique<Console>();
		
		auto sink = std::make_shared<notie_console_sink_mt>(*LogConsole.get());
		sink->set_pattern("%^[%T] %n %l: %v%$");// Pattern for: ColorRangeStart[TimeStamp] ConsoleName LogLevel: LogMessage ColorRangeStop
		
		CoreLogger = std::make_shared<spdlog::logger>("NOTIE", sink);
		spdlog::register_logger(CoreLogger);
		CoreLogger->set_level(spdlog::level::trace);

		ClientLogger = std::make_shared<spdlog::logger>("EDITOR", sink);
		spdlog::register_logger(ClientLogger);
		ClientLogger->set_level(spdlog::level::trace);
	}

	void Log::DrawConsole(bool* p_open)
	{
		LogConsole->Draw("Console", p_open);
	}
}
