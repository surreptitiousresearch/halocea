#include "../../headers/ws/ai/aiHO_SYS.h"
#include "../../headers/ws/nav/navWP.h"         // navWP::IsHO
#include "../../headers/ws/mdl/mdlITRC_BASE.h"  // mdlITRC_BASE::IsValid

// aiHO_SYS::IsInHO @ 0x832A79E0
// ?IsInHO@aiHO_SYS@@UAA_NAAVnavWP@@@Z
//
// True when the body's current hideout ITRC is the one that waypoint `wp` produces. Only meaningful for
// HO waypoints; produces the candidate ITRC for `wp` (no-shift, borrow-only) and compares corners.
// Disasm-verified GetItrcHO args: (wp, HO_SHIFT_DISABLED, 0.0f, false, true).
bool aiHO_SYS::IsInHO(navWP &wp)
{
    if (!wp.IsHO())
        return false;

    ItrcPtr current = GetCurrentItrcHO();
    if (!current.pointee) {
        current.Reset();
        return false;
    }

    mdlITRC_HO *produced = GetItrcHO(wp, HO_SHIFT_DISABLED, 0.0f, false, true);
    ItrcPtr candidate(reinterpret_cast<mdlITRC_BASE *>(produced));

    bool match = candidate.pointee
              && candidate.pointee->IsValid()
              && IsSameItrc(candidate, current); // consumes copies of both

    candidate.Reset();
    current.Reset();
    return match;
}
