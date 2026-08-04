// aiPLANNER::impltClearFlag @ 0x832618B0
// ?impltClearFlag@aiPLANNER@@UAA_NABV?$dsTSTRING@D@@@Z
#include "../../headers/ws/ai/aiPLANNER.h"
#include "../../headers/ws/ds/dsTSTRING.h"
#include "../../headers/ws/ds/dsSTRID.h"
#include "../../headers/ws/ds/dsCMP.h"

extern void _apLog(const char *fmt, ...); // ws/ap — engine log  boundary

// SSL "tClearFlag" op: remove `flag` from the active task-system flag set. Returns whether the flag
// was present (a miss logs "nothing to clear"). The set is looked up twice — once to report the
// result, once to erase — matching the source.
bool aiPLANNER::impltClearFlag(const dsTSTRING<char> &flag)
{
    dsSTRID flagId(flag, false);

    dsCMP cmp;
    bool existed = taskSysFlags.propList.FindSorted<dsCMP, dsSTRID>(flagId, cmp) != -1;

    _apLog("~AITask~tClearFlag(%s): %s", flag.CStr(), existed ? "success" : "nothing to clear");

    dsCMP cmp2;
    int idx = taskSysFlags.propList.FindSorted<dsCMP, dsSTRID>(flagId, cmp2);
    if (idx != -1)
        taskSysFlags.propList.Erase(idx, 1);

    return existed;
}
