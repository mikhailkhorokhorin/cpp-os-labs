#include "shared_memory_manager.hpp"

SharedMemoryManager::SharedMemoryManager() : sharedmemory(nullptr), fd(-1) {
    init_shared_memory();
}

void SharedMemoryManager::init_shared_memory() {
    int fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666);
    if (fd < 0) {
        perror("shm_open");
        std::exit(1);
    }

    if (ftruncate(fd, sizeof(SharedMemory)) != 0) {
        perror("ftruncate");
        std::exit(1);
    }

    sharedmemory = (SharedMemory*)mmap(nullptr, sizeof(SharedMemory), PROT_READ | PROT_WRITE,
                                       MAP_SHARED, fd, 0);
    if (sharedmemory == MAP_FAILED) {
        perror("mmap");
        std::exit(1);
    }

    pthread_mutexattr_t mattr;
    pthread_condattr_t cattr;

    pthread_mutexattr_init(&mattr);
    pthread_mutexattr_setpshared(&mattr, PTHREAD_PROCESS_SHARED);

    pthread_condattr_init(&cattr);
    pthread_condattr_setpshared(&cattr, PTHREAD_PROCESS_SHARED);

    pthread_mutex_init(&sharedmemory->users_mutex, &mattr);

    sharedmemory->history_count = 0;

    sharedmemory->server_queue.count = 0;
    pthread_mutex_init(&sharedmemory->server_queue.mutex, &mattr);
    pthread_cond_init(&sharedmemory->server_queue.cond, &cattr);

    for (int i = 0; i < MAX_USERS; ++i) {
        sharedmemory->usernames[i][0] = '\0';
        sharedmemory->client_queues[i].count = 0;
        pthread_mutex_init(&sharedmemory->client_queues[i].mutex, &mattr);
        pthread_cond_init(&sharedmemory->client_queues[i].cond, &cattr);
    }

    std::cout << "[system] Shared memory initialized" << std::endl;
}

SharedMemoryManager::~SharedMemoryManager() {
    if (sharedmemory) {
        munmap(sharedmemory, sizeof(SharedMemory));
        shm_unlink(SHM_NAME);
    }
}

SharedMemory* SharedMemoryManager::get() {
    return sharedmemory;
}