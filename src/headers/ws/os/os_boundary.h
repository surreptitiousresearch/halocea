#pragma once
// Boundary declarations for the Xbox 360 kernel critical-section primitives used by osLOCK,
// plus the Win32/xboxkrnl/CRT surface the os* free-function wrappers in this folder call
// straight through to. Not reversed here -- these are xboxkrnl.exe (NT-derived Rtl*) exports
// or CRT/kernel32-style Win32 APIs. osLOCK::data[32] is treated as an opaque
// RTL_CRITICAL_SECTION-sized blob and passed by address, matching the disasm's raw cast of
// `this` to PRTL_CRITICAL_SECTION.

#include "../../_LARGE_INTEGER.h"

extern "C" {
    void         RtlInitializeCriticalSection(void *criticalSection); // boundary -- xboxkrnl
    void         RtlEnterCriticalSection(void *criticalSection);      // boundary -- xboxkrnl
    void         RtlLeaveCriticalSection(void *criticalSection);      // boundary -- xboxkrnl
    unsigned int GetCurrentProcessorNumber(void);                     // boundary -- xboxkrnl

    // -- osGetTime / osSleep / osDebugBreak / thread id --
    unsigned int GetTickCount(void);                                  // boundary -- kernel32
    void         Sleep(unsigned int milliseconds);                    // boundary -- kernel32
    void         DebugBreak(void);                                    // boundary -- kernel32
    unsigned int GetCurrentThreadId(void);                            // boundary -- kernel32
    void         OutputDebugStringA(const char *str);                 // boundary -- kernel32

    // -- signal (event) primitives: osHANDLE_DUMMY* implicitly converts to void* --
    int  SetEvent(void *handle);                                      // boundary -- kernel32
    int  ResetEvent(void *handle);                                    // boundary -- kernel32
    int  CloseHandle(void *handle);                                   // boundary -- kernel32
    unsigned int WaitForSingleObject(void *handle, unsigned int msTimeout); // boundary -- kernel32

    // -- file I/O: osFILE_HANDLE_DUMMY* implicitly converts to void* --
    void        *CreateFileA(const char *name, unsigned int access, unsigned int share,
                              void *security, unsigned int disposition,
                              unsigned int flags, void *templateFile);       // boundary -- kernel32
    int          WriteFile(void *handle, const void *buffer, unsigned int size,
                            unsigned int *written, void *overlapped);        // boundary -- kernel32
    unsigned int GetFileSize(void *handle, unsigned int *highPart);          // boundary -- kernel32
    unsigned int SetFilePointer(void *handle, int distance, void *highPtr,
                                 unsigned int method);                       // boundary -- kernel32
    int          SetFileAttributesA(const char *name, unsigned int attrs);   // boundary -- kernel32
    int          CreateDirectoryA(const char *name, void *security);        // boundary -- kernel32
    unsigned int GetLastError(void);                                        // boundary -- kernel32
    int          access(const char *path, int mode);                       // boundary -- CRT (posix-style existence/access check)

    // -- high-resolution timing --
    int QueryPerformanceCounter(_LARGE_INTEGER *result);                    // boundary -- kernel32
    int QueryPerformanceFrequency(_LARGE_INTEGER *result);                  // boundary -- kernel32

    // -- PIX event markers (D3D/PIX subsystem -- not decompiled here) --
    void PIXBeginNamedEvent_Copy_NoVarArgs(int color, const char *name);     // boundary -- PIX
    void PIXEndNamedEvent(void);                                            // boundary -- PIX

    // -- heap free (dlmalloc-family allocator used throughout the engine) --
    void dlFree(void *block);                                              // boundary -- allocator

    // -- compiler/CPU intrinsic: PowerPC lightweight sync (osMemoryBarrier) --
    void __lwsync(void);                                                   // boundary -- PPC intrinsic

    // -- debugger thread-naming idiom (VC++/Xbox 360 THREADNAME_INFO exception trick) --
    void RaiseException(unsigned int code, unsigned int flags,
                         unsigned int argCount, const unsigned int *args);  // boundary -- kernel32
}

// Saber os-layer module-scope globals used by the PIX event wrappers and thread-id queries.
extern int osPixEventDepth; // real symbol `_osPixEventDepth` (PDB leading-underscore convention)
extern int MAIN_THREAD_ID;  // real symbol `?MAIN_THREAD_ID@@3HA` == `int MAIN_THREAD_ID`

// NOTE: osGetCurThreadId, osLockedIncrement, osLockedDecrement, osGetPerfCounter, and
// osGetPerfFrequancy previously had boundary-extern stand-ins here (or relied on inline
// stand-ins in sibling .cpp files); they now have real bodies in src/ws/os/<name>.cpp and are
// declared by including this header (GetCurrentThreadId/QueryPerformanceCounter/
// QueryPerformanceFrequency above), so the old `int osGetCurThreadId();` stand-in is removed
// to avoid a duplicate/conflicting declaration. osLockedIncrement/osLockedDecrement have their
// own inline extern declarations in entENTITY__Register.cpp / txm/txmTEXTURE__Release.cpp
// respectively (untouched per instructions) which are compatible with the real definitions.
