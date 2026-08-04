#pragma once
/* HCEX_LIGHT / hcex_light_pair — a Blam flashlight light's ws-engine binding. ent is at +4 (after id),
 * verified against disasm (dsPAIR read at +8 == HCEX_LIGHT.ent.pHandle). */

#include "../ds_weak_ptr.h"

typedef struct entSFX_LIGHT entSFX_LIGHT;

typedef struct HCEX_LIGHT
{
    int           id;       /* 0x0 */
    ds::WEAK_PTR<entENTITY> ent; /* 0x4 — DB WEAK_PTR<entENTITY> */
    entSFX_LIGHT *entLight; /* 0x8 */

    /* 0x823D69AC — default-construct: null the weak-pointer handle (id/entLight left
     * uninitialized, as observed in the disassembly). */
    HCEX_LIGHT();
    /* 0x823D6120 — copy-construct: copy id, share that.ent's handle (ref-bump if non-null), copy
     * entLight. */
    HCEX_LIGHT(const HCEX_LIGHT &that);
    /* 0x823D69CC — drop the weak-pointer handle's reference, operator-deleting it at zero, then
     * null the handle. */
    ~HCEX_LIGHT();
    /* 0x823D60B0 — copy-assign: copy id; if `ent` isn't already shared with `that.ent`, release
     * this->ent's handle and adopt that.ent's handle (ref-bump if non-null); copy entLight. */
    HCEX_LIGHT &operator=(const HCEX_LIGHT &that);
} HCEX_LIGHT;

/* boundary: ds::WEAK_PTR<entENTITY>::Release — drop this weak-pointer's handle reference,
 * operator-deleting the handle at zero (does not null out ent.pHandle itself; the caller
 * overwrites it right after, per the HCEX_LIGHT::operator= disassembly). Not reversed in this
 * batch (ws-engine C++ boundary, private/AAA mangle). */
extern void ds_WEAK_PTR_Release(ds_WEAK_PTR *weak);

#include "../ws/ds/dsPAIR.h"
typedef dsPAIR<int, HCEX_LIGHT> hcex_light_pair; /* DB dsPAIR<int,HCEX_LIGHT> */
