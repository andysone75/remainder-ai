#include "Utils.h"
#include <string>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <codecvt>

std::string utils::readFileToString(const std::string& filepath) {
	std::ifstream file(filepath);
	if (!file.is_open()) {
		throw std::runtime_error("Could not open file: " + filepath);
	}

	file.seekg(0, std::ios::end);
	std::streampos fileSize = file.tellg();
	file.seekg(0, std::ios::beg);

	std::string content;
	content.reserve(fileSize);

	content.assign(
		std::istreambuf_iterator<char>(file),
		std::istreambuf_iterator<char>());

	return content;
}

std::wstring utils::toWstring(const std::string& str) {
	std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
	return converter.from_bytes(str);
}
