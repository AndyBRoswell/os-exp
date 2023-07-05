#include <format>
#include <iostream>
#include <vector>

#include <Windows.h>

HANDLE s;

DWORD WINAPI child(const LPVOID arg) {
    WaitForSingleObject(s, INFINITE);
    std::cout << std::format("child {}", reinterpret_cast<size_t>(arg)) << std::endl;
    Sleep(1);
    ReleaseSemaphore(s, 1, nullptr);
    return EXIT_SUCCESS;
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cout << std::format("usage: {} <num_threads> <sem_value>", argv[0]) << std::endl;
        return EXIT_FAILURE;
    }
    const size_t num_threads = atoi(argv[1]);
    s = CreateSemaphore(nullptr, atoi(argv[2]), LONG_MAX, nullptr);
    std::cout << "parent: begin" << std::endl;
    std::vector<HANDLE> c(num_threads);
    for (size_t i = 0; i < num_threads; ++i) {
        c[i] = CreateThread(nullptr, 0, child, reinterpret_cast<LPVOID>(i), 0, nullptr);
        if (c[i] == nullptr) { std::cerr << "CreateThread failed" << std::endl; return EXIT_FAILURE; }
    }
    WaitForMultipleObjects(num_threads, c.data(), TRUE, INFINITE);
    std::cout << "parent: end" << std::endl;
    for (size_t i = 0; i < num_threads; ++i) { CloseHandle(c[i]); }
    CloseHandle(s);
    return EXIT_SUCCESS;
}