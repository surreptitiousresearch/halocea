#pragma once
#include "propENT_DESC.h"
#include "../DEFAULT_CTOR.h"
#include "../ds/dsVECTOR.h"
#include "../ds/dsPAIR.h"
#include "../ds/dsSTRID.h"
#include "../ds/dsSMART_PTR.h"
#include "../ds/dsREF_COUNT.h"
#include "../gs/gsOBJ_MODIFIER_BASE_DESC.h"
#include "../ds/dsTYPE_ID.h"
// ws-engine prop: descriptor for propOBJ_MODIFIER_LIST — the parsed name->modifier-descriptor
// table that propOBJ_MODIFIER_LIST::EventModifier searches by name.
// DB-verified layout (types_members propOBJ_MODIFIER_LIST_DESC): <propENT_DESC base>@0 (52),
// isEntSslClassInited@52, listModifiers@56 (dsVECTOR<dsPAIR<dsSTRID,dsSMART_PTR<
// gsOBJ_MODIFIER_BASE_DESC,_dsSP_OWNER_PLC_REFCOUNT<dsREF_COUNT*>>>,8>, 20) — size 76.

struct propOBJ_MODIFIER_LIST_DESC : propENT_DESC {
    DEFAULT_CTOR<bool> isEntSslClassInited; // 0x34
    unsigned char _pad35[3];  // 0x35

    // 0x38 name -> modifier-descriptor table (parallel to propOBJ_MODIFIER_LIST::listModifiers).
    dsVECTOR<dsPAIR<dsSTRID, dsSMART_PTR<gsOBJ_MODIFIER_BASE_DESC, _dsSP_OWNER_PLC_REFCOUNT<dsREF_COUNT *> > >, 8> listModifiers;

    // RTTI descriptor id (static; mirrors the sibling *_DESC pattern). Used by
    // iaIACTOR::GetProperty<propOBJ_MODIFIER_LIST> for the property lookup key.
    static dsTYPE_ID TYPE_ID;
};
