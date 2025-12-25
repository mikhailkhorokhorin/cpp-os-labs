#pragma once

#include <atomic>

#include "message_dispatcher.hpp"
#include "message_logger.hpp"
#include "shared_memory_manager.hpp"
#include "user_manager.hpp"

class ChatServer {
   public:
    ChatServer();
    ~ChatServer();

    void run();

   private:
    SharedMemoryManager sharedmemory_manager;
    UserManager user_manager;
    MessageLogger message_logger;
    MessageDispatcher message_dispatcher;
    std::atomic<bool> running;

    static void signal_handler(int);
    void handle_message(const Message& message);
};
