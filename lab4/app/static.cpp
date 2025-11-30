#include <iostream>

#include "../include/contract.h"

int main() {
    std::cout << "Static linking program\n";

    while (true) {
        std::cout << "> ";
        int cmd;
        std::cin >> cmd;

        if (cmd == 1) {
            int A, B;
            std::cin >> A >> B;
            std::cout << "PrimeCount = " << PrimeCount(A, B) << "\n";
        } else if (cmd == 2) {
            int K;
            std::cin >> K;
            std::cout << "Pi = " << Pi(K) << "\n";
        } else {
            std::cout << "Unknown command\n";
        }
    }
}
