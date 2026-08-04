#pragma once
#include "../ds/dsTSTRING.h"
#include "../ds/dsVECTOR.h"
#include "../ssl/sslOBJ_REF.h"
#include "../anim/animINST.h"
#include "instCONSTRUCTOR_NODE.h"
// ws-engine prop subsystem: the process-wide manager that builds/configures prop instances
// from a shared tree of named preset/skin nodes (instCONSTRUCTOR_NODE). propINST_CONSTRUCTOR
// (per-object) delegates preset lookup and skin-change construction to this single shared
// instance (`mng`, a global — see below).
// DB-verified layout (types_members instCONSTRUCTOR_MNG):
//   sslObject@0 (sslOBJ_REF), presets@4 (dsVECTOR<instCONSTRUCTOR_NODE*,8>), relations@24
//   (dsVECTOR<instCONSTRUCTOR_NODE_RELATIONS*,8>), skinMode@44 (bool) — size 48.

typedef struct instCONSTRUCTOR_MNG {
    sslOBJ_REF sslObject;   // 0x00
    dsVECTOR<instCONSTRUCTOR_NODE *, 8>            presets;   // 0x04 — indexed by GetPresetName/GetPresetCount
    dsVECTOR<instCONSTRUCTOR_NODE_RELATIONS *, 8>  relations; // 0x18
    bool       skinMode;    // 0x2C

    // 0x826DD2A0 (instCONSTRUCTOR_MNG) — number of registered presets. Same body as
    // propINST_CONSTRUCTOR::GetPresetCount (which reads mng.presets.nElem inline); not
    // reversed separately in this batch.
    int GetPresetCount() const; // boundary

    // 0x826E3FD0 — build/apply `presetName` onto `pInst`; `canRemove` controls whether nodes
    // not present in the new preset are torn down (only honored when `!skinMode`, gated behind
    // the `dbg_del` debug toggle). Reversed in the entCreate batch.
    // CORRECTION: an earlier pass here noted this address as 0x826E47B4 (a different, unrelated
    // function) — 0x826E3FD0 is confirmed by direct lookup as instCONSTRUCTOR_MNG::Construct.
    void Construct(const dsTSTRING<char> &presetName, animINST *pInst, bool canRemove);

private:
    // 0x826E1440 (private, AAA) — find the preset node named `presetName` in `presets`.
    // Same-class helper called by Construct; external to this batch. boundary.
    instCONSTRUCTOR_NODE *GetPreset(const dsTSTRING<char> &presetName); // boundary

    // 0x826E1538 (private, AAA) — find the relation-override table named `presetName` in
    // `relations`. Same-class helper called by Construct; external to this batch. boundary.
    instCONSTRUCTOR_NODE_RELATIONS *GetRelations(const dsTSTRING<char> &presetName); // boundary
} instCONSTRUCTOR_MNG;

// The single process-wide instance every propINST_CONSTRUCTOR method operates on (DB address
// 0x8837B598). All prop instances share one construction manager / preset tree.
extern instCONSTRUCTOR_MNG mng;
