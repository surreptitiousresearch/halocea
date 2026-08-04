// aiSPD::ParsePS @ 0x832B0708
// ?ParsePS@aiSPD@@QAAXABVpsSECTION@@ABV?$dsTSTRING@D@@@Z
//
// Parse a speed/gait descriptor out of script section `ps` (named `_name`). Reads the distance
// tunables and FSM event ids, defaulting each to its current value, and re-interns the (possibly
// updated) FSM event / trigger names. The section handle is accessed through psSECTION_INTERFACE
// (which embeds psSECTION at offset 0).
#include "../../headers/ws/ai/aiGOAL_BASE.h"          // aiSPD
#include "../../headers/ws/ps/psSECTION_INTERFACE.h"
#include "../../headers/ws/ds/dsSTRID.h"
#include "../../headers/ws/ds/dsTSTRING.h"

void aiSPD::ParsePS(const psSECTION *ps, const dsTSTRING<char> *name)
{
    // psSECTION_INTERFACE : psSECTION — the parse accessors live on the interface view.
    const psSECTION_INTERFACE *psRead = static_cast<const psSECTION_INTERFACE *>(ps);

    this->name = dsSTRID(*name, false);

    // Current FSM ids become the read defaults.
    dsTSTRING<char> fsmEventStr(this->fsmEvent.CStr());
    dsTSTRING<char> fsmOnStr(this->fsmTriggerEventOn.CStr());
    dsTSTRING<char> fsmOffStr(this->fsmTriggerEventOff.CStr());

    this->maxDist = psRead->GetFloat("maxDist", this->maxDist, 0);
    this->maxDistDefault = this->maxDist;
    this->minDistToEnemy = psRead->GetFloat("minDistToEnemy", this->minDistToEnemy, 0);
    this->maxDistToGoal = psRead->GetFloat("maxDistToGoal", this->maxDistToGoal, 0);
    this->isHOJumpSlideEnabled =
        psRead->GetBool("isHOJumpSlideEnabled", this->isHOJumpSlideEnabled, 0) != 0;
    this->hoJumpSlideDist = psRead->GetFloat("hoJumpSlideDist", this->hoJumpSlideDist, 0);
    this->isInertiaSupported =
        psRead->GetBool("isInertiaSupported", this->isInertiaSupported, 0) != 0;

    fsmEventStr = psRead->GetStr("fsmEvent", fsmEventStr, 0);
    fsmOnStr = psRead->GetStr("fsmTriggerEventOn", fsmOnStr, 0);
    fsmOffStr = psRead->GetStr("fsmTriggerEventOff", fsmOffStr, 0);

    this->isForcedOnly = psRead->GetBool("isForcedOnly", this->isForcedOnly, 0) != 0;

    this->fsmEvent = dsSTRID(fsmEventStr, false);
    if (fsmOnStr.pBuffer->strLen)
        this->fsmTriggerEventOn = dsSTRID(fsmOnStr, false);
    if (fsmOffStr.pBuffer->strLen)
        this->fsmTriggerEventOff = dsSTRID(fsmOffStr, false);
}
