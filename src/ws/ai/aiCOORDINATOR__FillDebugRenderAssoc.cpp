#include "../../headers/ws/ai/aiCOORDINATOR.h"

// aiCOORDINATOR::FillDebugRenderAssoc @ 0x83235AE8
// ?FillDebugRenderAssoc@aiCOORDINATOR@@IAAXABV?$dsVECTOR@UnavASSOC_INFO@@$07@@@Z
//
// Snapshot up to 100 nav-association records into the debug-render watchee buffer.
void aiCOORDINATOR::FillDebugRenderAssoc(const dsVECTOR<navASSOC_INFO, 8> &dbgAssocInfo)
{
    int count = dbgAssocInfo.nElem;
    if ((unsigned int)count >= 100)
        count = 100;
    dbgGroupAssocWatcheeCount = count;
    for (int i = 0; i < count; ++i)
        dbgGroupAssocWatchee[i] = dbgAssocInfo[i];
}
