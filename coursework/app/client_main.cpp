#include "chat_client.hpp"

int main() {
    std::string name;
    std::cout << "Enter a login: ";
    std::getline(std::cin, name);

    ChatClient client(name);
    client.run();
    

    return 0;
}
