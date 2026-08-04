#include "../../headers/ws/ai/aiCOORDINATOR.h"
#include "../../headers/ws/ai/aiCOORD_GROUP_boundary.h"
#include "../../headers/ws/ai/aiDEBUG.h"          // aiDEBUG::FillDebugRenderInfoWPs
#include "../../headers/ws/nav/navWP_ID_WEIGHT.h"

// aiCOORDINATOR::UpdateDebugRenderInfo @ 0x83237C20
// ?UpdateDebugRenderInfo@aiCOORDINATOR@@IAAXXZ
//
// Rebuild the group-debug overlay snapshot for the currently-watched brain's group: the used-WP
// list, the WP-set gradient curve mesh, the front/excluder meshes, and the nav-association lines.
// aiCOORD_GROUP's full layout is a boundary (next frontier), so its debug fields are read through
// DB-verified byte offsets: wpSet@0xE4(228), assocInfo@0x150(336), curvePoints@0x164(356),
// startMatr@0x178(376), dbgUsedNS@0x1B8(440), usedFrontList@0x1BC(444), exclFrontList@0x1D0(464).
void aiCOORDINATOR::UpdateDebugRenderInfo()
{
    aiBRAIN *watchee = currentWatchee;
    dbgGroupWpSetWatcheeCount = 0;
    if (!watchee)
        return;

    aiCOORD_GROUP *grp = GetGroup_2(watchee);
    if (!grp)
        return;

    char *g = reinterpret_cast<char *>(grp);

    aiDEBUG::FillDebugRenderInfoWPs(
        *reinterpret_cast<dsVECTOR<navWP_ID_WEIGHT, 8> *>(g + 228),
        dbgGroupWpSetWatchee, 100, &dbgGroupWpSetWatcheeCount);

    BuildGroupWPSetCurveMesh(
        *reinterpret_cast<dsVECTOR<m2dV, 8> *>(g + 356),
        *reinterpret_cast<m3dMATR *>(g + 376));

    BuildGroupFrontsMesh(
        *reinterpret_cast<navSYS **>(g + 440),
        *reinterpret_cast<dsVECTOR<dsTSTRING<char>, 8> *>(g + 444),
        *reinterpret_cast<dsVECTOR<dsTSTRING<char>, 8> *>(g + 464));

    FillDebugRenderAssoc(
        *reinterpret_cast<dsVECTOR<navASSOC_INFO, 8> *>(g + 336));
}
