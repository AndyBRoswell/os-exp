#include <format>
#include <iostream>

#include <Windows.h>

int main(int argc, char* argv[]) {
    struct dat_t {
        const struct { size_t a, b; } arg;
        struct { size_t x, y; } ret;
    };
    auto dat = new dat_t{ .arg = { 10,20 } };
    const HANDLE const hThread = CreateThread(
        nullptr,
        0,
        [](const LPVOID arg) -> DWORD {
            dat_t* const dat = reinterpret_cast<dat_t*>(arg);
            std::cout << std::format("args {} {}\n", dat->arg.a, dat->arg.b);
            dat->ret = { .x = 1, .y = 2 };
            return EXIT_SUCCESS;
        },
        dat,
        0,
        nullptr
    );
    if (hThread == nullptr) {
        std::cerr << "CreateThread failed\n";
        return EXIT_FAILURE;
    }
    WaitForSingleObject(hThread, INFINITE);
    std::cout<<std::format("returned {} {}\n", dat->ret.x, dat->ret.y);
    CloseHandle(hThread);
    return EXIT_SUCCESS;
}