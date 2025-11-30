#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>

#include <cstring>
#include <iostream>
#include <sstream>
#include <string>

const size_t SIZE = 4096;

int main() {
    int fd = shm_open("/mymmap", O_RDWR, 0666);
    if (fd == -1) {
        std::cerr << "Failed to open shared memory" << std::endl;
        return 1;
    }

    void* ptr = mmap(nullptr, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (ptr == MAP_FAILED) {
        std::cerr << "Failed to mmap" << std::endl;
        return 1;
    }

    char* data = static_cast<char*>(ptr);
    std::istringstream iss(data);
    std::string line;
    std::ostringstream result;

    while (std::getline(iss, line)) {
        std::istringstream line_stream(line);
        float num;
        double sum = 0.0;
        while (line_stream >> num)
            sum += num;
        result << sum << std::endl;
    }

    strncpy(data, result.str().c_str(), SIZE - 1);
    data[SIZE - 1] = '\0';

    munmap(ptr, SIZE);
    close(fd);

    return 0;
}
