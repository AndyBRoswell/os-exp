#include <iostream>

#include "../common.h"

int main(int argc, char* argv[], char* envp[]) {
    if (argc != 2) {
        std::cerr << "Usage:\ncpu <string>" << std::endl;
        exit(1);
    }
    while (true) {
        std::cout << argv[1] << std::endl;
        spin(1);
    }
    return 0;
}