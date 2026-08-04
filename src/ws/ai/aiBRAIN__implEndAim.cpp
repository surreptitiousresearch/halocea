#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiMIND.h"
#include "../../headers/ws/ai/aiSTATUS.h"
#include "../../headers/ws/ai/aiAIM_BASE.h"
#include "../../headers/ws/ds/dsTSTRING.h"

// aiBRAIN::implEndAim @ 0x8329A0E0
// ?implEndAim@aiBRAIN@@UAAXXZ
//
// End the scripted aim: drop the aim module's tracker, restore the normal selene subsystem, and
// clear the recorded "given aim" name.
void aiBRAIN::implEndAim()
{
    mind.aim.pointee->DropTrk();
    mind.SetNormalSelene();
    st.pointee->aim.nameGiven.Clear();
}
