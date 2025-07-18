#include "LlmClient.h"

LlmClient::LlmClient(const std::string& url)
	: cli(url) {}

json LlmClient::sendMessage(const json& chatHistory) {
	json request = {
		{"model", "local-model"},
		{"messages", chatHistory},
		{"temperature", 0.15}
	};

	auto ans = cli.Post("/v1/chat/completions", request.dump(), "application/json");

	json response;
	if (!ans || ans->status != 200) {
		response["type"] = "error";
		response["content"] = "";
	}
	else {
		std::string content = json::parse(ans->body)["choices"][0]["message"]["content"];
		response["type"] = "message";
		response["content"] = content;
	}

	return response;
}
