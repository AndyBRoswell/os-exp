#include <format>
#include <iostream>

#include <Windows.h>

constexpr size_t thread_count = 2;
constexpr size_t increment_count = 1e7;

HANDLE const mutex = CreateSemaphore(nullptr, 1, LONG_MAX, nullptr);
volatile size_t counter = 0;

DWORD WINAPI child(const LPVOID const arg) {
    for (size_t i = 0; i < increment_count; ++i) {
        WaitForSingleObject(mutex, INFINITE);
        ++counter;
        ReleaseSemaphore(mutex, 1, nullptr);
    }
    return EXIT_SUCCESS;
}

int main(int argc, char* argv[]) {
    HANDLE thread[thread_count];
    for (size_t i = 0; i < thread_count; ++i) { thread[i] = CreateThread(nullptr, 0, child, nullptr, 0, nullptr); }
    WaitForMultipleObjects(thread_count, thread, TRUE, INFINITE);
    //std::cout << std::format("result: {} (should be {})", counter, increment_count * thread_count) << std::endl;
    std::cout << "result: " << counter << " (should be " << increment_count * thread_count << ")" << std::endl;
    return EXIT_SUCCESS;
}