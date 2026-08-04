#include "../../headers/ws/ai/aiAIM_COMMON.h"
#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiSTATUS.h"
#include "../../headers/ws/ent/entTRACKER.h"
#include "../../headers/ws/ds/dsTSTRING.h"

// aiAIM_COMMON::GetGivenAim @ 0x8329A358
// ?GetGivenAim@aiAIM_COMMON@@UBA?AV?$dsTSTRING@D@@XZ  (const, returns dsTSTRING by value)
//
// The name of the assigned aim tracker when a scripted aim is set (mind bit 0x400), else the empty
// string. Result is constructed from the C string via the dsTSTRING(const char*) ctor.
dsTSTRING<char> aiAIM_COMMON::GetGivenAim() const
{
    const char *name;
    if ((pBrain->st.pointee->mind.val >> 10) & 1)
        name = trkAim.GetName();
    else
        name = "";
    return dsTSTRING<char>(name);
}
