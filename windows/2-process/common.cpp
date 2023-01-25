#include "common.h"

#include <stdexcept>

DWORD get_parent_PID() {
    const DWORD pid = GetCurrentProcessId();
    const HANDLE process_snapshot_handle = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (process_snapshot_handle == INVALID_HANDLE_VALUE) throw std::runtime_error("Failed to create process snapshot via Toolhelp32");
    PROCESSENTRY32 process_entry{ .dwSize = sizeof(PROCESSENTRY32) };
    if (Process32First(process_snapshot_handle, &process_entry)) {
        do {
            if (process_entry.th32ProcessID == pid) return process_entry.th32ParentProcessID;
        } while (Process32Next(process_snapshot_handle, &process_entry));
    }
    else throw std::runtime_error("Failed to read the 1st process entry via Toolhelp32");
}

std::wstring get_process_name(DWORD PID) {
    const HANDLE process_handle = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, PID);
    if (process_handle == nullptr) throw std::runtime_error("Failed to get process handle");
    constexpr DWORD lmax = 32767;
    TCHAR process_name[lmax];
    GetModuleFileNameEx(parent_process_handle, nullptr, process_name, lmax);
}