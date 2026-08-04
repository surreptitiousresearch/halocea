// aiPLANNER::impltSetFlag(const dsTSTRING<char>&) @ 0x83263C48
// ?impltSetFlag@aiPLANNER@@UAA_NABV?$dsTSTRING@D@@@Z
#include "../../headers/ws/ai/aiPLANNER.h"
#include "../../headers/ws/ds/dsSTRID.h"
#include "../../headers/ws/ds/dsCMP.h"
#include "../../headers/ws/ds/dsSTRID_FLAGS.h"
#include "../../headers/ws/ds/dsTSTRING.h"

extern void _apLog(const char *fmt, ...); // ?_apLog@@YAXPBDZZ  boundary

// SSL "tSetFlag" op: add `flag` to the active task-system flag set; returns true only when it was
// not already present.
bool aiPLANNER::impltSetFlag(const dsTSTRING<char> &flag)
{
    dsSTRID_FLAGS *flags = &this->taskSysFlags;

    dsSTRID id(flag, false);
    dsCMP cmp;
    // FindSorted returns the existing index or -1; the flag is newly set only when absent (index -1).
    int found = flags->propList.FindSorted<dsCMP, dsSTRID>(id, cmp);
    bool wasNewlySet = (found + 1 == 0);

    _apLog("~AITask~tSetFlag(%s): %s", flag.pBuffer->str, wasNewlySet ? "success" : "already set");

    flags->Set(id, true);
    return wasNewlySet;
}
