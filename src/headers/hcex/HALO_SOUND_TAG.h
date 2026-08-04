#pragma once
// HCEX sound bridge: one playable Blam sound-tag entry inside a HALO_SOUND_LIST's FSB bank.
// DB-verified layout (types_members HALO_SOUND_TAG): name@0 (dsSTRID), startIndex@4,
// count@8, previousIndex@12, params@16 (HALO_SOUND_PARAMS*) — size 20.
// Sibling type: owned by the parallel HALO_SOUND_LIST drain; declared here from the DB only
// far enough to type HALO_SOUND_SYSTEM's field reads (name/count/startIndex/previousIndex).

#include "../ws/ds/dsSTRID.h"

struct HALO_SOUND_PARAMS; // forward — full layout in HALO_SOUND_PARAMS.h

typedef struct HALO_SOUND_TAG {
    dsSTRID              name;          // 0x00
    int                   startIndex;    // 0x04 first FSB sub-sound index for this tag
    int                   count;         // 0x08 number of FSB sub-sounds for this tag
    int                   previousIndex; // 0x0C last-picked sub-sound (anti-repeat)
    HALO_SOUND_PARAMS    *params;        // 0x10

    // CMP -- case-insensitive name comparator used by dsVECTOR<HALO_SOUND_TAG,8>::FindSorted
    // over a HALO_SOUND_LIST's tag table.
    // DEVIATION: decompiler shows `const HALO_SOUND_TAG *a` (its convenience prototype string
    // always renders references as pointers); the mangle decodes `a` as `ABU2@@` =
    // reference-to-const, so the true parameter type is `const HALO_SOUND_TAG &`.
    struct CMP {
        int cmp(const HALO_SOUND_TAG &a, const char *name) const; // ?cmp@CMP@HALO_SOUND_TAG@@QBAHABU2@PBD@Z
    };

    HALO_SOUND_TAG(); // ??0HALO_SOUND_TAG@@QAA@XZ
} HALO_SOUND_TAG;
