#include "user_manager.hpp"

UserManager::UserManager(SharedMemory* sm) : sharedmemory(sm) {
}

int UserManager::find_user(const char* name) const {
    for (int i = 0; i < MAX_USERS; ++i)
        if (strcmp(sharedmemory->usernames[i], name) == 0)
            return i;
    return -1;
}

bool UserManager::login_user(const char* name) {
    pthread_mutex_lock(&sharedmemory->users_mutex);

    for (int i = 0; i < MAX_USERS; ++i) {
        if (sharedmemory->usernames[i][0] == '\0') {
            strcpy(sharedmemory->usernames[i], name);
            std::cout << "[user] " << name << " has logged in" << std::endl;
            pthread_mutex_unlock(&sharedmemory->users_mutex);
            return true;
        }
    }

    pthread_mutex_unlock(&sharedmemory->users_mutex);
    std::cout << "[error] No free slots available for user " << name << std::endl;
    return false;
}

bool UserManager::logout_user(const char* name) {
    pthread_mutex_lock(&sharedmemory->users_mutex);

    int index = find_user(name);
    if (index >= 0) {
        sharedmemory->usernames[index][0] = '\0';
        std::cout << "[user] " << name << " has logged out" << std::endl;
        pthread_mutex_unlock(&sharedmemory->users_mutex);
        return true;
    }

    pthread_mutex_unlock(&sharedmemory->users_mutex);
    std::cout << "[error] User " << name << " not found" << std::endl;
    return false;
}
