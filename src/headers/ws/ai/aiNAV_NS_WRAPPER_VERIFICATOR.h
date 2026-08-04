#pragma once
#include "../ds/dsTSTRING.h"
#include "../ds/dsVECTOR.h"
// ws-engine ai08 nav: a verification wrapper over a nav-system list.
// DB-verified layout (types_members aiNAV_NS_WRAPPER_VERIFICATOR): aiNAV_NS_WRAPPER base@0 (4B),
// isVerMode@4 (bool), nsList@8 (dsVECTOR<...NS,8>) — size 0x1C.

struct aiNAV_NS_WRAPPER_VERIFICATOR {
    // aiNAV_NS_WRAPPER base — its sole slot is a vtable pointer.  boundary (opaque)
    void         *__vftable; // 0x00
    bool          isVerMode; // 0x04
    // nsList element type (aiNAV_NS_WRAPPER_VERIFICATOR::NS) is not touched by this batch.
    struct NS;               // opaque element
    dsVECTOR<NS, 8> nsList;   // 0x08

    // 0x8318FAE0 (?GetName@aiNAV_NS_WRAPPER_VERIFICATOR@@UBA?AV?$dsTSTRING@D@@H@Z) — virtual, sret.
    // Base default: return the fixed string "undefined" regardless of `nsIdx`.
    dsTSTRING<char> GetName(int nsIdx) const;
};
