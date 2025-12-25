#pragma once

#include <atomic>
#include <cstring>
#include <fcntl.h>
#include <iostream>
#include <pthread.h>
#include <string>
#include <sys/mman.h>
#include <thread>
#include <unistd.h>

#include "shared_memory.hpp"

constexpr const char* SHM_NAME = "/chat_shm";

class ChatClient {
public:
    explicit ChatClient(const std::string& name);
    ~ChatClient();

    void run();
    void stop();

private:
    std::string username;
    std::atomic<bool> running;
    SharedMemory* sharedmemory;
    int my_index;

    std::thread receiver_thread;

    void connect_shared_memory();
    void login();
    void logout();
    void receiver_loop();
    void send_to_server(const Message& message);
    void send_message(const std::string& input);
};
