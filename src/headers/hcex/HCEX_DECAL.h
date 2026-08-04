#pragma once
/* HCEX_DECAL — one entry in the hcexCreateDecals vector: a decal to be spawned by the ws-engine at a
 * bridged world position, plus its (converted) tag name. Layout from the database (types_members):
 *   pos  @ 0x00 (m3dV — a plain 3-float vector)
 *   name @ 0x0C (dsTSTRING<char> — ref-counted string) */

#include "../ws/m3d/m3dV.h"
#include "../ws/ds/dsTSTRING.h"

typedef struct HCEX_DECAL
{
    m3dV            pos;   /* 0x00 */
    dsTSTRING<char> name;  /* 0x0C */

    /* 0x823D6934 — default-construct: name adopts the shared empty-string singleton (pos left
     * uninitialized, as observed in the disassembly). */
    HCEX_DECAL();
    /* 0x823D3EF0 — copy-construct: copy pos, share that.name's buffer (ref-bump). */
    HCEX_DECAL(const HCEX_DECAL &that);
    /* 0x823D3158 — release name's buffer reference, freeing it at zero. */
    ~HCEX_DECAL();
    /* 0x823D3E98 (`??_G` mangle) — scalar deleting destructor: run ~HCEX_DECAL(), then
     * conditionally operator-delete `this` when bit 0 of `deleteFlags` is set. */
    HCEX_DECAL *ScalarDeletingDtor(unsigned char deleteFlags);
} HCEX_DECAL;          /* 16 bytes */
