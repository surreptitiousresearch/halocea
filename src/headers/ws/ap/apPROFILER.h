#pragma once
#include "../ds/dsTSTRING.h"
#include "apCOUNTER_UNIT.h"
// ws-engine ap: a named live profiler binding, pairing a display name/id with the
// apCOUNTER_UNIT currently reporting through it. Boundary — methods not decompiled here.
// DB-verified layout (types_members apPROFILER): __vftable@0, name@4, id@8, currentUnit@12
// — size 16.

struct apPROFILER;
struct apPROFILER_DESC; // boundary — not decompiled here

// DB-verified layout (types_members apPROFILER_vtbl): dtr@0, Start@4, Stop@8, StartFrame@12,
// EndFrame@16, SaveProfileResult@20, SetDesc@24 — all boundary (bodies not decompiled here).
typedef struct apPROFILER_vtbl {
    void (*dtr_apPROFILER)(apPROFILER *thisPtr, int deleteFlag); // deleting dtor: vftable+0x00 holds ??_GapPROFILER@@UAAPAXI@Z
    void (*Start)(apPROFILER *thisPtr);
    void (*Stop)(apPROFILER *thisPtr);
    void (*StartFrame)(apPROFILER *thisPtr);
    void (*EndFrame)(apPROFILER *thisPtr);
    void (*SaveProfileResult)(apPROFILER *thisPtr, const char *path);
    void (*SetDesc)(apPROFILER *thisPtr, const apPROFILER_DESC *desc);
} apPROFILER_vtbl;

typedef struct apPROFILER {
    apPROFILER_vtbl  *__vftable;   // 0x00
    dsTSTRING<char>   name;        // 0x04
    int               id;          // 0x08
    apCOUNTER_UNIT   *currentUnit; // 0x0C unit currently bound to this profiler

    // vtbl slot 0x04 — boundary (virtual), begin a profiling sample.
    void Start();
    // vtbl slot 0x08 — boundary (virtual), end a profiling sample.
    void Stop();
} apPROFILER;
