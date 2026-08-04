#pragma once
#include "propENT.h"
#include "../ds/dsVECTOR.h"
#include "../ds/dsSHARED_PTR.h"
#include "../ds/Deleter.h"
#include "../ds/dsTSTRING.h"
#include "../ds/REF_TYPE.h"
#include "../gs/gsOBJ_MODIFIER_BASE.h"
#include "../ds/dsVECTOR_PARAM_LIST.h"
#include "../ds/dsTYPE_ID.h"
// ws-engine prop: property holding the live list of gsOBJ_MODIFIER_BASE-derived modifier
// instances attached to an entity (e.g. animation/state modifiers), keyed by name against the
// descriptor's parallel name->desc list (see propOBJ_MODIFIER_LIST_DESC).
// DB-verified layout (types_members propOBJ_MODIFIER_LIST): <propENT base>@0 (40),
// listModifiers@40 (dsVECTOR<dsSHARED_PTR<gsOBJ_MODIFIER_BASE,0,Deleter<gsOBJ_MODIFIER_BASE>>,8>,
// 20) — size 60.

// Descriptor for propOBJ_MODIFIER_LIST — full layout in propOBJ_MODIFIER_LIST_DESC.h. Include the
// canonical header rather than redefining a minimal shim here (avoids C2011 when co-included).
#include "propOBJ_MODIFIER_LIST_DESC.h"

struct propOBJ_MODIFIER_LIST : propENT {
    dsVECTOR<dsSHARED_PTR<gsOBJ_MODIFIER_BASE, 0, Deleter<gsOBJ_MODIFIER_BASE> >, 8> listModifiers; // 0x28

    // 0x826D0680 — look up the modifier named `name` in the descriptor's name->desc list; if
    // found, dispatch `event`/`params` to the corresponding live modifier instance in
    // listModifiers via its OnEvent virtual.
    void EventModifier(const dsTSTRING<char> &name, const dsTSTRING<char> &event,
                        const ds_data::REF_TYPE<dsVECTOR_PARAM_LIST> &params);
};
