#pragma once
#include <string>

namespace Utils
{
	inline std::string GetWorkingDirectory()
	{
		return std::filesystem::current_path().string() + "/";
	}
}
