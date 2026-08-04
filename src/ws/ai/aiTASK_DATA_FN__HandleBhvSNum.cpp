// aiTASK_DATA_FN::HandleBhvSNum @ 0x83261970
// ?HandleBhvSNum@aiTASK_DATA_FN@@IAA_NK@Z
#include "../../headers/ws/ai/aiTASK_DATA_FN.h"
#include "../../headers/ws/ds/dsCMP.h"

// Protected: if `bhvSNum` is one of the recorded after-fire behaviours, remove it and return true;
// otherwise return false (nothing accounted).
bool aiTASK_DATA_FN::HandleBhvSNum(unsigned int bhvSNum)
{
    unsigned long key = bhvSNum;
    dsCMP cmp;
    int idx = afBhvList.Find<dsCMP, unsigned long>(key, cmp, 0);
    if (idx < 0)
        return false;
    afBhvList.Erase(idx, 1);
    return true;
}
