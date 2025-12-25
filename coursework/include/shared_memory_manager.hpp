#pragma once

#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>

#include <cstdlib>
#include <iostream>

#include "shared_memory.hpp"

constexpr const char* SHM_NAME = "/chat_shm";

class SharedMemoryManager {
   public:
    SharedMemoryManager();
    ~SharedMemoryManager();

    SharedMemory* get();

    SharedMemoryManager(const SharedMemoryManager&) = delete;
    SharedMemoryManager& operator=(const SharedMemoryManager&) = delete;

   private:
    SharedMemory* sharedmemory;
    int fd;

    void init_shared_memory();
};
