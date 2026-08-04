#pragma once
#include "../ds/dsFIXED_BIT_ARRAY.h"
#include "../ds/dsTSTRING.h"
#include "../ia/iaDESC.h"

// ws-engine sml (small state-machine library). iaIACTOR is the entity/actor engine's actor
// type (separate subsystem, external to this batch) — treated as an opaque boundary
// declaration here. iaDESC (and its RegisterSmlState, called by sml::STATE::Set(name)) is now
// a real, fully-reversed type — see ../ia/iaDESC.h; no local stub is kept here.
class iaIACTOR; // boundary — entity/actor engine

// ??$Desc@ViaIACTOR@@@@YAPAViaDESC@@PBViaIACTOR@@@Z — free template helper that returns
// the iaDESC describing `actor`. boundary (not decompiled here).
template<class T>
iaDESC *Desc(const T *actor); // boundary

namespace sml {

// DB-verified layout (types_members sml::STATE): state@0x00 (dsFIXED_BIT_ARRAY<2>, 8 bytes),
// pOwner@0x08 (iaIACTOR*, 4 bytes). Total size 12 bytes.
// An active-state bitmask (up to 64 state ids, dsFIXED_BIT_ARRAY<2> == 2*32 bits) held by
// an owning actor.
typedef struct STATE {
    dsFIXED_BIT_ARRAY<2> state; // 0x00 — bit set of active state id(s)
    iaIACTOR *pOwner;           // 0x08 — owning actor; used to resolve state names via iaDESC

    // 0x82666510 (sml::STATE) — set the active state by NAME: resolve `nameState` to a
    // numeric state id via the owner's iaDESC::RegisterSmlState, then forward to Set(int).
    void Set(const dsTSTRING<char> &nameState);

    // 0x826661F0 (sml::STATE) — set the active state by numeric id: bounds-checked
    // (0 <= idxState < 64) direct bit-set into `state`.
    void Set(int idxState);

    // sml::STATE — test / clear the active-state bit for numeric id `idxState`. boundary.
    bool Is(int idxState) const;
    void Clear(int idxState);
} STATE;

} // namespace sml
