#pragma once
#include "../ds/dsVECTOR.h"
#include "../ds/dsTSTRING.h"
#include "STATE_FACTORY.h"

// ws-engine sml: small state-machine library — description/lookup table of state ids
// registered against the shared sml::fctState name registry. Distinct from
// sml::STATE_FACTORY (the registry itself): STATE_DESC is a per-owner ordered list of
// state ids this owner cares about, keyed by name via sml::fctState.
// DB-verified layout (types_members sml::STATE_DESC): nameIdx@0, size 20
// (a single dsVECTOR<int,8> — no vtable, no other fields).

namespace sml {

extern STATE_FACTORY fctState; // 0x83280800 — process-wide shared state-name registry

typedef struct STATE_DESC {
    dsVECTOR<int, 8> nameIdx; // 0x00 — state ids (indices into sml::fctState), in registration order

    // 0x82666750 — register nameState with the shared sml::fctState registry (or reuse its
    // existing id) and append that id to nameIdx if not already present here; returns the
    // index within this STATE_DESC (nameIdx position), not the sml::fctState id.
    int Register(const dsTSTRING<char> &nameState);

    // 0x82666638 — look up nameState among the entries already recorded in this STATE_DESC
    // by comparing against sml::fctState's registered names; returns its position in
    // nameIdx, or -1 if not found here.
    int Find(const dsTSTRING<char> &nameState);

    // Un-reversed same-class methods called by siblings — declared for type-checkability. // boundary
    STATE_DESC();
    ~STATE_DESC();
    STATE_DESC &operator=(const STATE_DESC &other);
    int GetNumStates() const;
} STATE_DESC;

} // namespace sml
