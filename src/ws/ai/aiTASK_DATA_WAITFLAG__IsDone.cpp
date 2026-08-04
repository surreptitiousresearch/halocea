// aiTASK_DATA_WAITFLAG::IsDone @ 0x832609C0
// ?IsDone@aiTASK_DATA_WAITFLAG@@UAA_NXZ
#include "../../headers/ws/ai/aiTASK_DATA_WAITFLAG.h"
#include "../../headers/ws/ai/aiPLANNER.h"
#include "../../headers/ws/ds/dsCMP.h"

extern aiPLANNER *aiPlanner; // ai08 process-wide planner

// Virtual: this wait-flag task is complete once the planner's active task-system flag set contains
// `flag` (binary search of the ascending-sorted propList).
bool aiTASK_DATA_WAITFLAG::IsDone()
{
    dsCMP cmp;
    return aiPlanner->taskSysFlags.propList.FindSorted<dsCMP, dsSTRID>(flag, cmp) != -1;
}
