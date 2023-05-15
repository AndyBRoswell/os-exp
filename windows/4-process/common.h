#include <Windows.h>
#include <TlHelp32.h>
#include <Psapi.h>

DWORD get_parent_PID();

std::wstring get_process_name(DWORD PID);