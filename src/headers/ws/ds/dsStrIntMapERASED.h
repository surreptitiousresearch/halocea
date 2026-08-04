#pragma once
/* External/SDK extern boundary — partial by design, see .complete/ESCALATIONS.md */
// ds deferred-erase record:
//   dsERASE_MANAGER<dsLF_SORTED_MAP<const char*,int,dsSTR_HASH,dsSTR_CMP>::LINE_ERASER>::ERASED
// DB-verified (types_members): threadMask@0, eraser@4 (LINE_ERASER{line,index}) — size 12.

// Opaque sorted-map LINE node for the int-valued string map.
struct dsStrIntMapLINE; // boundary

typedef struct dsStrIntMapLINE_ERASER {
    dsStrIntMapLINE *line;  // 0x00
    int              index; // 0x04
} dsStrIntMapLINE_ERASER;

typedef struct dsStrIntMapERASED {
    unsigned int           threadMask; // 0x00
    dsStrIntMapLINE_ERASER eraser;     // 0x04
} dsStrIntMapERASED;
