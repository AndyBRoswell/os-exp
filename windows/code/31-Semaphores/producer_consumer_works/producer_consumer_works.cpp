#include <format>
#include <iostream>
#include <vector>

#include <Windows.h>

using Vty = size_t;

constexpr Vty end_marker = -1;

size_t maximum;
size_t loops;
size_t consumers = 1;

Vty* buffer;

size_t use = 0;
size_t fill = 0;

HANDLE const empty = CreateSemaphore(nullptr, maximum, LONG_MAX, nullptr);  // maximum are empty
HANDLE const full = CreateSemaphore(nullptr, 0, LONG_MAX, nullptr);         // 0 are full
HANDLE const mutex = CreateSemaphore(nullptr, 1, LONG_MAX, nullptr);        // mutex

void do_fill(Vty value) {
    buffer[fill] = value;
    fill = (fill + 1) % maximum;
}

Vty do_get() {
    Vty tmp = buffer[use];
    use = (use + 1) % maximum;
    return tmp;
}

DWORD WINAPI producer(const LPVOID const arg) {
    for (size_t i = 0; i < loops; ++i) {
        WaitForSingleObject(empty, INFINITE);
        WaitForSingleObject(mutex, INFINITE);
        do_fill(i);
        ReleaseSemaphore(mutex, 1, nullptr);
        ReleaseSemaphore(full, 1, nullptr);
    }
    for (size_t i = 0; i < consumers; ++i) { // end case
        WaitForSingleObject(empty, INFINITE);
        WaitForSingleObject(mutex, INFINITE);
        do_fill(end_marker);
        ReleaseSemaphore(mutex, 1, nullptr);
        ReleaseSemaphore(full, 1, nullptr);
    }
    return EXIT_SUCCESS;
}

DWORD WINAPI consumer(const LPVOID const arg) {
    Vty tmp = 0;
    while (tmp != end_marker) {
        WaitForSingleObject(full, INFINITE);
        WaitForSingleObject(mutex, INFINITE);
        tmp = do_get();
        ReleaseSemaphore(mutex, 1, nullptr);
        ReleaseSemaphore(empty, 1, nullptr);
        std::cout << std::format("{} {}", reinterpret_cast<size_t>(arg), tmp) << std::endl;
    }
    return EXIT_SUCCESS;
}

int main(int argc, char* argv[]) {
    if (argc != 4) {
        std::cout << std::format("usage: {} <buffersize> <loops> <consumers>", argv[0]) << std::endl;
        return EXIT_FAILURE;
    }
    maximum = atoi(argv[1]);
    loops = atoi(argv[2]);
    consumers = atoi(argv[3]);

    buffer = new size_t[maximum]();

    HANDLE p;
    std::vector<HANDLE> c(consumers);
    p = CreateThread(nullptr, 0, producer, nullptr, 0, nullptr);
    if (p == nullptr) { std::cerr << "CreateThread failed" << std::endl; return EXIT_FAILURE; }
    for (size_t i = 0; i < consumers; ++i) {
        c[i] = CreateThread(nullptr, 0, consumer, reinterpret_cast<void*>(i), 0, nullptr);
        if (c[i] == nullptr) { std::cerr << "CreateThread failed" << std::endl; return EXIT_FAILURE; }
    }
    WaitForSingleObject(p, INFINITE);
    for (size_t i = 0; i < consumers; ++i) { WaitForSingleObject(c[i], INFINITE); }
    CloseHandle(p);
    for (size_t i = 0; i < consumers; ++i) { CloseHandle(c[i]); }
    CloseHandle(empty);
    CloseHandle(full);
    CloseHandle(mutex);
    delete[] buffer;
    return EXIT_SUCCESS;
}