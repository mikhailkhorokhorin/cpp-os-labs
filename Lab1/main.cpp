#include <sys/wait.h>
#include <unistd.h>

#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <string>

int main() {
    std::string filename;
    std::cout << "Enter file name: ";
    std::getline(std::cin, filename);

    int pipe1[2];
    if (pipe(pipe1) == -1) {
        std::cerr << "Failed to create pipe" << std::endl;
        return 1;
    }

    pid_t pid = fork();
    if (pid < 0) {
        std::cerr << "Failed to create child process" << std::endl;
        return 1;
    }

    if (pid == 0) {
        close(pipe1[0]);

        FILE* f = fopen(filename.c_str(), "r");

        if (!f) {
            std::cerr << "Failed to open file" << std::endl;
            exit(1);
        }

        dup2(fileno(f), STDIN_FILENO);
        fclose(f);

        dup2(pipe1[1], STDOUT_FILENO);
        close(pipe1[1]);

        execl("./build/child", "child", nullptr);
        std::cerr << "Failed to execute './child'" << std::endl;
        exit(1);

    } else {
        close(pipe1[1]);

        char buffer[256];
        ssize_t n;
        while ((n = read(pipe1[0], buffer, sizeof(buffer) - 1)) > 0) {
            buffer[n] = '\0';
            std::cout << buffer;
        }

        close(pipe1[0]);

        waitpid(pid, nullptr, 0);
    }

    return 0;
}
