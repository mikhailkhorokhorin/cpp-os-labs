#include "message_dispatcher.hpp"

#include <cstring>
#include <iostream>

MessageDispatcher::MessageDispatcher(SharedMemory* sm, UserManager* um)
    : sharedmemory(sm), user_manager(um) {
}

int MessageDispatcher::find_user(const char* name) {
    return user_manager->find_user(name);
}

bool MessageDispatcher::deliver_message(const Message& msg) {
    int to_idx = find_user(msg.to);
    if (to_idx < 0) {
        return false;
    }

    auto& queue = sharedmemory->client_queues[to_idx];
    pthread_mutex_lock(&queue.mutex);
    if (queue.count < MAX_QUEUE) {
        queue.messages[queue.count++] = msg;
        pthread_cond_signal(&queue.cond);
    }
    pthread_mutex_unlock(&queue.mutex);
    return true;
}
