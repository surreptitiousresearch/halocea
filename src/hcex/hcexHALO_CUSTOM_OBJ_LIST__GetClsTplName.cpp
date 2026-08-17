/* ?GetClsTplName@hcexHALO_CUSTOM_OBJ_LIST@@QAA_NHHAAV?$dsTSTRING@D@@00@Z @0x823DCAD8 */
#include "../headers/hcex/hcexHALO_CUSTOM_OBJ_LIST.h"
#include "../headers/ws/ds/dsCMP.h"

// 0x823DCAD8
// Look up the pending create-data entry keyed by (sceneryIdx,typeId) and copy out its
// template/class/instance names. Returns false when no entry is sorted-found for the key.
bool hcexHALO_CUSTOM_OBJ_LIST::GetClsTplName(int sceneryIdx, int typeId, dsTSTRING<char> &clsName,
                                             dsTSTRING<char> &tplName, dsTSTRING<char> &instName)
{
    unsigned int key = (typeId << 16) | sceneryIdx;
    dsCMP cmp;
    int idx = cdList.FindSorted<dsCMP, unsigned long>(key, cmp);
    if ( idx < 0 )
        return false;

    tplName  = cdList[idx].val.nameTpl;
    clsName  = cdList[idx].val.nameClass;
    instName = cdList[idx].val.nameInst;
    return true;
}
