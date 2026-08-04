#pragma once
/* Reconstruction (no DB/PDB type) — adjudicated KEEP, see .complete/ESCALATIONS.md */
/* cache_copy_worker — background cache-copy worker state. Only the two Win32 event handles used to
 * stop/await the copy are modeled; the remainder is opaque. */

typedef struct cache_copy_worker
{
    void *copy_complete_event; /* 0x0 */
    void *copy_stop_event;     /* 0x4 */
    /* + opaque */
} cache_copy_worker;
