#include <iostream>

#include <Windows.h>

CONDITION_VARIABLE c;
CRITICAL_SECTION m;

DWORD child(const LPVOID const arg) {
    std::cout << "child: begin" << std::endl;
    EnterCriticalSection(&m);
    std::cout << "child: signal" << std::endl;
    WakeConditionVariable(&c);
    LeaveCriticalSection(&m);
    return EXIT_SUCCESS;
}

int main(int argc, char* argv[]) {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);
    InitializeConditionVariable(&c);
    InitializeCriticalSection(&m);
    std::cout << "parent: begin" << std::endl;
    const HANDLE const child_thread = CreateThread(nullptr, 0, child, nullptr, 0, nullptr);
    Sleep(2);
    std::cout << "parent: wait to be signalled..." << std::endl;
    EnterCriticalSection(&m);
    SleepConditionVariableCS(&c, &m, INFINITE);
    LeaveCriticalSection(&m);
    std::cout << "parent: end" << std::endl;
    return EXIT_SUCCESS;
}