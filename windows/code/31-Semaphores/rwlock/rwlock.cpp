#include <format>
#include <iostream>

#include "reader_writer_lock.h"

size_t read_loops;
size_t write_loops;
size_t counter = 0;

reader_writer_lock mutex;

DWORD WINAPI reader(const LPVOID const arg) {
    size_t local;
    for (size_t i = 0; i < read_loops; ++i) {
        mutex.acquire_read_lock();
        local = counter;
        mutex.release_read_lock();
        std::cout << std::format("read: {}", local) << std::endl;
    }
    std::cout << std::format("read done: {}", local) << std::endl;
    return EXIT_SUCCESS;
}

DWORD WINAPI writer(const LPVOID const arg) {
    for (size_t i = 0; i < write_loops; ++i) {
        mutex.acquire_write_lock();
        ++counter;
        mutex.release_write_lock();
    }
    std::cout << "write done" << std::endl;
    return EXIT_SUCCESS;
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cout << std::format("usage: {} readloops writeloops", argv[0]) << std::endl;
        return EXIT_FAILURE;
    }
    read_loops = atoi(argv[1]);
    write_loops = atoi(argv[2]);

    const HANDLE const c1 = CreateThread(nullptr, 0, reader, nullptr, 0, nullptr);
    if (c1 == nullptr) { return EXIT_FAILURE; }
    const HANDLE const c2 = CreateThread(nullptr, 0, writer, nullptr, 0, nullptr);
    if (c2 == nullptr) { return EXIT_FAILURE; }
    WaitForSingleObject(c1, INFINITE);
    WaitForSingleObject(c2, INFINITE);
    std::cout << "all done" << std::endl;
    CloseHandle(c1);
    CloseHandle(c2);
    return EXIT_SUCCESS;
}