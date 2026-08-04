#include "../../headers/ws/ai/aiWEAPON.h"
#include "../../headers/ws/m3d/m3dV.h"

// ?widxCTRL_INVALID@@3V?$aiW_IDX@VpropWPN_AI_USABLE@@@@B @ 0x8416CAAC — the sentinel "no controller"
// index. boundary.
extern const aiW_IDX<propWPN_AI_USABLE> widxCTRL_INVALID;

// aiWEAPON::GetPosDirTip @ 0x832951A8
// ?GetPosDirTip@aiWEAPON@@IBA_NV?$aiW_IDX@VpropWPN_AI_USABLE@@@@W4aimCHECK_TYPE@@AAUm3dV@@2@Z
//
// Dispatch a muzzle-tip query for controller `widx` by aim-check `type`: modes 1 and 2 use the
// body-relative tip, other valid modes (0/3) use the weapon-shooter tip. Fails on an invalid index
// or an out-of-range type.
bool aiWEAPON::GetPosDirTip(aiW_IDX<propWPN_AI_USABLE> widx, aimCHECK_TYPE type,
                           m3dV &posTip, m3dV &dirTip) const
{
    if (widx.idx == widxCTRL_INVALID.idx || (unsigned int)type > 3)
        return false;

    aiW_IDX<propWPN_AI_USABLE> w;
    w.idx = widx.idx;
    if (type == 1 || type == 2)
        return GetPosDirTipFromBody(w, posTip, dirTip);
    return GetPosDirTipFromWpn(w, posTip, dirTip);
}
