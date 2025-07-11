#include <httplib.h>
#include <string>
#include <fstream>

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

int main() {
	httplib::Server svr;
	std::string html;

	try {
		html = readFileToString("static/index.htm");
	}
	catch (const std::exception& e) {
		std::cerr << "Error: " << e.what() << std::endl;
		return 1;
	}

	svr.Get("/api/data", [html](const httplib::Request&, httplib::Response& res) {
		res.set_content(html, "text/html");
	});
	svr.listen("0.0.0.0", 8080);
}