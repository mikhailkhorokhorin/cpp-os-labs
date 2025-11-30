#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <string>

const size_t SIZE = 4096;

int main() {
    std::string filename;
    std::cout << "Enter file name: ";
    std::getline(std::cin, filename);

    int fd = shm_open("/mymmap", O_CREAT | O_RDWR, 0666);
    if (fd == -1) {
        std::cerr << "Failed to create shared memory" << std::endl;
        return 1;
    }

    if (ftruncate(fd, SIZE) == -1) {
        std::cerr << "Failed to set size of shared memory" << std::endl;
        return 1;
    }

    void* ptr = mmap(nullptr, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (ptr == MAP_FAILED) {
        std::cerr << "Failed to mmap" << std::endl;
        return 1;
    }

    pid_t pid = fork();
    if (pid < 0) {
        std::cerr << "Failed to fork" << std::endl;
        return 1;
    }

    if (pid == 0) {
        FILE* f = fopen(filename.c_str(), "r");
        if (!f) {
            std::cerr << "Failed to open file" << std::endl;
            exit(1);
        }

        char* mmap_ptr = static_cast<char*>(ptr);
        size_t total = fread(mmap_ptr, 1, SIZE - 1, f);
        mmap_ptr[total] = '\0';
        fclose(f);

        execl("./child", "child", nullptr);
        std::cerr << "Failed to execute child" << std::endl;
        exit(1);
    } else {
        waitpid(pid, nullptr, 0);

        char* mmap_ptr = static_cast<char*>(ptr);
        std::cout << mmap_ptr << std::endl;

        munmap(ptr, SIZE);
        shm_unlink("/mymmap");
    }

    return 0;
}
