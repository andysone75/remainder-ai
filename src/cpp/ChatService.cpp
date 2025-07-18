#include "ChatService.h"
#include <Utils.h>

ChatService::ChatService(LlmClient& _llm, std::string _systemPromptPath)
	: llm(_llm), systemPromptPath(_systemPromptPath) {}

json ChatService::answer(json chatHistory) {
	// Remove old messages for previous requests
	auto cur = chatHistory.begin();
	auto lastCreate = chatHistory.end();

	while (cur != chatHistory.end()) {
		if ((*cur)["role"] == "assistant" && (*cur)["type"] == "create") {
			lastCreate = cur;
		}
		cur++;
	}

	if (lastCreate != chatHistory.end()) {
		lastCreate++;
		chatHistory.erase(chatHistory.begin(), lastCreate);
	}

	// Insert system prompt
	std::string systemPrompt = utils::readFileToString(systemPromptPath);
	//systemPrompt += utils::getDateTime();

	json systemMsg = {
		{"role", "system"},
		{"content", systemPrompt}
	};

	chatHistory.insert(chatHistory.begin(), systemMsg);

	return llm.sendMessage(chatHistory);
}
