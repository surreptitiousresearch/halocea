#include "../../headers/ws/ai/aiVISION_COMMON.h"
#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiSTATUS.h"
#include "../../headers/ws/ai/aiBRAIN_DESC.h"
#include "../../headers/ws/ds/dsTSTRING.h"

// aiVISION_COMMON::ProcessInit @ 0x832927D8
// ?ProcessInit@aiVISION_COMMON@@UAAXXZ
//
// Per-spawn init: copy the descriptor's sight-clip flags into the status perception block, zero the
// reported view-percent, and install the default view-percent-raising-speed curve.
void aiVISION_COMMON::ProcessInit()
{
    aiBRAIN_DESC *desc = reinterpret_cast<aiBRAIN_DESC *>(pBrain->spDesc.pointee);
    aiSTATUS     *st   = pBrain->st.pointee;

    st->perc.isClipUpAngle   = desc->isClipUpAngle;
    st->perc.isClipDownAngle = desc->isClipDownAngle;
    st->perc.viewPrecent     = 0;

    dsTSTRING<char> curve("0;10000|888;10000");
    SetVPRaisingSpeedCurve(curve);
}
