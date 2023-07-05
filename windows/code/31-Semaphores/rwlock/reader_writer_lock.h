#pragma once

#include <Windows.h>

class reader_writer_lock {
public:
    reader_writer_lock();
    ~reader_writer_lock();
    void acquire_read_lock();
    void release_read_lock();
    void acquire_write_lock();
    void release_write_lock();
private:
    HANDLE lock = CreateSemaphore(nullptr, 1, LONG_MAX, nullptr);
    HANDLE write_lock = CreateSemaphore(nullptr, 1, LONG_MAX, nullptr);
    size_t readers = 0;
};