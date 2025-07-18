#include <httplib.h>
#include <nlohmann/json.hpp>
#include <string>
#include "Utils.h"
#include <LlmClient.h>
#include <ChatService.h>

using json = nlohmann::json;

//LlmClient llm("http://llama-cpp-server:1234");
LlmClient llm("http://localhost:1234");
ChatService chat(llm, "systemPrompt.txt");

void mainPageCallback(const httplib::Request&, httplib::Response& res) {
	std::string html = utils::readFileToString("static/index.htm");
	res.set_content(html, "text/html");
}

void assistantMsgCallback(const httplib::Request& req, httplib::Response& res) {
	auto history = json::parse(req.body)["history"];
	auto ans = chat.answer(history);
	res.set_content(ans.dump(), "application/json");
}

int main() {
	std::cout << "Server starting... ";

	httplib::Server svr;
	svr.Get("/", mainPageCallback);
	svr.Post("/api/msg", assistantMsgCallback);

	std::cout << "[ OK ]" << std::endl;
	svr.listen("0.0.0.0", 8080);
	
	return 0;
}