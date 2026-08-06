#pragma once
/* DB-verified: anonymous PDB struct $3ECBAFD5DDBB19ABB6BFAB57EA031D46 (types.size = 4), both
 * members and offsets from types_members. (Previously annotated "Reconstruction (no DB/PDB type)
 * ... only the fields touched by reconstructed callers are modeled" — both claims are false: the
 * type is keyed by the address of hs_runtime_globals @ 0x8441E650 via applied_types rather than
 * by name, and these two members account for all 4 bytes, so the layout is complete.) */
/* hs_runtime_globals — HaloScript runtime state (hs_runtime.c). */

#include <stdint.h>

typedef struct hs_runtime_globals_t
{
    uint8_t initialized;             /* 0x0 */
    uint8_t _pad1[1];                /* 0x1 */
    int16_t executing_thread_index;  /* 0x2 */
} hs_runtime_globals_t;              /* 4 bytes */

#ifdef __cplusplus
extern "C" {
#endif

extern hs_runtime_globals_t hs_runtime_globals;

#ifdef __cplusplus
}
#endif
