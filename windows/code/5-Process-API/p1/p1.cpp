#include <format>
#include <iostream>

#include <Windows.h>

#include "../common.h"

int main(int argc, char* argv[]) {
    std::cout << std::format("hello world {}", GetCurrentProcessId()) << std::endl;
    {
        std::wstring cwd(MAX_PATH, L'\0');
        const DWORD l = GetCurrentDirectoryW(MAX_PATH, cwd.data());
        cwd.resize(l);
        std::wcout << std::format(L"{:12}{}", L"cwd:", cwd) << std::endl;
    }
    std::wstring executable_path_name(MAX_PATH, L'\0');
    {
        const DWORD l = GetModuleFileNameW(nullptr, executable_path_name.data(), MAX_PATH);
        executable_path_name.resize(l);
    }
    std::cout << std::format("{:12}{}", "argv[0]:", argv[0]) << std::endl;
    std::wcout << std::format(L"{:12}{}", L"Executable:", executable_path_name) << std::endl;
    const BOOL ret = CreateProcess(executable_path_name.c_str(), nullptr, nullptr, nullptr, FALSE, 0, nullptr, nullptr, nullptr, nullptr);
    if (ret != 0) {
        const DWORD PPID = get_parent_PID();
        std::cout << std::format("Parent PID: {}", PPID) << std::endl;
    }
    else {
        std::cout << std::format("CreateProcess failed: {}", GetLastError()) << std::endl;
    }
    return 0;
}