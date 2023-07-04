#include <format>
#include <iostream>
#include <vector>

#include <Windows.h>

using Vty = size_t;

size_t maximum;
size_t loops;
size_t consumers;

Vty* buffer;
constexpr Vty end_marker = -1;

SSIZE_T use_pos;
SSIZE_T fill_pos;
size_t num_full;

CONDITION_VARIABLE empty;
CONDITION_VARIABLE fill;
CRITICAL_SECTION m;

void do_fill(Vty value) {
    buffer[fill_pos] = value;
    fill_pos = (fill_pos + 1) % maximum;
    ++num_full;
}

Vty do_get() {
    const Vty tmp = buffer[use_pos];
    use_pos = (use_pos + 1) % maximum;
    --num_full;
    return tmp;
}

DWORD producer(const LPVOID const arg) {
    for (size_t i = 0; i < loops; ++i) {
        EnterCriticalSection(&m);
        while (num_full == maximum) { SleepConditionVariableCS(&empty, &m, INFINITE); }
        do_fill(i);
        WakeConditionVariable(&fill);
        LeaveCriticalSection(&m);
    }
    for (size_t i = 0; i < consumers; ++i) { // end case: put an end-of-production marker into shared buffer, 1 per consumer
        EnterCriticalSection(&m);
        while (num_full == maximum) { SleepConditionVariableCS(&empty, &m, INFINITE); }
        do_fill(end_marker);
        WakeConditionVariable(&fill);
        LeaveCriticalSection(&m);
    }
    return EXIT_SUCCESS;
}

DWORD consumer(const LPVOID const arg) {
    Vty tmp = 0;
    while (tmp != end_marker) {
        EnterCriticalSection(&m);
        while (num_full == 0) { SleepConditionVariableCS(&fill, &m, INFINITE); }
        tmp = do_get();
        WakeConditionVariable(&empty);
        LeaveCriticalSection(&m);
    }
    return EXIT_SUCCESS;
}

int main(int argc, char* argv[]) {
    if (argc != 4) {
        std::cerr << std::format("usage: {} <buffersize> <loops> <consumers>", argv[0]) << std::endl;
        return EXIT_FAILURE;
    }
    maximum = atoi(argv[1]);
    loops = atoi(argv[2]);
    consumers = atoi(argv[3]);

    buffer = new Vty[maximum]();

    const HANDLE const producer_thread = CreateThread(nullptr, 0, producer, nullptr, 0, nullptr);
    if (producer_thread == nullptr) { return EXIT_FAILURE; }
    std::vector<HANDLE> consumer_thread(consumers);
    for (size_t i = 0; i < consumers; ++i) {
        consumer_thread[i] = CreateThread(nullptr, 0, consumer, nullptr, 0, nullptr);
        if (consumer_thread[i] == nullptr) { return EXIT_FAILURE; }
    }
    WaitForSingleObject(producer_thread, INFINITE);
    for (size_t i = 0; i < consumers; ++i) { WaitForSingleObject(consumer_thread[i], INFINITE); }
    return EXIT_SUCCESS;
}