#include <format>
#include <iostream>

#include <Windows.h>

int main(int argc, char* argv[]) {
    if (argc != 4) {
        std::cerr << std::format("usage: {} <buffersize> <loops> <consumers>", argv[0]) << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}