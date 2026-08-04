#pragma once
#include "propBASE_DESC.h"
#include "../ds/dsVECTOR.h"
#include "../ds/dsSMART_PTR.h"
#include "../ds/dsREF_COUNT.h"
// ws-engine prop: descriptor for a propCONTAINER (the parsed, shared config template describing
// an entity's property/component set). DB-verified layout (types_members propCONTAINER_DESC):
// propBASE_DESC (base)@0 (52B), propDesc@52 (dsVECTOR<dsSMART_PTR<propBASE_DESC,...>,8>, 20B),
// componentDesc@72 (same, 20B) — size 92. Beyond the layout this stays an OTHER-SUBSYSTEM
// BOUNDARY — only the one virtual entry point iaIACTOR::UnshareDescIfNeeded dispatches through
// is modeled.
struct propCONTAINER_DESC : propBASE_DESC {
    dsVECTOR<dsSMART_PTR<propBASE_DESC, _dsSP_OWNER_PLC_REFCOUNT<dsREF_COUNT *> >, 8> propDesc;      // 0x34
    dsVECTOR<dsSMART_PTR<propBASE_DESC, _dsSP_OWNER_PLC_REFCOUNT<dsREF_COUNT *> >, 8> componentDesc; // 0x48

    // DB-verified static (?TYPE_ID@propCONTAINER_DESC@@2VdsTYPE_ID@@A @ 0x8427C254) — this class's
    // runtime-type bitmask, used by propBASE::GetProperty<P> to detect container objects.
    static dsTYPE_ID TYPE_ID;

    // 0x82705AE8 (?IsNeedToUnshareByInst@propCONTAINER_DESC@@UBA_NXZ) — true when a live instance
    // referencing this (possibly shared) descriptor must clone a private copy before mutating it
    // (e.g. an instance-specific config override was applied). const (UBA), virtual. boundary.
    bool IsNeedToUnshareByInst() const;
};
