#include <iostream>
#include <sstream>
#include <string>

int main() {
    std::string line;

    while (std::getline(std::cin, line)) {
        std::istringstream iss(line);

        float num = 0.0f;
        double sum = 0.0;

        while (iss >> num)
            sum += num;

        std::cout << sum << std::endl;
    }

    return 0;
}
