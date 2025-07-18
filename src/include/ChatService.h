#pragma once
#include <LlmClient.h>

class ChatService {
public:
	ChatService(LlmClient& llm, std::string systemPromptPath);
	json answer(json chatHistory);

private:
	LlmClient& llm;
	std::string systemPromptPath;
};