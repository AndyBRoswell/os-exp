#include <format>
#include <iostream>

#include <Windows.h>

#include "../common.h"

int main(int argc, char* argv[]) {
    constexpr size_t max_path_length = 32767;

    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);

    const DWORD PID = GetCurrentProcessId();
    std::cout << std::format("hello world {}", PID) << std::endl;
    {
        std::wstring cwd(max_path_length, L'\0');
        const DWORD l = GetCurrentDirectory(max_path_length, cwd.data());
        cwd.resize(l);
        std::wcout << std::format(L"{:12}{}", L"cwd:", cwd) << std::endl;
    }
    std::wstring executable_path_name; {
        if (std::string(argv[0]).starts_with(R"(\\?\)") == false) { executable_path_name = LR"(\\?\)"; }
        const size_t l0 = executable_path_name.size();
        executable_path_name.resize(max_path_length);
        const DWORD l = GetModuleFileNameW(nullptr, executable_path_name.data() + l0, max_path_length - l0);
        executable_path_name.resize(l + l0);
        std::cout << std::format("{:12}{}", "argv[0]:", argv[0]) << std::endl;
        std::wcout << std::format(L"{:12}{}", L"Executable:", executable_path_name) << std::endl;
    }

    STARTUPINFO startup_info{ .cb = sizeof(startup_info) };
    PROCESS_INFORMATION process_info;
    if (argc == 1) {
        const BOOL ret = CreateProcess(nullptr, (executable_path_name + std::format(L" {}", PID)).data(), nullptr, nullptr, FALSE, 0, nullptr, nullptr, &startup_info, &process_info);
        if (ret != 0) { std::cout << std::format("hello, I am parent of {}. Parent PID: {}.", process_info.dwProcessId, get_parent_PID()) << std::endl; }
        else {
            std::cout << std::format("CreateProcess failed. Error code: {}.", GetLastError()) << std::endl;
            return EXIT_FAILURE;
        }
        CloseHandle(process_info.hProcess);
        CloseHandle(process_info.hThread);
    }
    else { std::cout << std::format("hello, I am child {}. Parent PID: {}", GetCurrentProcessId(), get_parent_PID()) << std::endl; }
    return EXIT_SUCCESS;
}