#include "reader_writer_lock.h"

reader_writer_lock::reader_writer_lock() {}

reader_writer_lock::~reader_writer_lock() {
    CloseHandle(lock);
    CloseHandle(write_lock);
}

void reader_writer_lock::acquire_read_lock() {
    WaitForSingleObject(lock, INFINITE);
    ++readers;
    if (readers == 1) { WaitForSingleObject(write_lock, INFINITE); }
    ReleaseSemaphore(lock, 1, nullptr);
}

void reader_writer_lock::release_read_lock() {
    WaitForSingleObject(lock, INFINITE);
    --readers;
    if (readers == 0) { ReleaseSemaphore(write_lock, 1, nullptr); }
    ReleaseSemaphore(lock, 1, nullptr);
}

void reader_writer_lock::acquire_write_lock() {
    WaitForSingleObject(write_lock, INFINITE);
}

void reader_writer_lock::release_write_lock() {
    ReleaseSemaphore(write_lock, 1, nullptr);
}
