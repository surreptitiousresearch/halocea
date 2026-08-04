#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiSTATUS.h"
#include "../../headers/ws/ia/iaSND_SOURCE_DATA.h"
#include "../../headers/ws/m3d/m3dV.h"

// aiBRAIN::GetSndSource @ 0x83299468
// ?GetSndSource@aiBRAIN@@UAAXVdsSTRID@@PAViaSND_SOURCE_DATA@@@Z
//
// Fill `pSrcData`'s start position with this brain's current combat "face" position (from its status
// snapshot). `nameObj` is accepted for interface compatibility but not used by this override.
void aiBRAIN::GetSndSource(dsSTRID nameObj, iaSND_SOURCE_DATA *pSrcData)
{
    (void)nameObj;
    aiSTATUS *status = st.pointee;

    m3dV pos;
    pos.x = status->cs.posFace.x;
    pos.y = status->cs.posFace.y;
    pos.z = status->cs.posFace.z;

    pSrcData->SetStartPos(&pos);
}
