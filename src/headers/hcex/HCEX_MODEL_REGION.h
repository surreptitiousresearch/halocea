#pragma once
#include "../ws/ds/dsVECTOR.h"
#include "../ws/ds/dsTSTRING.h"
// HCEX bridge: a named region of a Halo model — a set of object sub-ids/names.
// DB-verified layout (types_members HCEX_MODEL_REGION):
//   objRegId@0, objIds@4, objNames@24, name@44 — size 48.

typedef struct HCEX_MODEL_REGION {
    short                        objRegId;  // 0x00 region id
    unsigned char _pad0[2]; /* db-verified padding */
    dsVECTOR<short, 8>           objIds;    // 0x04 member object sub-ids
    dsVECTOR<dsTSTRING<char>, 8> objNames;  // 0x18 member object names
    dsTSTRING<char>              name;      // 0x2C region name

    // 0x823D6800 — default-construct: objIds/objNames adopt the ds_vector.h:24 call-site cookie
    // (empty vectors), name adopts the shared empty-string singleton (objRegId left uninitialized).
    HCEX_MODEL_REGION();
    // 0x823E42AC (0x823E4290 in an earlier revision of this comment) — copy-construct: copy
    // objRegId, deep-copy objIds/objNames via Insert (adopting each source vector's call-site
    // cookie), then share that.name's buffer (ref-bump).
    HCEX_MODEL_REGION(const HCEX_MODEL_REGION &that);
    // 0x823D4AFC — release name's buffer reference, destroy objNames, free objIds' backing store.
    ~HCEX_MODEL_REGION();
    // 0x823D5FE8 (`??_G` mangle) — scalar deleting destructor: run ~HCEX_MODEL_REGION(), then
    // conditionally operator-delete `this` when bit 0 of `deleteFlags` is set.
    HCEX_MODEL_REGION *ScalarDeletingDtor(unsigned char deleteFlags);
} HCEX_MODEL_REGION;
