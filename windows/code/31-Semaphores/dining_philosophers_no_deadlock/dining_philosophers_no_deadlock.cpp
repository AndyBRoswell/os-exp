#include <format>
#include <iostream>

#include <Windows.h>

size_t loop_count;

HANDLE fork[5];

size_t left(size_t p) { return p; }

size_t right(size_t p) { return (p + 1) % 5; }

void get_forks(size_t p) {
    if (p == 4) {
        WaitForSingleObject(fork[right(p)], INFINITE);
        WaitForSingleObject(fork[left(p)], INFINITE);
    }
    else {
        WaitForSingleObject(fork[left(p)], INFINITE);
        WaitForSingleObject(fork[right(p)], INFINITE);
    }
}

void put_forks(size_t p) {
    ReleaseSemaphore(fork[left(p)], 1, nullptr);
    ReleaseSemaphore(fork[right(p)], 1, nullptr);
}

void think() { return; }

void eat() { return; }

DWORD WINAPI philosopher(void* arg) {
    const size_t p = reinterpret_cast<size_t>(arg);
    for (size_t i = 0; i < loop_count; ++i) {
        think();
        get_forks(p);
        eat();
        put_forks(p);
    }
    return EXIT_SUCCESS;
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cout << std::format("usage: {} <num_loops>", argv[0]) << std::endl;
        return EXIT_FAILURE;
    }
    std::cout << "dining: started" << std::endl;

    for (size_t i = 0; i < 5; ++i) {
        fork[i] = CreateSemaphore(nullptr, 1, LONG_MAX, nullptr);
        if (fork[i] == nullptr) { std::cerr << "CreateSemaphore failed" << std::endl; return EXIT_FAILURE; }
    }

    HANDLE p[5];
    loop_count = atoi(argv[1]);
    for (size_t i = 0; i < 5; ++i) {
        p[i] = CreateThread(nullptr, 0, philosopher, reinterpret_cast<LPVOID>(i), 0, nullptr);
        if (p[i] == nullptr) { std::cerr << "CreateThread failed" << std::endl; return EXIT_FAILURE; }
    }

    WaitForMultipleObjects(5, p, TRUE, INFINITE);

    std::cout << "dining: finished" << std::endl;
    for (size_t i = 0; i < 5; ++i) { CloseHandle(p[i]); CloseHandle(fork[i]); }
    return EXIT_SUCCESS;
}