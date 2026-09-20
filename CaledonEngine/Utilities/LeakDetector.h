// LeakDetector.h
// Lightweight heap-leak detector for MSVC / Visual Studio.
//
// Setup : add LeakDetector.h and LeakDetector.cpp to your project.
// Usage : #include "LeakDetector.h"
//         int main() { LeakDetector::Start(); /* ... */ }
//
// Every allocation made through global operator new after Start() is recorded with
// its call stack. When the process exits, anything never freed is printed to the
// console, grouped by allocation site. LeakDetector::Report() prints the currently
// live allocations at any point (e.g. after unloading a level).
//
// Active in Debug builds only (_DEBUG). Override with LEAK_DETECTOR_ENABLED=0/1.
#pragma once

#ifndef LEAK_DETECTOR_ENABLED
    #if defined(_WIN32) && defined(_DEBUG)
        #define LEAK_DETECTOR_ENABLED 1
    #else
        #define LEAK_DETECTOR_ENABLED 0
    #endif
#endif

namespace LeakDetector
{
#if LEAK_DETECTOR_ENABLED
    void Start();   // begin tracking; leaks are reported automatically at process exit
    void Report();  // print everything allocated since Start() that is still alive
#else
    inline void Start() {}
    inline void Report() {}
#endif
}
