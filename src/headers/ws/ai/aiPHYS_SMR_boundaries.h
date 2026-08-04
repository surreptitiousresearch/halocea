#pragma once
#include "../m3d/m3dOBB.h"
// ws-engine phys boundary (partial) — just the slice of physRB_BASE that aiPHYS_SMR::ProcessRbStartRest
// reads: the owning anim instance (userData.pInst) and GetPhysOBB. physRB_BASE is only ever used
// through a pointer here, so the tail past userData is intentionally not modelled. Offsets are
// DB-verified (types_members physRB_BASE / physRB_USER_DATA); the leading fields are byte-accurate
// padding placeholders, NOT an authoritative field-by-field layout.

struct animINST; // anim instance — boundary (fwd)

// DB (types_members physRB_USER_DATA): __vftable@0, ..., pInst@0x5C — size 128.
struct physRB_USER_DATA {
    unsigned char _pad0[0x5C]; // 0x00 vtable + user fields (not modelled)
    animINST     *pInst;       // 0x5C owning anim instance
    unsigned char _pad1[0x80 - 0x60]; // pad to full 128-byte size
};

// DB (types_members physRB_BASE): userData@0xB4, previousObb@0x17C — the full class continues past
// this point; only the head through previousObb is needed by this boundary (pointer-only usage).
struct physRB_BASE {
    unsigned char    _pad0[0xB4];      // 0x00 vtable + rigid-body state (not modelled)
    physRB_USER_DATA userData;         // 0xB4 owner/instance data (128B, ends 0x134)
    unsigned char    _pad1[0x17C - 0x134]; // 0x134 constrType/staticObb/... (not modelled)
    m3dOBB           previousObb;      // 0x17C body's previous-frame world OBB

    // ?GetPhysOBB@physRB_BASE@@QAAXAAVm3dOBB@@@Z — current world-space oriented bounding box.
    void GetPhysOBB(m3dOBB &out);
};
