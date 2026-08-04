// aiTASK_DATA_FN::CanRestartBhv @ 0x832607F0
// ?CanRestartBhv@aiTASK_DATA_FN@@UAA_NK@Z
#include "../../headers/ws/ai/aiTASK_DATA_FN.h"
#include "../../headers/ws/ds/dsCMP.h"

// Virtual: a behaviour may restart only while awaiting a task reply and if `bhvSNum` is one of the
// after-fire behaviours recorded for this task.
bool aiTASK_DATA_FN::CanRestartBhv(unsigned int bhvSNum)
{
    if (state != AI_TASK_DATA_FN_WAIT_REPLY)
        return false;
    unsigned long key = bhvSNum;
    dsCMP cmp;
    return afBhvList.Find<dsCMP, unsigned long>(key, cmp, 0) >= 0;
}
