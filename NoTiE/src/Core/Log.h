#pragma once
#include <memory>

#include <spdlog/spdlog.h>

class Console;

namespace notie
{
	class Log
	{
	public:
		static void Init();
		static void DrawConsole(bool* p_open);

		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return CoreLogger; }
		inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return ClientLogger; }

	private:
		static std::shared_ptr<spdlog::logger> CoreLogger;
		static std::shared_ptr<spdlog::logger> ClientLogger;
		static std::unique_ptr<Console> LogConsole;
	};
}

//Core log macros
#define NTE_CORE_TRACE(...)		::notie::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define NTE_CORE_INFO(...)		::notie::Log::GetCoreLogger()->info(__VA_ARGS__)
#define NTE_CORE_WARN(...)		::notie::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define NTE_CORE_ERROR(...)		::notie::Log::GetCoreLogger()->error(__VA_ARGS__)
#define NTE_CORE_CRITICAL(...)	::notie::Log::GetCoreLogger()->critical(__VA_ARGS__)
#define NTE_CORE_DEBUG(...)		::notie::Log::GetCoreLogger()->debug(__VA_ARGS__)

//Client log macros
#define NTE_TRACE(...)		::notie::Log::GetClientLogger()->trace(__VA_ARGS__)
#define NTE_INFO(...)		::notie::Log::GetClientLogger()->info(__VA_ARGS__)
#define NTE_WARN(...)		::notie::Log::GetClientLogger()->warn(__VA_ARGS__)
#define NTE_ERROR(...)		::notie::Log::GetClientLogger()->error(__VA_ARGS__)
#define NTE_CRITICAL(...)	::notie::Log::GetClientLogger()->critical(__VA_ARGS__)
#define NTE_DEBUG(...)		::notie::Log::GetClientLogger()->debug(__VA_ARGS__)