#include "synchronizer.h"

synchronizer::synchronizer() {
    InitializeConditionVariable(&c);
    InitializeCriticalSection(&m);
}

void synchronizer::wait() {
    EnterCriticalSection(&m);
    while (done == false) {SleepConditionVariableCS(&c, &m, INFINITE);}
    done = 0; // reset for next use
    LeaveCriticalSection(&m);
}

void synchronizer::signal() {
    EnterCriticalSection(&m);
    done = true;
    WakeConditionVariable(&c);
    LeaveCriticalSection(&m);
}