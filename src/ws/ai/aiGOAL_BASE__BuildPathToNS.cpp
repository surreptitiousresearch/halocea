#include "../../headers/ws/ai/aiGOAL_BASE.h"

// aiGOAL_BASE::BuildPathToNS @ 0x83179428
// ?BuildPathToNS@aiGOAL_BASE@@UAA_NFPBVm3dMATR@@PBVnavSYS@@1PAVnavPATH@@AAV?$dsTSTRING@D@@@Z
//
// Base default: cross-nav-system path building is disabled in this build. Report "disabled"
// through errHint and yield no path. The start/matrix/system/path arguments are ignored.
// (Mangle return type is _N (bool); the decompiler widened it to int — 0 either way.)
int aiGOAL_BASE::BuildPathToNS(short wpidFrom, const m3dMATR *mFrom, const navSYS *srcNS,
                              const navSYS *dstNS, navPATH *pPath, dsTSTRING<char> *errHint)
{
    (void)wpidFrom; (void)mFrom; (void)srcNS; (void)dstNS; (void)pPath;
    *errHint = dsTSTRING<char>("disabled");
    return 0;
}
