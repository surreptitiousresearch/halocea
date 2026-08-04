// aiPLANNER::impltIsFlag @ 0x83260628
// ?impltIsFlag@aiPLANNER@@UAA_NABV?$dsTSTRING@D@@@Z
#include "../../headers/ws/ai/aiPLANNER.h"
#include "../../headers/ws/ds/dsSTRID.h"
#include "../../headers/ws/ds/dsCMP.h"
#include "../../headers/ws/ai/aiBRAIN_boundary.h" // _apLog

// Virtual "tIsFlag" impl: true when `flag` (interned) is present in the task-system flag list.
bool aiPLANNER::impltIsFlag(const dsTSTRING<char> &flag)
{
    dsSTRID id(flag, false);
    dsCMP cmp;
    int idx = taskSysFlags.propList.FindSorted<dsCMP, dsSTRID>(id, cmp);
    bool found = (idx != -1);
    _apLog("~AITask~tIsFlag(%s): %s", flag.CStr(), found ? "true" : "false");
    return found;
}
