// aiTASK_DATA_FN::NoticeStartSubBhv(unsigned int, unsigned int) @ 0x83263260
// ?NoticeStartSubBhv@aiTASK_DATA_FN@@UAAXKK@Z
#include "../../headers/ws/ai/aiTASK_DATA_FN.h"
#include "../../headers/ws/ds/dsCMP.h"

// Virtual: a sub-behaviour started. If its parent is already tracked as a fired behaviour, record the
// child SNum too so the after-fire bookkeeping reaps it when it ends.
void aiTASK_DATA_FN::NoticeStartSubBhv(unsigned int parentBhvSNum, unsigned int bhvSNum)
{
    unsigned long parentKey = parentBhvSNum;
    dsCMP cmp;
    if (afBhvList.Find<dsCMP, unsigned long>(parentKey, cmp, 0) >= 0)
    {
        unsigned long child = bhvSNum;
        afBhvList.PushBack(child);
    }
}
