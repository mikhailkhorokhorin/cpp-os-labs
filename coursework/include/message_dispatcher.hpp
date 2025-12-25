#pragma once

#include <pthread.h>

#include "shared_memory.hpp"
#include "user_manager.hpp"

class MessageDispatcher {
   public:
    MessageDispatcher(SharedMemory* sm, UserManager* um);

    bool deliver_message(const Message& message);

   private:
    SharedMemory* sharedmemory;
    UserManager* user_manager;
    int find_user(const char* name);
};
