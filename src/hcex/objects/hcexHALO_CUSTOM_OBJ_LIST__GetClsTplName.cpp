/* hcexHALO_CUSTOM_OBJ_LIST::GetClsTplName @0x823DCAF4 — look up the pending create-data entry
 * keyed by (sceneryIdx,typeId) (packed as typeId<<16 | sceneryIdx) via a binary search, and copy
 * out its template/class/instance names. Returns false (out-params left untouched) on a miss. */

#include "../../headers/hcex/hcexHALO_CUSTOM_OBJ_LIST.h"
#include "../../headers/ws/ds/dsCMP.h"

bool hcexHALO_CUSTOM_OBJ_LIST::GetClsTplName(int sceneryIdx, int typeId, dsTSTRING<char> &clsName,
                                              dsTSTRING<char> &tplName, dsTSTRING<char> &instName)
{
    unsigned int key = (typeId << 16) | sceneryIdx;
    dsCMP cmp;
    int index = cdList.FindSorted(key, cmp);
    if ( index < 0 )
        return false;

    tplName = cdList[index].val.nameTpl;
    clsName = cdList[index].val.nameClass;
    instName = cdList[index].val.nameInst;
    return true;
}
