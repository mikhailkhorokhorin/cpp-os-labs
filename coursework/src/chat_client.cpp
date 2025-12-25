#include "chat_client.hpp"

ChatClient::ChatClient(const std::string& name)
    : username(name), running(true), sharedmemory(nullptr), my_index(-1) {
    connect_shared_memory();
    login();
    receiver_thread = std::thread(&ChatClient::receiver_loop, this);
}

ChatClient::~ChatClient() {
    stop();
}

void ChatClient::connect_shared_memory() {
    int fd = shm_open(SHM_NAME, O_RDWR, 0666);
    if (fd < 0) {
        perror("shm_open");
        exit(EXIT_FAILURE);
    }

    sharedmemory = (SharedMemory*)mmap(nullptr, sizeof(SharedMemory), PROT_READ | PROT_WRITE,
                                       MAP_SHARED, fd, 0);
    if (sharedmemory == MAP_FAILED) {
        perror("mmap");
        exit(EXIT_FAILURE);
    }
}

void ChatClient::login() {
    pthread_mutex_lock(&sharedmemory->users_mutex);
    for (int i = 0; i < MAX_USERS; ++i) {
        if (sharedmemory->usernames[i][0] == '\0') {
            my_index = i;
            break;
        }
    }
    pthread_mutex_unlock(&sharedmemory->users_mutex);

    Message msg{};
    strncpy(msg.from, username.c_str(), MAX_NAME);
    strcpy(msg.text, "LOGIN");
    send_to_server(msg);
}

void ChatClient::logout() {
    Message message{};
    strncpy(message.from, username.c_str(), MAX_NAME);
    strcpy(message.text, "LOGOUT");
    send_to_server(message);
}

void ChatClient::send_to_server(const Message& msg) {
    pthread_mutex_lock(&sharedmemory->server_queue.mutex);
    sharedmemory->server_queue.messages[sharedmemory->server_queue.count++] = msg;
    pthread_cond_signal(&sharedmemory->server_queue.cond);
    pthread_mutex_unlock(&sharedmemory->server_queue.mutex);
}

void ChatClient::send_message(const std::string& input) {
    auto pos = input.find(':');
    if (pos == std::string::npos) {
        std::cout << "Format: recipient:message" << std::endl;
        return;
    }

    Message message{};
    strncpy(message.from, username.c_str(), MAX_NAME);
    strncpy(message.to, input.substr(0, pos).c_str(), MAX_NAME);
    strncpy(message.text, input.substr(pos + 1).c_str(), MAX_TEXT);

    send_to_server(message);
}

void ChatClient::receiver_loop() {
    auto& queue = sharedmemory->client_queues[my_index];

    while (running) {
        pthread_mutex_lock(&queue.mutex);

        while (queue.count == 0 && running)
            pthread_cond_wait(&queue.cond, &queue.mutex);

        if (!running) {
            pthread_mutex_unlock(&queue.mutex);
            break;
        }

        Message message = queue.messages[0];
        for (int i = 1; i < queue.count; ++i)
            queue.messages[i - 1] = queue.messages[i];
        queue.count--;

        pthread_mutex_unlock(&queue.mutex);

        std::cout << std::endl
                  << "[message] " << message.from << ": " << message.text << std::endl
                  << "> ";
        std::cout.flush();
    }
}

void ChatClient::run() {
    std::string line;
    std::cout << "Format: recipient:message (type 'quit' to exit)" << std::endl;

    while (true) {
        std::cout << "> ";
        std::getline(std::cin, line);

        if (line == "quit")
            break;

        send_message(line);
    }

    stop();
}

void ChatClient::stop() {
    if (!running)
        return;

    running = false;
    logout();

    auto& queue = sharedmemory->client_queues[my_index];
    pthread_mutex_lock(&queue.mutex);
    pthread_cond_signal(&queue.cond);
    pthread_mutex_unlock(&queue.mutex);

    if (receiver_thread.joinable())
        receiver_thread.join();

    std::cout << std::endl;
}
