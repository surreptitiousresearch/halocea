#pragma once
/* Reconstruction (no DB/PDB type) — adjudicated KEEP, see .complete/ESCALATIONS.md */
/* hs_runtime_globals — HaloScript runtime state (hs_runtime.c). Only the fields touched by reconstructed
 * callers are modeled. */

typedef struct hs_runtime_globals_t
{
    unsigned char initialized;            /* 0x0 */
    short         executing_thread_index; /* 0x2 */
} hs_runtime_globals_t;

extern hs_runtime_globals_t hs_runtime_globals;
