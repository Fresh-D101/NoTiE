#pragma once
#include <spdlog/fmt/bundled/format.h>
#include <spdlog/sinks/base_sink.h>

#include "Console.h"

template<typename Mutex>
class notie_console_sink : public spdlog::sinks::base_sink<Mutex>
{
public:
	notie_console_sink(Console& console) : Console(console)
	{
		SetColors();
	};

protected:
	void sink_it_(const spdlog::details::log_msg& msg) override
	{
		spdlog::memory_buf_t formatted;
		spdlog::sinks::base_sink<Mutex>::formatter_->format(msg, formatted);
		Console.AddLog(fmt::to_string(formatted).c_str(), colors[msg.level]);
	}

	void flush_() override{}

private:
	void SetColors()
	{
		colors[spdlog::level::trace] = Color::White;
		//colors[spdlog::level::debug] = Color::Cyan;
		colors[spdlog::level::debug] = Color::Cornflower;
		colors[spdlog::level::info] = Color::Green;
		colors[spdlog::level::warn] = Color::Yellow;
		colors[spdlog::level::err] = Color::Red;
		colors[spdlog::level::critical] = Color::Red;
		//TODO
		colors[spdlog::level::off] = Color::White;
	}
	
	Console& Console;
	notie::Color colors[spdlog::level::n_levels];
};

#include "spdlog/details/null_mutex.h"
#include <mutex>
using notie_console_sink_mt = notie_console_sink<std::mutex>;
using notie_console_sink_st = notie_console_sink<spdlog::details::null_mutex>;