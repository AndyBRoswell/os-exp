#include <iostream>

#include <Windows.h>

CONDITION_VARIABLE c;
CRITICAL_SECTION m;
bool done = false;

DWORD child(const LPVOID const arg) {
    std::cout << "child: begin" << std::endl;
    Sleep(1);
    done = true;
    std::cout << "child: signal" << std::endl;
    WakeConditionVariable(&c);
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
    EnterCriticalSection(&m);
    std::cout << "parent: check condition" << std::endl;
    while (done == false) {
        Sleep(2);
        std::cout << "parent: wait to be signalled..." << std::endl;
        SleepConditionVariableCS(&c, &m, INFINITE);
    }
    LeaveCriticalSection(&m);
    std::cout << "parent: end" << std::endl;
    CloseHandle(child_thread);
    return EXIT_SUCCESS;
}