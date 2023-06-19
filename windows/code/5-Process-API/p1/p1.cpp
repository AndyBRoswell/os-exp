#include <format>
#include <iostream>

#include <Windows.h>

int main(int argc, char* argv[]) {
    std::cout << std::format("hello world {}", GetCurrentProcessId()) << std::endl;
    return 0;
}