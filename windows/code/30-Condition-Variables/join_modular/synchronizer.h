#pragma once

#include <Windows.h>

class synchronizer {
public:
    synchronizer();
    void wait();
    void signal();
private:
    CONDITION_VARIABLE c;
    CRITICAL_SECTION m; // similar to pthread_mutex_t
    bool done = false;
};