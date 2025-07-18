#pragma once

#include <string>
#include <nlohmann/json.hpp>
#include <httplib.h>

using json = nlohmann::json;

class LlmClient {
public:
	LlmClient(const std::string& url);
	json sendMessage(const json& chatHistory);

private:
	httplib::Client cli;
};