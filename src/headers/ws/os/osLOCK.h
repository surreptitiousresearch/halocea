#pragma once
// ws-engine os: recursive critical-section lock.
// DB-verified layout (types_members osLOCK) — size 52.

typedef struct osLOCK {
    unsigned char       data[32];    // 0x00 platform critical-section storage
    bool                isInited;    // 0x20
    char                threadNmb;   // 0x21 owning thread slot (-1 = none)
    unsigned char       _pad22[2];   // 0x22
    volatile int        lockDepth;   // 0x24 recursion count
    const char         *name;        // 0x28 debug name
    const char         *locker_file; // 0x2C source file of current holder
    int                 locker_line; // 0x30 source line of current holder

    // 0x823D092C — zero isInited/threadNmb(-1)/locker_file/locker_line(-1), adopt `name`
    // (falling back to a fixed debug label when null), then finish platform init via Init().
    osLOCK(const char *name);
    osLOCK() {}

    // Finish initialising the platform primitive (lazily; also inlined into Lock()).
    void Init();

    // Tear down the platform primitive. NOTE: this build only clears isInited -- it does NOT
    // call RtlDeleteCriticalSection, so the underlying critical section object is leaked/left
    // live. Reconstructed verbatim from disasm (2 instructions): DB is ground truth.
    void Term();

    // Acquire / release the lock, recording the caller's source location.
    void Lock(const char *file, int line);
    void Unlock(const char *file, int line);

    // Bind the underlying platform critical section / owning thread to a specific hardware
    // processor. boundary (platform primitive; used by vidDRIVER::BindToProcessor).
    void Bind(int threadProcessorNmb);
} osLOCK;
