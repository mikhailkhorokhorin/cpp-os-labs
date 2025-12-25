#include "chat_server.hpp"

#include <pthread.h>

#include <csignal>
#include <cstring>
#include <iostream>

ChatServer* server_instance = nullptr;

ChatServer::ChatServer()
    : sharedmemory_manager(),
      user_manager(sharedmemory_manager.get()),
      message_logger(sharedmemory_manager.get()),
      message_dispatcher(sharedmemory_manager.get(), &user_manager),
      running(true) {
    server_instance = this;
    std::signal(SIGINT, signal_handler);
}

ChatServer::~ChatServer() = default;

void ChatServer::signal_handler(int) {
    if (server_instance)
        server_instance->running = false;
}

void ChatServer::handle_message(const Message& msg) {
    if (strcmp(msg.text, "LOGIN") == 0)
        user_manager.login_user(msg.from);
    else if (strcmp(msg.text, "LOGOUT") == 0)
        user_manager.logout_user(msg.from);
    else {
        bool delivered = message_dispatcher.deliver_message(msg);

        if (delivered) {
            message_logger.log_message(msg);
            message_logger.save_history(msg);
        } else {
            std::cout << "[error] Message from " << msg.from << " to " << msg.to << " was not sent"
                      << std::endl;
        }
    }
}

void ChatServer::run() {
    std::cout << "[system] Server startesd" << std::endl;

    SharedMemory* shm = sharedmemory_manager.get();
    while (running) {
        pthread_mutex_lock(&shm->server_queue.mutex);

        while (shm->server_queue.count == 0 && running)
            pthread_cond_wait(&shm->server_queue.cond, &shm->server_queue.mutex);

        if (!running) {
            pthread_mutex_unlock(&shm->server_queue.mutex);
            break;
        }

        Message msg = shm->server_queue.messages[0];
        for (int i = 1; i < shm->server_queue.count; ++i)
            shm->server_queue.messages[i - 1] = shm->server_queue.messages[i];
        shm->server_queue.count--;

        pthread_mutex_unlock(&shm->server_queue.mutex);

        handle_message(msg);
    }

    std::cout << "[system] Server stopped" << std::endl;
}
