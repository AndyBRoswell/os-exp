#include <iostream>

#include "synchronizer.h"

class synchronizer synchronizer;

DWORD child(const LPVOID const arg) {
    std::cout << "child" << std::endl;
    Sleep(1);
    synchronizer.signal();
    return EXIT_SUCCESS;
}

int main(int argc, char* argv[]) {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);
    std::cout << "parent: begin" << std::endl;
    const HANDLE const child_thread = CreateThread(nullptr, 0, child, nullptr, 0, nullptr);
    synchronizer.wait();
    std::cout << "parent: end" << std::endl;
    CloseHandle(child_thread);
    return EXIT_SUCCESS;
}