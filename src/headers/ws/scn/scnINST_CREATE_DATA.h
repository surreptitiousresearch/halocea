#pragma once
#include "../ds/dsTSTRING.h"
#include "../m3d/m3dMATR.h"
#include "../ds/dsAFFIX_STRING.h"
#include "../ps/psSECTION.h"

// ws-engine scn: instance-creation record consumed by the scene-update spawn path.
// DB-verified layout (types_members scnINST_CREATE_DATA): state@0(4), nameInst@4, nameTpl@8,
// nameClass@12 (dsTSTRING<char>, 4 each), matrInst@16(64, m3dMATR), affixes@80(4, dsAFFIX_STRING),
// ps@84(4, psSECTION) — total size 88.
// Canonical home, and as of 2026-08-07 the SOLE definition: hcex/hcex_kill_breakable_surface_boundary.h
// carried a member-for-member identical flat copy and now reaches this one through
// hcex_surf_vector.h -> hcex_breakable_surf.h (odr_dup drain).
typedef struct scnINST_CREATE_DATA {
    int state;                    // 0x00
    dsTSTRING<char> nameInst;     // 0x04
    dsTSTRING<char> nameTpl;      // 0x08
    dsTSTRING<char> nameClass;    // 0x0C
    m3dMATR matrInst;             // 0x10
    dsAFFIX_STRING affixes;       // 0x50
    psSECTION ps;                 // 0x54

    // 0x823B16F8 (also found at 0x82F51598 in this session's disassembly) — default-construct:
    // state=0, nameInst/nameTpl/nameClass each adopt the process-wide shared empty-string
    // singleton (dsTSTRING<char>::UnsafeInitEmpty, ref-bumped), then default-construct
    // affixes/ps.
    scnINST_CREATE_DATA();

    // Copy-constructs each dsTSTRING (ref-bumps its buffer) and the affixes/ps members.
    // Body external to this batch. // boundary
    scnINST_CREATE_DATA(const scnINST_CREATE_DATA &other);

    // Drops each dsTSTRING's buffer reference and tears down affixes/ps.
    // Body external to this batch. // boundary
    ~scnINST_CREATE_DATA();

    // 0x823B1390 (called from hcexBREAKABLE_SURF::operator=) — copy-assign every field. Body not
    // reversed in this batch (only the calling convention is confirmed from that call site). // boundary
    scnINST_CREATE_DATA &operator=(const scnINST_CREATE_DATA &other);
} scnINST_CREATE_DATA;
