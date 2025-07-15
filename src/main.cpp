#include <httplib.h>
#include <nlohmann/json.hpp>
#include <string>
#include <fstream>
#include <codecvt>

using json = nlohmann::json;

std::string readFileToString(const std::string& filepath) {
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

std::string getDateTime() {
	std::time_t now = std::time(nullptr);
	std::tm* localTime = std::localtime(&now);
	
	std::ostringstream oss;
	oss << std::put_time(localTime, "%Y-%m-%d %H:%M");
	
	return oss.str();
}

std::wstring toWstring(std::string& str) {
	std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
	return converter.from_bytes(str);
}

int main() {
	std::cout << "Server starting... ";

	httplib::Server svr;
	httplib::Client cli("http://localhost:1234");

	std::string html;
	try {
		html = readFileToString("static/index.htm");
	}
	catch (const std::exception& e) {
		std::cerr << "Error: " << e.what() << std::endl;
		return 1;
	}

	svr.Get("/", [](const httplib::Request&, httplib::Response& res) {
		std::string html = readFileToString("static/index.htm");
		res.set_content(html, "text/html");
	});

	svr.Post("/api/msg", [&cli](const httplib::Request& req, httplib::Response& res) {
		auto history = json::parse(req.body)["history"];
		std::string systemPrompt = readFileToString("systemPrompt.txt");
		systemPrompt += getDateTime();

		json systemMsg = {
			{"role", "system"},
			{"content", systemPrompt}
		};

		history.insert(history.begin(), systemMsg);

		json request = {
			{"model", "local-model"},
			{"messages", history},
			{"temperature", 0.7}
		};

		auto gpt = cli.Post("/v1/chat/completions", request.dump(), "application/json");
		json response;

		if (!gpt || gpt->status != 200) {
			response["type"] = "error";
			response["content"] = "";
		}
		else {
			std::string ans = json::parse(gpt->body)["choices"][0]["message"]["content"];
			response["type"] = "message";
			response["content"] = ans;
		}

		res.set_content(response.dump(), "application/json");
	});

	std::cout << "[ OK ]" << std::endl;
	svr.listen("0.0.0.0", 8080);
	return 0;
}