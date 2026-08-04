#pragma once
#include "dsVECTOR.h"
#include "dsTSTRING.h"
// ws-engine ds: a compiled glob-style pattern matcher over one or more '/'-delimited path
// segments, supporting `*`/`?` wildcards per segment plus `..`-style "revert" segments.
// DB-verified layout (types_members ds::WILDCARD): caseSensitive@0, direct@4
// (dsVECTOR<dsVECTOR<dsTSTRING<char>,8>,8> — a list of alternative patterns, each itself a
// list of per-segment glob strings), revert@24 (same shape, for the "not"/exclude list) —
// size 44.

namespace ds {

typedef struct WILDCARD {
    bool                                       caseSensitive; // 0x00
    unsigned char _pad0[3]; /* db-verified padding */
    dsVECTOR<dsVECTOR<dsTSTRING<char>, 8>, 8>   direct;         // 0x04 -- alternative include patterns
    dsVECTOR<dsVECTOR<dsTSTRING<char>, 8>, 8>   revert;         // 0x18 -- alternative exclude patterns

    // Compile `pattern` (comma/space-separated glob list) into direct/revert; caseSensitive
    // selects ordinal vs case-insensitive segment matching. boundary (not decompiled in this
    // batch; only called by HALO_SOUND_LIST::InitSetupTagParams).
    void Init(const dsTSTRING<char> *pattern, bool caseSensitive); // boundary
    // True if `name` matches any `direct` pattern and no `revert` pattern. boundary.
    bool Match(const dsTSTRING<char> *name) const;                 // boundary
} WILDCARD;

} // namespace ds
