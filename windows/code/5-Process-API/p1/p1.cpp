#include <format>
#include <iostream>

#include <Windows.h>

#include "../common.h"

int main(int argc, char* argv[]) {
    constexpr size_t max_path_length = 32767;
    std::cout << std::format("hello world {}", GetCurrentProcessId()) << std::endl;
    {
        std::wstring cwd(max_path_length, L'\0');
        const DWORD l = GetCurrentDirectoryW(max_path_length, cwd.data());
        cwd.resize(l);
        std::wcout << std::format(L"{:12}{}", L"cwd:", cwd) << std::endl;
    }
    std::wstring executable_path_name = LR"(\\?\)";
    {
        executable_path_name.resize(max_path_length);
        const DWORD l = GetModuleFileNameW(nullptr, executable_path_name.data() + 4, max_path_length);
        executable_path_name.resize(l);
    }
    std::cout << std::format("{:12}{}", "argv[0]:", argv[0]) << std::endl;
    std::wcout << std::format(L"{:12}{}", L"Executable:", executable_path_name) << std::endl;

    STARTUPINFO startup_info{ .cb = sizeof(startup_info) };
    PROCESS_INFORMATION process_info;
    const BOOL ret = CreateProcess(executable_path_name.c_str(), nullptr, nullptr, nullptr, FALSE, 0, nullptr, nullptr, &startup_info, &process_info);
    if (ret != 0) {
        const DWORD PPID = get_parent_PID();
        std::cout << std::format("Parent PID: {}", PPID) << std::endl;
    }
    else {
        std::cout << std::format("CreateProcess failed: {}", GetLastError()) << std::endl;
    }
    return 0;
}