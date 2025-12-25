#include "message_logger.hpp"

#include <cstring>

MessageLogger::MessageLogger(SharedMemory* sm) : sharedmemory(sm) {
}

void MessageLogger::log_message(const Message& message) const {
    std::cout << "[message] " << message.from << " -> " << message.to << ": " << message.text
              << std::endl;
}

void MessageLogger::save_history(const Message& message) {
    if (sharedmemory->history_count >= MAX_HISTORY)
        return;

    auto& entry = sharedmemory->history[sharedmemory->history_count++];
    strcpy(entry.from, message.from);
    strcpy(entry.to, message.to);
    strcpy(entry.text, message.text);
}

std::vector<Message> MessageLogger::search_history(const std::string& keyword) const {
    std::vector<Message> results;

    for (int i = 0; i < sharedmemory->history_count; ++i) {
        const HistoryEntry& entry = sharedmemory->history[i];
        if (std::string(entry.from).find(keyword) != std::string::npos ||
            std::string(entry.to).find(keyword) != std::string::npos ||
            std::string(entry.text).find(keyword) != std::string::npos) {
        
            Message message{};
            strncpy(message.from, entry.from, MAX_NAME);
            strncpy(message.to, entry.to, MAX_NAME);
            strncpy(message.text, entry.text, MAX_TEXT);

            results.push_back(message);
        }
    }

    return results;
}