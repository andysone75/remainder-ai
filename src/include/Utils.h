#pragma once
#include <string>

namespace utils {
	std::string readFileToString(const std::string& filepath);
	std::wstring toWstring(const std::string& str);
}