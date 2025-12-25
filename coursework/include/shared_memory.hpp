#pragma once

#include <pthread.h>

constexpr int MAX_USERS = 8;
constexpr int MAX_QUEUE = 32;
constexpr int MAX_TEXT = 256;
constexpr int MAX_NAME = 32;
constexpr int MAX_HISTORY = 256;

struct Message {
    char from[MAX_NAME];
    char to[MAX_NAME];
    char text[MAX_TEXT];
};

struct Queue {
    pthread_mutex_t mutex;
    pthread_cond_t cond;
    int count;
    Message messages[MAX_QUEUE];
};

struct HistoryEntry {
    char from[MAX_NAME];
    char to[MAX_NAME];
    char text[MAX_TEXT];
};

struct SharedMemory {
    pthread_mutex_t users_mutex;

    char usernames[MAX_USERS][MAX_NAME];
    Queue client_queues[MAX_USERS];

    Queue server_queue;

    HistoryEntry history[MAX_HISTORY];
    int history_count;
};
