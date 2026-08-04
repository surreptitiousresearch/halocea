#include "../../headers/ws/ai/aiGOAL_BASE.h"

// aiGOAL_BASE::GetCmpPathDestWP @ 0x83179380
// ?GetCmpPathDestWP@aiGOAL_BASE@@UAAPAVnavWP@@ABV?$dsTSTRING@D@@AAFAAPAVnavSYS@@_NAAV3@@Z
//
// Base default: compare-path destination resolution is disabled in this build. Report "disabled"
// through errHint and yield no destination waypoint. dest/destWPID/destNS/isNeedHO are ignored.
navWP *aiGOAL_BASE::GetCmpPathDestWP(const dsTSTRING<char> *dest, short *destWPID,
                                    navSYS **destNS, bool isNeedHO, dsTSTRING<char> *errHint)
{
    (void)dest; (void)destWPID; (void)destNS; (void)isNeedHO;
    *errHint = dsTSTRING<char>("disabled");
    return nullptr;
}
