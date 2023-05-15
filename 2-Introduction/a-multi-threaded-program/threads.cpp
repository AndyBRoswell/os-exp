#include <iostream>

#include <Windows.h>

constexpr size_t thread_count = 2;

volatile unsigned long long counter;
unsigned long long loops;

DWORD WINAPI worker(void* arg) {
    for (unsigned long long i = 0; i < loops; ++i) { ++counter; }
    return EXIT_SUCCESS;
}

int main(int argc, char* argv[], char* envp[]) {
    if (argc != 2) {
        std::cerr << "usage\n./a-multi-threaded-program <value>" << std::endl;
        exit(EXIT_FAILURE);
    }
    loops = std::strtoull(argv[1], reinterpret_cast<char**>(argv[1]) + strlen(argv[1]), 10);
    DWORD thread_id[thread_count];
    HANDLE thread_handle[thread_count];
    for (size_t i = 0; i < thread_count; ++i) { thread_handle[i] = CreateThread(nullptr, 0, worker, nullptr, 0, &thread_id[i]); }
    for (size_t i = 0; i < thread_count; ++i) { WaitForSingleObject(thread_handle[i], INFINITE); }
    std::cout << "Final value: " << counter << std::endl;
    for (size_t i = 0; i < thread_count; ++i) { CloseHandle(thread_handle[i]); }
    return EXIT_SUCCESS;
}