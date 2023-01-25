#include <string>

#include <Windows.h>

int main(int argc, char* argv[], char* envp[]) {
    LARGE_INTEGER tick_count;
    QueryPerformanceCounter(&tick_count);
    const HANDLE f = CreateFile((std::to_wstring(tick_count.QuadPart) + L".tmp").c_str(), GENERIC_WRITE, 0, nullptr, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, nullptr); // OpenFile has path length limit (128 ch)
    if (f == INVALID_HANDLE_VALUE) exit(EXIT_FAILURE);
    const char content[] = "Hello World";
    DWORD bytes_written;
    const BOOL ret = WriteFile(f, content, sizeof(content), &bytes_written, nullptr); // C library function fwrite(const void *, size_t, size_t, FILE*)) is more recommended
    if (ret != TRUE) exit(EXIT_FAILURE);
    CloseHandle(f);
    return EXIT_SUCCESS;
}