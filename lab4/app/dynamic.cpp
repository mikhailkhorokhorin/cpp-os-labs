#include <dlfcn.h>

#include <iostream>

#include "../include/contract.h"

void* lib = nullptr;
PrimeCountFunc PrimeCountPtr = nullptr;
PiFunc PiPtr = nullptr;

bool loadLib(const char* path) {
    if (lib)
        dlclose(lib);

    lib = dlopen(path, RTLD_LAZY);
    if (!lib) {
        std::cerr << "dlopen error: " << dlerror() << "\n";
        return false;
    }

    PrimeCountPtr = (PrimeCountFunc)dlsym(lib, "PrimeCount");
    PiPtr = (PiFunc)dlsym(lib, "Pi");

    if (!PrimeCountPtr || !PiPtr) {
        std::cerr << "dlsym error\n";
        return false;
    }

    std::cout << "Loaded: " << path << "\n";
    return true;
}

int main() {
    std::cout << "Dynamic loading program\n";

    loadLib("./libbasic.so");

    while (true) {
        std::cout << "> ";
        int cmd;
        std::cin >> cmd;

        if (cmd == 0) {
            static bool toggle = false;
            toggle = !toggle;
            loadLib(toggle ? "./libadvanced.so" : "./libbasic.so");
        } else if (cmd == 1) {
            int A, B;
            std::cin >> A >> B;
            std::cout << "PrimeCount = " << PrimeCountPtr(A, B) << "\n";
        } else if (cmd == 2) {
            int K;
            std::cin >> K;
            std::cout << "Pi = " << PiPtr(K) << "\n";
        }
    }
}
