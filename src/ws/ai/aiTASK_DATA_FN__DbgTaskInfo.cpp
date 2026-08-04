// aiTASK_DATA_FN::DbgTaskInfo @ 0x83260EC0
// ?DbgTaskInfo@aiTASK_DATA_FN@@UAAXAAV?$dsTSTRING@D@@@Z
#include "../../headers/ws/ai/aiTASK_DATA_FN.h"
#include "../../headers/ws/ds/dsTSTRING.h"

// Virtual: print the task type, a separating space, then the per-task info into `out`.
void aiTASK_DATA_FN::DbgTaskInfo(dsTSTRING<char> *out)
{
    DbgPrintType(out);
    out->Insert(out->pBuffer->strLen, " ", -1);
    DbgPrintInfo(out);
}
