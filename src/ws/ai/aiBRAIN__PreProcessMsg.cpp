#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ent/entENTITY.h"     // base entENTITY::PreProcessMsg + pInst
#include "../../headers/animINST.h"              // animINST::cdtLayer / ClearIGNORE_CDT_ALWAYS

// aiBRAIN::PreProcessMsg(int, void*, msgADDR*) @ 0x832AD770
// ?PreProcessMsg@aiBRAIN@@UAAHHPAXPAVmsgADDR@@@Z
//
// Virtual message pre-dispatch hook. On the TERM message (2) fire the SSL OnDestroy event first,
// then run the base entity pre-process. On the INIT message (1) build the brain subsystems; on
// message 1257 reset the instance's collision-detection layer (128 -> 0x4000) and clear its
// always-ignore-CDT flag. Returns the base pre-process result unchanged.
int aiBRAIN::PreProcessMsg(int msg, void *pInfo, msgADDR *pSend)
{
    if (msg == 2) // MSG_TERM/destroy
        OnDestroy();

    int res = entENTITY::PreProcessMsg(msg, pInfo, pSend);

    if (msg == 1) // MSG_INIT
    {
        PreProcessINIT();
    }
    else if (msg == 1257)
    {
        animINST *inst = pInst;
        if (inst && inst->cdtLayer == 128)
        {
            inst->cdtLayer = 0x4000;
            inst->ClearIGNORE_CDT_ALWAYS();
        }
    }
    return res;
}
