#pragma once

#include <cstring>
#include <iostream>

#include "shared_memory.hpp"

class UserManager {
   public:
    explicit UserManager(SharedMemory* sm);

    int find_user(const char* name) const;

    bool login_user(const char* name);
    bool logout_user(const char* name);

   private:
    SharedMemory* sharedmemory;
};
