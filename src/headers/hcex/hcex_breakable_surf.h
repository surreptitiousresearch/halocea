#pragma once
/* hcexBREAKABLE_SURF and the ws-engine create-data boundary types it embeds.
 *
 * DEVIATION (fix): this header used to carry its own local, duplicate typedefs of
 * scnINST_CREATE_DATA/animCREATE_DATA/dsTSTRING/dsAFFIX_STRING/psSECTION/m3dMATR. Those types
 * already have canonical, DB-verified headers elsewhere in the corpus
 * (src/headers/ws/scn/scnINST_CREATE_DATA.h, src/headers/ws/anim/animCREATE_DATA.h,
 * src/headers/ws/ds/dsTSTRING.h, src/headers/ws/ds/dsAFFIX_STRING.h,
 * src/headers/ws/ps/psSECTION.h, src/headers/ws/m3d/m3dMATR.h) — a duplicate definition of the
 * same struct in two headers is a hard compile error the moment both are included together.
 * Switched to including the canonical headers and reusing their types verbatim; sizes match
 * (scnINST_CREATE_DATA: 88 bytes per types_members, confirmed identical to the old local copy). */

#include "../animINST.h"
#include "../ws/scn/scnINST_CREATE_DATA.h"

/* HCEX breakable-surface slot: a bsp/index key, a live scene instance, and the create-data to
 * rebuild it. Layout corrected to DB ground truth (types_members hcexBREAKABLE_SURF): the record
 * is keyed on (bsp, idx) at the front — earlier revision of this header omitted them and placed
 * pInst at 0x00, which was wrong (size 92 vs the DB's 100). dsVECTOR<hcexBREAKABLE_SURF,8>::
 * FindSorted binary-searches on this (bsp, idx) key. */
typedef struct hcexBREAKABLE_SURF
{
    int                 bsp;    /* 0x00 */
    int                 idx;    /* 0x04 */
    animINST           *pInst;  /* 0x08 */
    scnINST_CREATE_DATA cd;     /* 0x0C */

    // 0x823B16F8 — default-construct: default-construct the embedded scnINST_CREATE_DATA (bsp/idx/
    // pInst are left uninitialized by the ctor itself, as observed in the disassembly).
    hcexBREAKABLE_SURF();
    // 0x823B1EBC — copy-construct: copy bsp/idx/pInst, then copy-construct cd from that.cd.
    hcexBREAKABLE_SURF(const hcexBREAKABLE_SURF &that);
    // 0x823B1144 — destroy the embedded scnINST_CREATE_DATA.
    ~hcexBREAKABLE_SURF();
    // 0x823B137C — copy-assign bsp/idx/pInst, then copy-assign cd from that.cd.
    hcexBREAKABLE_SURF &operator=(const hcexBREAKABLE_SURF &that);
    // 0x823B0900 — ascending order on the (bsp, idx) key: bsp first, then idx on a bsp tie
    // (dsVECTOR<hcexBREAKABLE_SURF,8>::FindSorted/InsertSorted's default predicate).
    bool operator<(const hcexBREAKABLE_SURF &other) const;
    // 0x823B1320 (`??_G` mangle) — scalar deleting destructor: run ~hcexBREAKABLE_SURF(), then
    // conditionally operator-delete `this` when bit 0 of deleteFlags is set.
    hcexBREAKABLE_SURF *ScalarDeletingDtor(unsigned char deleteFlags);
} hcexBREAKABLE_SURF;
