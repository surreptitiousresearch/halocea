#pragma once
/* External/SDK extern boundary — partial by design, see .complete/ESCALATIONS.md */
// ds deferred-erase record:
//   dsERASE_MANAGER<dsLF_SORTED_MAP<const char*,dsSTRID,dsSTR_HASH,dsSTR_CMP>::LINE_ERASER>::ERASED
// DB-verified (types_members): threadMask@0, eraser@4 (LINE_ERASER{line,index}) — size 12.

// Opaque sorted-map LINE node for the dsSTRID-valued string map.
struct dsStrStridMapLINE; // boundary

typedef struct dsStrStridMapLINE_ERASER {
    dsStrStridMapLINE *line;  // 0x00
    int                index; // 0x04
} dsStrStridMapLINE_ERASER;

typedef struct dsStrStridMapERASED {
    unsigned int             threadMask; // 0x00
    dsStrStridMapLINE_ERASER eraser;     // 0x04
} dsStrStridMapERASED;
