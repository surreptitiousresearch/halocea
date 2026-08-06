#pragma once
// ws-engine ap: abstract base for memory-report dumpers. Dump() re-entrancy-guards on
// the global isDumpInProgress flag, calls the virtual coreDump to gather the report,
// then the virtual Write to emit it.
// DB-verified layout (types_members apMEM_DUMPER_BASE): __vftable@0, dumpToDbgOut@4 — size 8.

struct apMEM_DUMPER_BASE;

// vtable — DB-verified (types_members apMEM_DUMPER_BASE_vtbl).
typedef struct apMEM_DUMPER_BASE_vtbl {
    void (*dtr_apMEM_DUMPER_BASE)(apMEM_DUMPER_BASE *self);
    void (*Write)(apMEM_DUMPER_BASE *self, const char *fileName, char *text, ...);
    void (*coreDump)(apMEM_DUMPER_BASE *self, const char *fileName, unsigned int typesMask);
} apMEM_DUMPER_BASE_vtbl;

// Re-entrancy guard shared by every dumper — true while a dump is running.
extern "C" bool isDumpInProgress;

typedef struct apMEM_DUMPER_BASE {
    apMEM_DUMPER_BASE_vtbl *__vftable; // 0x00
    bool                    dumpToDbgOut; // 0x04 also echo the report to the debug console

    // Convenience shim: coreDump then Write, guarded against re-entrancy.
    void Dump(const char *fileName, unsigned int typesMask);
} apMEM_DUMPER_BASE;
