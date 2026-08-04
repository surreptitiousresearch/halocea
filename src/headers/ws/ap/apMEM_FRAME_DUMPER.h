#pragma once
// ws-engine ap: per-frame memory-usage logger. Records a named allocation window and
// the ap/dl byte totals it saw, and formats log lines through apForceLogV.
// DB-verified layout (types_members apMEM_FRAME_DUMPER):
//   isStarted@0, type@4, size@8, nameLog@12 (char[260]), sizeAp@272, sizeDl@280,
//   dlStat@288 (dsCONST_ARRAY<apMEM_DL_STAT,32>) — size 548.
#include "apMEM_BLOCK_TYPE.h"
#include "apMEM_FRMDMP_SIZE.h"
#include "apMEM_DL_STAT.h"
#include "../ds/dsCONST_ARRAY.h"

// Variadic log sink (Saber logging boundary): returns an int, walks argPtr as a va_list.
extern "C" int apForceLogV(const char *name, char *format, char *argPtr);

typedef struct apMEM_FRAME_DUMPER {
    bool                             isStarted; // 0x00
    unsigned char _pad0[3]; /* db-verified padding */
    apMEM_BLOCK_TYPE                 type;      // 0x04 bucket being watched
    int                              size;      // 0x08 window byte size
    char                             nameLog[260]; // 0x0C log label
    apMEM_FRMDMP_SIZE                sizeAp;    // 0x110 ap-allocator totals
    apMEM_FRMDMP_SIZE                sizeDl;    // 0x118 dlmalloc totals
    dsCONST_ARRAY<apMEM_DL_STAT, 32> dlStat;    // 0x120 per-type dl stats snapshot

    // Format and emit one log line (private variadic helper).
    void Dump(const char *name, char *format, ...);

    // 0x82C4C0C8 — record a dlmalloc allocation of `size` bytes: bump the running/peak dl total
    // and append an AP_DLSTAT_DL_ALLOC entry to the snapshot (capped at 32 entries).
    void AllocDl(int size);

    // 0x82C4C148 — record a dlmalloc free of `size` bytes: reduce the running dl total and append
    // an AP_DLSTAT_DL_FREE entry to the snapshot (capped at 32 entries).
    void FreeDl(int size);

    // apDlStatFrameNotifyAlloc/Free (ap_dlstat.cpp) are the module-private free-function
    // trampolines that call the private NotifyAp below on the global `_frameDumper` instance.
    friend void apDlStatFrameNotifyAlloc(int size, const char *filename, int line, apMEM_BLOCK_TYPE type);
    friend void apDlStatFrameNotifyFree(int size, const char *filename, int line, apMEM_BLOCK_TYPE type);

private:
    // 0x82C4BF30 — notify of an ap-allocator alloc/free of `memSize` bytes: if it falls within the
    // watched bucket and size window, fold it into the ap running/peak totals and emit a log line;
    // otherwise flush the matching dl snapshot entries via DumpDlInfo. Private (AAA mangle).
    void NotifyAp(int memSize, const char *filename, int fileLine,
                  apMEM_BLOCK_TYPE _type, apMEM_DL_ALLOC_TYPE allocType);

    // 0x82C4BD50 — drain every snapshot entry whose alloc category equals `type`: log each one
    // (against the current dl running total) and compact it out of the fixed array. Private (AAA).
    void DumpDlInfo(apMEM_DL_ALLOC_TYPE type);
} apMEM_FRAME_DUMPER;
