#include "common.h"

double get_time() {
    LARGE_INTEGER count, frequency;
    QueryPerformanceFrequency(&frequency);
    QueryPerformanceCounter(&count);
    return static_cast<double>(count.QuadPart) / frequency.QuadPart;
}

void spin(double sec) {
    const double t = get_time();
    while (get_time() - t < sec) {}
}