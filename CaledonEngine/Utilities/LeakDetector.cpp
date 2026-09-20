// LeakDetector.cpp
// NOTE: if your project uses precompiled headers, set this file to
//       C/C++ > Precompiled Headers > "Not Using Precompiled Headers".
#include "LeakDetector.h"

#if LEAK_DETECTOR_ENABLED

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#include <dbghelp.h>
#include <malloc.h>

#include <algorithm>
#include <atomic>
#include <cstdarg>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <mutex>
#include <new>
#include <unordered_map>
#include <vector>

#pragma comment(lib, "dbghelp.lib")

// Construct our globals before any other translation unit's, so they are destroyed
// after all of them. Global objects have released their memory by the time we report.
#pragma warning(disable : 4073)
#pragma init_seg(lib)

namespace
{
    constexpr unsigned kMaxFrames     = 16;  // call-stack depth recorded per allocation
    constexpr unsigned kFramesPrinted = 8;   // frames shown per leak in the report

    // malloc-backed allocator so the tracker's own storage never re-enters operator new.
    template <class T>
    struct RawAllocator
    {
        using value_type = T;
        RawAllocator() = default;
        template <class U> RawAllocator(const RawAllocator<U>&) noexcept {}

        T* allocate(std::size_t n)
        {
            if (void* p = std::malloc(n * sizeof(T))) return static_cast<T*>(p);
            throw std::bad_alloc();
        }
        void deallocate(T* p, std::size_t) noexcept { std::free(p); }

        template <class U> bool operator==(const RawAllocator<U>&) const noexcept { return true; }
        template <class U> bool operator!=(const RawAllocator<U>&) const noexcept { return false; }
    };

    struct Record
    {
        std::size_t size;
        unsigned    depth;
        void*       frames[kMaxFrames];
    };

    using RecordMap = std::unordered_map<void*, Record, std::hash<void*>, std::equal_to<void*>,
                                         RawAllocator<std::pair<void* const, Record>>>;

    std::atomic<bool> g_active{ false };
    thread_local bool t_busy = false;  // true while this thread is inside the detector (skip tracking)

    void ReportLeaks();

    struct Tracker
    {
        std::mutex mutex;
        RecordMap  records;
        ~Tracker() { if (g_active.exchange(false)) ReportLeaks(); }
    };
    Tracker g_tracker;

    // ---- output ---------------------------------------------------------------------------

    void Print(const char* fmt, ...)
    {
        char buffer[1024];
        va_list args;
        va_start(args, fmt);
        std::vsnprintf(buffer, sizeof buffer, fmt, args);
        va_end(args);

        std::fputs(buffer, stdout);
        if (IsDebuggerPresent()) OutputDebugStringA(buffer);  // also mirror to the VS Output window
    }

    // Prints one stack frame. Returns true if it is the program entry point (stop walking).
    bool PrintFrame(HANDLE process, bool haveSymbols, void* frame)
    {
        if (!haveSymbols) { Print("      %p\n", frame); return false; }

        const DWORD64 address = static_cast<DWORD64>(reinterpret_cast<std::uintptr_t>(frame));

        alignas(SYMBOL_INFO) char symbolBuffer[sizeof(SYMBOL_INFO) + 255] = {};
        SYMBOL_INFO* symbol = reinterpret_cast<SYMBOL_INFO*>(symbolBuffer);
        symbol->SizeOfStruct = sizeof(SYMBOL_INFO);
        symbol->MaxNameLen   = 255;

        DWORD64 displacement = 0;
        const char* name = SymFromAddr(process, address, &displacement, symbol) ? symbol->Name : "<unknown>";

        IMAGEHLP_LINE64 line = {};
        line.SizeOfStruct = sizeof(line);
        DWORD lineDisplacement = 0;
        // frames hold return addresses; step back one byte to land inside the call instruction
        if (SymGetLineFromAddr64(process, address - 1, &lineDisplacement, &line))
            Print("      %s  (%s:%lu)\n", name, line.FileName, line.LineNumber);
        else
            Print("      %s\n", name);

        return std::strcmp(name, "main") == 0  || std::strcmp(name, "wmain") == 0 ||
               std::strcmp(name, "WinMain") == 0 || std::strcmp(name, "wWinMain") == 0;
    }

    void ReportLeaks()
    {
        // Anything this thread allocates while reporting must not be tracked.
        struct BusyScope
        {
            bool previous = t_busy;
            BusyScope()  { t_busy = true; }
            ~BusyScope() { t_busy = previous; }
        } busy;

        std::vector<Record> live;
        {
            std::lock_guard<std::mutex> lock(g_tracker.mutex);
            live.reserve(g_tracker.records.size());
            for (const auto& entry : g_tracker.records) live.push_back(entry.second);
        }

        if (live.empty())
        {
            Print("\n[LeakDetector] No memory leaks detected.\n");
            std::fflush(stdout);
            return;
        }

        // Group leaks that share the same call stack.
        struct Group { const Record* sample; std::size_t count; std::size_t bytes; };
        std::unordered_map<std::uint64_t, Group> byStack;
        std::size_t totalBytes = 0;

        for (const Record& r : live)
        {
            std::uint64_t hash = 14695981039346656037ull;  // FNV-1a over the return addresses
            for (unsigned i = 0; i < r.depth; ++i)
                hash = (hash ^ reinterpret_cast<std::uintptr_t>(r.frames[i])) * 1099511628211ull;

            auto it = byStack.find(hash);
            if (it == byStack.end()) it = byStack.emplace(hash, Group{ &r, 0, 0 }).first;
            ++it->second.count;
            it->second.bytes += r.size;
            totalBytes += r.size;
        }

        std::vector<Group> groups;
        groups.reserve(byStack.size());
        for (const auto& entry : byStack) groups.push_back(entry.second);
        std::sort(groups.begin(), groups.end(),
                  [](const Group& a, const Group& b) { return a.bytes > b.bytes; });

        Print("\n========== LeakDetector: %zu leaked allocation(s), %zu byte(s), %zu location(s) ==========\n",
              live.size(), totalBytes, groups.size());

        HANDLE process = GetCurrentProcess();
        SymSetOptions(SYMOPT_LOAD_LINES | SYMOPT_UNDNAME | SYMOPT_DEFERRED_LOADS);
        const bool haveSymbols = SymInitialize(process, nullptr, TRUE) != FALSE;

        unsigned index = 1;
        for (const Group& g : groups)
        {
            Print("\n[%u] %zu allocation(s), %zu byte(s)\n", index++, g.count, g.bytes);
            const unsigned limit = std::min(g.sample->depth, kFramesPrinted);
            for (unsigned i = 0; i < limit; ++i)
                if (PrintFrame(process, haveSymbols, g.sample->frames[i])) break;
        }

        if (haveSymbols) SymCleanup(process);
        Print("\n");
        std::fflush(stdout);
    }

    // ---- allocation core --------------------------------------------------------------------

    __declspec(noinline) void* Allocate(std::size_t size, std::size_t alignment, bool nothrow)
    {
        if (size == 0) size = 1;

        for (;;)
        {
            void* p = alignment ? _aligned_malloc(size, alignment) : std::malloc(size);
            if (p)
            {
                if (g_active.load(std::memory_order_relaxed) && !t_busy)
                {
                    Record record;
                    record.size  = size;
                    // skip 2 frames: Allocate() and the operator new that called it
                    record.depth = CaptureStackBackTrace(2, kMaxFrames, record.frames, nullptr);

                    std::lock_guard<std::mutex> lock(g_tracker.mutex);
                    g_tracker.records[p] = record;
                }
                return p;
            }

            if (std::new_handler handler = std::get_new_handler()) handler();
            else if (nothrow) return nullptr;
            else throw std::bad_alloc();
        }
    }

    void Release(void* p, bool aligned) noexcept
    {
        if (!p) return;

        // Untrack before freeing, otherwise another thread could be handed this address
        // and register it before we erase the stale entry.
        if (g_active.load(std::memory_order_relaxed) && !t_busy)
        {
            std::lock_guard<std::mutex> lock(g_tracker.mutex);
            g_tracker.records.erase(p);
        }

        if (aligned) _aligned_free(p);
        else         std::free(p);
    }
}  // namespace

// ---- public API -------------------------------------------------------------------------------

void LeakDetector::Start()  { g_active.store(true); }
void LeakDetector::Report() { if (g_active.load()) ReportLeaks(); }

// ---- global operator new / delete replacements ---------------------------------------------

void* operator new(std::size_t size)                                    { return Allocate(size, 0, false); }
void* operator new[](std::size_t size)                                  { return Allocate(size, 0, false); }
void* operator new(std::size_t size, const std::nothrow_t&) noexcept    { return Allocate(size, 0, true); }
void* operator new[](std::size_t size, const std::nothrow_t&) noexcept  { return Allocate(size, 0, true); }

void operator delete(void* p) noexcept                                  { Release(p, false); }
void operator delete[](void* p) noexcept                                { Release(p, false); }
void operator delete(void* p, std::size_t) noexcept                     { Release(p, false); }
void operator delete[](void* p, std::size_t) noexcept                   { Release(p, false); }
void operator delete(void* p, const std::nothrow_t&) noexcept          { Release(p, false); }
void operator delete[](void* p, const std::nothrow_t&) noexcept        { Release(p, false); }

#ifdef __cpp_aligned_new  // over-aligned types, C++17 (/std:c++17 or later)
void* operator new(std::size_t size, std::align_val_t al)
    { return Allocate(size, static_cast<std::size_t>(al), false); }
void* operator new[](std::size_t size, std::align_val_t al)
    { return Allocate(size, static_cast<std::size_t>(al), false); }
void* operator new(std::size_t size, std::align_val_t al, const std::nothrow_t&) noexcept
    { return Allocate(size, static_cast<std::size_t>(al), true); }
void* operator new[](std::size_t size, std::align_val_t al, const std::nothrow_t&) noexcept
    { return Allocate(size, static_cast<std::size_t>(al), true); }

void operator delete(void* p, std::align_val_t) noexcept                { Release(p, true); }
void operator delete[](void* p, std::align_val_t) noexcept              { Release(p, true); }
void operator delete(void* p, std::size_t, std::align_val_t) noexcept   { Release(p, true); }
void operator delete[](void* p, std::size_t, std::align_val_t) noexcept { Release(p, true); }
void operator delete(void* p, std::align_val_t, const std::nothrow_t&) noexcept   { Release(p, true); }
void operator delete[](void* p, std::align_val_t, const std::nothrow_t&) noexcept { Release(p, true); }
#endif

#endif  // LEAK_DETECTOR_ENABLED
