#include <format>
#include <iostream>

int main() {
    std::cout << std::format("{:20}{:#018x}", "location of code:", reinterpret_cast<intptr_t>(main)) << std::endl;
    std::cout << std::format("{:20}{:#018x}", "location of stack:", reinterpret_cast<intptr_t>(malloc(1))) << std::endl;
    int x = 5;
    std::cout << std::format("{:20}{:#018x}", "location of heap:", reinterpret_cast<intptr_t>(&x)) << std::endl;
    return 0;
}
