#include "../../headers/ws/ai/aiVISION_BATTLE.h"
#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiSTATUS.h"
#include "../../headers/ws/ai/aiBRAIN_DESC.h"

// aiVISION_BATTLE::ProcessInit @ 0x83292880
// ?ProcessInit@aiVISION_BATTLE@@UAAXXZ
//
// Run the common vision init, then seed the status sight cone/range from the descriptor's battle
// sense profile (the first four aiSENSES fields: forward/side cone half-angles and ranges).
void aiVISION_BATTLE::ProcessInit()
{
    aiVISION_COMMON::ProcessInit();

    aiBRAIN_DESC *desc = reinterpret_cast<aiBRAIN_DESC *>(pBrain->spDesc.pointee);
    aiSTATUS     *st   = pBrain->st.pointee;

    st->perc.senses.angleSeeStraight = desc->battle.angleSeeStraight;
    st->perc.senses.angleSeeSide     = desc->battle.angleSeeSide;
    st->perc.senses.radSeeStraight   = desc->battle.radSeeStraight;
    st->perc.senses.radSeeSide       = desc->battle.radSeeSide;
}
