#pragma once
#include "../ds/dsVECTOR.h"
#include "../ds/dsTSTRING.h"

// ws-engine sml: small state-machine library — name-to-id registry for state types.
// DB-verified layout (types_members sml::STATE_FACTORY): states@0, size 20
// (a single dsVECTOR<dsTSTRING<char>,8> — no vtable, no other fields).

namespace sml {

typedef struct STATE_FACTORY {
    dsVECTOR<dsTSTRING<char>, 8> states; // 0x00 — registered state names, index == state id

    // 0x82666568 — look up the registered name for a state id (bounds-checked via
    // dsVECTOR::operator[], boundary — external to this batch).
    const dsTSTRING<char> &GetState(int idx) const;

    // 0x826666E8 — register a new named state, or return the existing id if `nameState`
    // is already registered.
    int Register(const dsTSTRING<char> &nameState);

    // 0x82666598 — look up an existing state's id by name; -1 if not found.
    int Find(const dsTSTRING<char> &nameState);
} STATE_FACTORY;

} // namespace sml
