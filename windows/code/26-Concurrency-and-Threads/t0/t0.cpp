#include <format>
#include <iostream>

#include <Windows.h>

DWORD my_thread(LPVOID arg) {
    std::cout << std::format("{}\n", reinterpret_cast<const char*>(arg));
    return 0;
}

int main(int argc, char* argv[]) {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);
    HANDLE t[2];
    std::cout << "main: begin" << std::endl;
    t[0] = CreateThread(nullptr, 0, my_thread, const_cast<char*>("A"), 0, nullptr);
    t[1] = CreateThread(nullptr, 0, my_thread, const_cast<char*>("B"), 0, nullptr);
    WaitForSingleObject(t[0], INFINITE);
    WaitForSingleObject(t[1], INFINITE);
    std::cout<< "main: end" << std::endl;
    return 0;
}