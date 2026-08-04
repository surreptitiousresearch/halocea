#include "../../headers/ws/ai/aiPLANNER.h"

// aiPLANNER::DbgTaskFlagsLine(dsTSTRING<char>&) — 0x83166D88
// Append a debug listing of the active task-system flag names onto `line`, one per "\r\n\t"-
// prefixed row, or "[none]" when there are none.
void aiPLANNER::DbgTaskFlagsLine(dsTSTRING<char> &line)
{
    int nElem = taskSysFlags.propList.nElem;
    if (nElem == 0)
    {
        line.Insert(line.pBuffer->strLen, "[none]", -1);
        return;
    }

    for (int i = 0; i < nElem; ++i)
    {
        line.Insert(line.pBuffer->strLen, "\r\n\t", -1);
        dsSTRID &flag = taskSysFlags.propList[i];
        line.Insert(line.pBuffer->strLen, flag.CStr(), -1);
    }
}
