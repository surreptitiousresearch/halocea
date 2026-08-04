#include "../../headers/ws/ai/aiCOORDINATOR.h"

// aiCOORDINATOR::BuildGroupFrontsMesh @ 0x83237A20
// ?BuildGroupFrontsMesh@aiCOORDINATOR@@IAAXPBVnavSYS@@ABV?$dsVECTOR@V?$dsTSTRING@D@@$07@@1@Z
//
// Rebuild the group's front/excluder debug meshes: clear the four vertex/index buffers, then append
// each used front (green) and each excluder front (red) as an outlined nav-domain mesh.
void aiCOORDINATOR::BuildGroupFrontsMesh(const navSYS *dbgUsedNS,
                                         const dsVECTOR<dsTSTRING<char>, 8> &dbgUsedFrontList,
                                         const dsVECTOR<dsTSTRING<char>, 8> &dbgExclFrontList)
{
    dbgGroupExcludersVBuf.nElem = 0;
    dbgGroupExcludersIBuf.nElem = 0;
    dbgGroupFrontsVBuf.nElem = 0;
    dbgGroupFrontsIBuf.nElem = 0;

    if (!dbgUsedNS)
        return;

    int usedCount = dbgUsedFrontList.nElem;
    for (int i = 0; i < usedCount; ++i)
        AddGroupFrontsMesh(dbgUsedNS, dbgUsedFrontList[i], 0xFF00FF00,
                           dbgGroupFrontsVBuf, dbgGroupFrontsIBuf);

    int exclCount = dbgExclFrontList.nElem;
    for (int i = 0; i < exclCount; ++i)
        AddGroupFrontsMesh(dbgUsedNS, dbgExclFrontList[i], 0xFFFF0000,
                           dbgGroupExcludersVBuf, dbgGroupExcludersIBuf);
}
