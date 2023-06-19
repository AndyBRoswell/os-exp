#include <iostream>

#include "../common.h"

int main(int argc, char* argv[], char* envp[]) {
    int* p = new int;
    if (p == nullptr) exit(1);
    std::cout << "Address pointed to by p: " << p << std::endl;
    *p = 0;
    while (true) {
        spin(1);
        ++* p;
        std::cout << "(" << GetCurrentProcessId() << ") p:" << *p << std::endl;
    }
    return 0;
}