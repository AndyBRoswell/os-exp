#include <iostream>

#include <Windows.h>

HANDLE const s = CreateSemaphore(nullptr, 0, LONG_MAX, nullptr);

DWORD WINAPI child(const LPVOID arg) {
    Sleep(2);
    std::cout << "child" << std::endl;
    ReleaseSemaphore(s, 1, nullptr); // signal here: child is done
    return EXIT_SUCCESS;
}

int main(int argc, char* argv[]) {
    std::cout << "parent: begin" << std::endl;
    const HANDLE c = CreateThread(nullptr, 0, child, nullptr, 0, nullptr);
    if (c == nullptr) { std::cerr << "CreateThread failed" << std::endl; return EXIT_FAILURE; }
    WaitForSingleObject(s, INFINITE); // wait here for child
    std::cout << "parent: end" << std::endl;
    CloseHandle(c);
    CloseHandle(s);
    return EXIT_SUCCESS;
}