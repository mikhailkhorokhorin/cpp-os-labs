#pragma once

#include <iostream>
#include <string>
#include <vector>

#include "shared_memory.hpp"

class MessageLogger {
   public:
    explicit MessageLogger(SharedMemory* sm);

    void log_message(const Message& message) const;
    void save_history(const Message& message);

    std::vector<Message> search_history(const std::string& keyword) const;

   private:
    SharedMemory* sharedmemory;
};
