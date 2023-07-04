#include <iostream>

#include <Windows.h>

CONDITION_VARIABLE c;
CRITICAL_SECTION m; // similar to pthread_mutex_t
bool done = false;

DWORD child(const LPVOID const arg) {
    std::cout << "child" << std::endl;
    Sleep(1);
    EnterCriticalSection(&m);
    done = true;
    WakeConditionVariable(&c);
    LeaveCriticalSection(&m);
    return EXIT_SUCCESS;
}

int main(int argc, char* argv[]) {
    InitializeConditionVariable(&c);
    InitializeCriticalSection(&m);
    std::cout << "parent: begin" << std::endl;
    const HANDLE const child_thread = CreateThread(
        nullptr,
        0,
        child, // If lambda is used, then you can only pass a capture-less lambda here.
        nullptr,
        0,
        nullptr
    );
    EnterCriticalSection(&m);
    while (done == false) { 
        SleepConditionVariableCS(&c, &m, INFINITE);
    }
    LeaveCriticalSection(&m);
    std::cout << "parent: end" << std::endl;
    DeleteCriticalSection(&m);
    return EXIT_SUCCESS;
}