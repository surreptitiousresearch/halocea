#pragma once
#include "../ap/apSTATE_T.h"
// ws-engine jbm: abstract base for a unit of work queued onto the job manager.
// DB-verified layout (types_members jbmJOB): __vftable@0, name@4 (const char*),
// state@8 (apSTATE_T<unsigned long>) — size 12.
// DB-verified vtable layout (types_members jbmJOB_vtbl): dtr_jbmJOB@0, Execute@4,
// PreExecute@8, PostExecute@12, OnAdd@16 (all `void (jbmJOB *, int)` except the dtor).

struct jbmJOB;

typedef struct jbmJOB_vtbl {
    void (__fastcall *dtr_jbmJOB)(jbmJOB *thisPtr);          // 0x00 destructor
    void (__fastcall *Execute)(jbmJOB *thisPtr, int procID);     // 0x04
    void (__fastcall *PreExecute)(jbmJOB *thisPtr, int procID);  // 0x08
    void (__fastcall *PostExecute)(jbmJOB *thisPtr, int procID); // 0x0C
    void (__fastcall *OnAdd)(jbmJOB *thisPtr, int procID);       // 0x10
} jbmJOB_vtbl;

typedef struct jbmJOB {
    static jbmJOB_vtbl vftable; // `jbmJOB::`vftable'' @ 0x82... -- boundary, not decompiled. Referenced
                                // by subclass destructors (e.g. hcexJOB_SYNC_INST::~hcexJOB_SYNC_INST)
                                // that re-point __vftable back to the base on teardown.

    jbmJOB_vtbl              *__vftable; // 0x00
    const char                *name;     // 0x04 debug name
    apSTATE_T<unsigned long>   state;    // 0x08 current queued/running state

    // Run this job's work body on worker `procID`. boundary (virtual) — not decompiled here,
    // implementations live in concrete jbmJOB subclasses.
    void Execute(int procID);     // 0x04 vtbl slot — boundary (virtual)
    // Called immediately before Execute() runs. boundary (virtual)
    void PreExecute(int procID);  // 0x08 vtbl slot — boundary (virtual)
    // Called immediately after Execute() completes. boundary (virtual)
    void PostExecute(int procID); // 0x0C vtbl slot — boundary (virtual)
    // Called synchronously when the job is handed to jbmTHREAD::AddJob (from the enqueuing
    // thread, before it is either run inline or pushed onto the target thread's deque).
    // boundary (virtual)
    void OnAdd(int procID);       // 0x10 vtbl slot — boundary (virtual)
} jbmJOB;
