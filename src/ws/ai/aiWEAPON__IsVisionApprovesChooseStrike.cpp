#include "../../headers/ws/ai/aiWEAPON.h"
#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiSTATUS.h"
#include "../../headers/ws/ai/aiBRAIN_DESC_abilities_boundary.h"

// aiWEAPON::IsVisionApprovesChooseStrike @ 0x83294268
// ?IsVisionApprovesChooseStrike@aiWEAPON@@IBA_NXZ
//
// May the current vision state permit choosing a strike now?
//  - blind-shoot weapons that need no vision check always approve;
//  - otherwise approve when the enemy is visible now (est bit 5), or
//  - when the "peeker seen" general-state bit (gen bit 20) is set AND the enemy was recently
//    visible (est bit 6).
bool aiWEAPON::IsVisionApprovesChooseStrike() const
{
    if (isOKBlindShoot && !aiBRAIN_DESC_AbilCheckVisForBlindShoot(pBrain->spDesc.pointee))
        return true;

    aiSTATUS *st = pBrain->st.pointee;
    unsigned int est = st->est.val;
    if (((est >> 5) & 1) != 0)
        return true;
    if (((static_cast<unsigned int>(st->gen.val) >> 20) & 1) == 0)
        return false;
    if (((est >> 6) & 1) == 0)
        return false;
    return true;
}
