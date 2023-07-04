#include <format>
#include <iostream>

#include <Windows.h>

int main(int argc, char* argv[]) {
    const HANDLE const hThread = CreateThread(
        nullptr,
        0,
        [](const LPVOID arg) -> DWORD {
            const size_t value = reinterpret_cast<size_t>(arg);
            std::cout << std::format("value = {}\n", value);
            return EXIT_SUCCESS;
        },
        reinterpret_cast<LPVOID>(100),
        0,
        nullptr
    );
    if (hThread == nullptr) {
        std::cerr << "CreateThread failed\n";
        return EXIT_FAILURE;
    }
    WaitForSingleObject(hThread, INFINITE);
    DWORD ret;
    GetExitCodeThread(hThread, &ret);
    std::cout<<std::format("returned {}\n", ret);
    return EXIT_SUCCESS;
}