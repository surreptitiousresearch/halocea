// aiGOAL::BuildFreePath @ 0x832AF0C0
// ?BuildFreePath@aiGOAL@@UAA_NFPAVnavPATH@@@Z
#include "../../headers/ws/ai/aiGOAL.h"
#include "../../headers/ws/nav/navSYS.h"
#include "../../headers/ws/nav/navPATH.h"
#include "../../headers/ws/nav/navQUERY.h"
#include "../../headers/apCL.h"
#include "../../headers/ws/dbg/STRONG_ASSERT_DUMMY.h"

extern int IGNORE_STRONG_ASSERT;      // _IGNORE_STRONG_ASSERT
extern unsigned char byte_8200155A;   // shared empty-string / assert-info byte @0x8200155A

static const char *const kSrcFile =
    "D:\\Projects\\code\\common\\src.sys\\gm_shared\\ai08\\a8_goal.cpp";

// Build a penalty/HO/used-path-aware path to waypoint `plid` into `pPath` using the current nav sys.
bool aiGOAL::BuildFreePath(short plid, navPATH *pPath)
{
    if (!navSys)
        return false;
    if (!IGNORE_STRONG_ASSERT && !pPath)
        STRONG_ASSERT_DUMMY::Crash(nullptr, "pPath", kSrcFile, 982, byte_8200155A);

    apCL cl = { kSrcFile, 985 };
    navQUERY query(pBrain, &cl);
    query.plidTo = plid;
    query.applyPenalties = true;
    query.applyHOs = true;
    query.applyUsedPaths = true;
    return navSys->BuildPath(&query, pPath);
}
