#include "../../headers/ws/ai/aiPERC.h"
#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiSTATUS.h"

// aiPERC::ApplyForcedWarPeace @ 0x8329CE18
// ?ApplyForcedWarPeace@aiPERC@@IAA_NXZ
//
// Honour a scripted force-war (gen bit0) / force-peace (gen bit1) override on the mind's "battle"
// flag (mind bit 0x80). Returns true when an override was applied.
bool aiPERC::ApplyForcedWarPeace()
{
    aiSTATUS *st = pBrain->st.pointee;
    unsigned int gen = st->gen.val;
    if (gen & 1) {
        st->mind.val |= 0x80u;
        return true;
    }
    if ((gen >> 1) & 1) {
        st->mind.val &= ~0x80u;
        return true;
    }
    return false;
}
