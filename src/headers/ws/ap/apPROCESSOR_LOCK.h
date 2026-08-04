#pragma once
// ws-engine ap: processor-affinity debug guard. Records the processor a lock was taken
// on and (in debug builds) asserts every re-entry happens on the same processor.
// DB-verified layout (types_members apPROCESSOR_LOCK): processor_@0 (int) — size 4.

typedef struct apPROCESSOR_LOCK {
    int processor_; // 0x00 hardware processor id the lock is bound to

    // Assert the caller is running on processor_; a no-op in this release build.
    void CheckCurrentProcessor(const char *file, int line);
} apPROCESSOR_LOCK;
