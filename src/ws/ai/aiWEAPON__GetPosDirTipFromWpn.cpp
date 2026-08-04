#include "../../headers/ws/ai/aiWEAPON.h"
#include "../../headers/ws/ai/aiWEAPON_boundaries.h"
#include "../../headers/ws/wpn/wpnLIST_BASE_boundary.h"
#include "../../headers/ws/m3d/m3dV.h"

// aiWEAPON::GetPosDirTipFromWpn @ 0x83293C58
// ?GetPosDirTipFromWpn@aiWEAPON@@IBA_NV?$aiW_IDX@VpropWPN_AI_USABLE@@@@AAUm3dV@@1@Z
//
// Resolve a controller index to its weapon interface and read the muzzle-tip origin/direction from
// its shooter. Validates the results (strong-assert) and, when the debug CDT override is on, warps
// the tip position to the debug camera. Returns false when the interface is absent.
bool aiWEAPON::GetPosDirTipFromWpn(aiW_IDX<propWPN_AI_USABLE> widx, m3dV &posTip, m3dV &dirTip) const
{
    wpnWEAPON *iface = GetIFace(widx);
    if (!iface)
        return false;

    iface->shooter.GetOrgDirTip(&posTip, &dirTip, 0, 1);

    if (!IGNORE_STRONG_ASSERT) {
        if (!_m3dCheckValid(&posTip))
            STRONG_ASSERT_DUMMY::Crash(nullptr, "_m3dCheckValid(&posTip)",
                "D:\\Projects\\code\\common\\src.sys\\gm_shared\\ai08\\a8_weapon.cpp", 986,
                byte_8200155A);
        if (!IGNORE_STRONG_ASSERT && !_m3dCheckValid(&dirTip))
            STRONG_ASSERT_DUMMY::Crash(nullptr, "_m3dCheckValid(&dirTip)",
                "D:\\Projects\\code\\common\\src.sys\\gm_shared\\ai08\\a8_weapon.cpp", 987,
                byte_8200155A);
    }

    if (dbg_defCDT_test.value) {
        m3dV camPos;
        aiDbgGetPosFromCamera(&camPos);
        posTip = camPos;
    }
    return true;
}
