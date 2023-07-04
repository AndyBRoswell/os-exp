#include <format>
#include <iostream>

#include <Windows.h>

int main(int argc, char* argv[]) {
    using arg_t = struct { size_t a, b; };

    PTHREAD_START_ROUTINE my_thread = [](const LPVOID lpParam) -> DWORD {
        const arg_t* const arg = reinterpret_cast<arg_t*>(lpParam);
        std::cout << std::format("a = {}, b = {}\n", arg->a, arg->b);
        return EXIT_SUCCESS;
    };

    const HANDLE const hThread = CreateThread(nullptr, 0, my_thread, new arg_t{ 1, 2 }, 0, nullptr);
    if (hThread == nullptr) {
        std::cerr << "CreateThread failed\n";
        return EXIT_FAILURE;
    }
    WaitForSingleObject(hThread, INFINITE);
    std::cout << "done" << std::endl;
    CloseHandle(hThread);

    return EXIT_SUCCESS;
}