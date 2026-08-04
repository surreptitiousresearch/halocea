#pragma once
/* Reconstruction (no DB/PDB type) — adjudicated KEEP, see .complete/ESCALATIONS.md */
/* async_read_request — a cached-map async read request block: a standard OVERLAPPED immediately followed by
 * a pointer to a completion flag (and two unused ints) that the blocking completion routine sets. */

#include "_OVERLAPPED.h" /* repo Win32 shim; system <windows.h> collides with the canonical globals header's Win32 type shims */

typedef struct async_read_request
{
    _OVERLAPPED    overlapped;
    volatile char *completed_flag;
    int            unused1;
    int            unused2;
} async_read_request;
