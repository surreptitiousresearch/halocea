#include "../../headers/ws/ai/aiWEAPON.h"
#include "../../headers/ws/ai/aiWEAPON_boundaries.h"
#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiSTATUS.h"
#include "../../headers/ws/prop/propWPN_AI_USABLE.h"
#include "../../headers/ws/prop/propWPN_AI_USABLE_DESC.h"
#include "../../headers/ws/prop/propBASE.h"
#include "../../headers/ws/m3d/m3dV.h"

// aiWEAPON::GetPosDirTipFromBody @ 0x83294AE8
// ?GetPosDirTipFromBody@aiWEAPON@@IBA_NV?$aiW_IDX@VpropWPN_AI_USABLE@@@@AAUm3dV@@1@Z
//
// Body-relative muzzle tip: seed pos/dir from the brain's body coordinate system (posFace/dirBody),
// then, if the controller at `widx` is present, add its descriptor's body-local tip offset expressed
// in the body up/right basis (offset.x*dirUBodyRight + offset.y*dirUpBody + offset.z*dirUBody).
bool aiWEAPON::GetPosDirTipFromBody(aiW_IDX<propWPN_AI_USABLE> widx, m3dV &posTip, m3dV &dirTip) const
{
    const aiSTATUS *st = pBrain->st.pointee;
    posTip = st->cs.posFace;
    dirTip = st->cs.dirBody;

    const CTRL_INFO &info = ctrlsList[widx.idx];
    propWPN_AI_USABLE *ctrl = info.ctrl.pHandle ? info.ctrl.pHandle->pPtr : nullptr;
    if (ctrl) {
        // spDesc is the propBASE-inherited descriptor smart pointer (propBASE@0x10); its pointee is a
        // propWPN_AI_USABLE_DESC for this controller type.
        const propWPN_AI_USABLE_DESC *desc =
            (const propWPN_AI_USABLE_DESC *)((const propBASE *)ctrl)->spDesc.pointee;
        if (!IGNORE_STRONG_ASSERT && !desc)
            STRONG_ASSERT_DUMMY::Crash(nullptr, "ctrlDesc",
                "D:\\Projects\\code\\common\\src.sys\\gm_shared\\ai08\\a8_weapon.cpp", 1016,
                byte_8200155A);

        m3dV vUBody      = desc->tipOffset.z * st->cs.dirUBody;      // pointee[0x94] * (aiSTATUS+0xD8)
        m3dV vUpBody     = desc->tipOffset.y * st->cs.dirUpBody;     // pointee[0x90] * (aiSTATUS+0x38)
        m3dV vUBodyRight = desc->tipOffset.x * st->cs.dirUBodyRight; // pointee[0x8C] * (aiSTATUS+0xE4)
        _m3dCheckValid(&vUBody);
        _m3dCheckValid(&vUpBody);

        m3dV partial;
        partial.x = vUpBody.x + vUBody.x;
        partial.y = vUBody.y + vUpBody.y;
        partial.z = vUBody.z + vUpBody.z;
        _m3dCheckValid(&partial);
        _m3dCheckValid(&vUBodyRight);

        m3dV tipOffsetWorld;
        tipOffsetWorld.x = vUBodyRight.x + partial.x;
        tipOffsetWorld.y = vUBodyRight.y + partial.y;
        tipOffsetWorld.z = vUBodyRight.z + partial.z;
        posTip += tipOffsetWorld;
    }
    return true;
}
