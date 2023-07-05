#include <format>
#include <iostream>

#include <Windows.h>

size_t loop_count;

HANDLE fork[5];
HANDLE const print_lock = CreateSemaphore(nullptr, 1, LONG_MAX, nullptr);

void space(size_t s) {
    WaitForSingleObject(print_lock, INFINITE);
    for (size_t i = 0; i < 10 * s; ++i) { std::cout << " "; }
}

void space_end() { ReleaseSemaphore(print_lock, 1, nullptr); }

size_t left(size_t p) { return p; }

size_t right(size_t p) { return (p + 1) % 5; }

void get_forks(size_t p) {
    WaitForSingleObject(fork[left(p)], INFINITE);
    WaitForSingleObject(fork[right(p)], INFINITE);
}

void put_forks(size_t p) {
    ReleaseSemaphore(fork[left(p)], 1, nullptr);
    ReleaseSemaphore(fork[right(p)], 1, nullptr);
}

void think() { return; }

void eat() { return; }

DWORD WINAPI philosopher(void* arg) {
    const size_t p = reinterpret_cast<size_t>(arg);
    space(p); std::cout << std::format("{}: start", p) << std::endl; space_end();
    for (size_t i = 0; i < loop_count; ++i) {
        space(p); std::cout << std::format("{}: think", p) << std::endl; space_end();
        think();
        get_forks(p);
        space(p); std::cout << std::format("{}: eat", p) << std::endl; space_end();
        eat();
        put_forks(p);
        space(p); std::cout << std::format("{}: done", p) << std::endl; space_end();
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
        fork[i] = CreateSemaphore(nullptr, 1, 1, nullptr);
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