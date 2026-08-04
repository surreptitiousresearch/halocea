// aiTASK::ParseTaskFnSpec(int, aiTASK_DATA_FN*, dsSTRID&, dsTSTRING<char>&, dsVECTOR<dsDATA,8>&) @ 0x832623C0
// ?ParseTaskFnSpec@aiTASK@@IAAXHPAVaiTASK_DATA_FN@@AAVdsSTRID@@AAV?$dsTSTRING@D@@AAV?$dsVECTOR@VdsDATA@@$07@@@Z
#include "../../headers/ws/ai/aiTASK.h"
#include "../../headers/ws/ai/aiTASK_DATA_FN.h"
#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/AI_SSL_TASK_FN_INFO.h"
#include "../../headers/ws/ds/dsDATA.h"
#include "../../headers/ws/ds/dsDATA_TYPE.h"
#include "../../headers/ws/ds/ds_boundary.h"        // dsDATA_TYPE_STORAGE<>::dataType
#include "../../headers/ws/ds/NULL_TYPE.h"
#include "../../headers/ws/ds/dsCONVERTION_TYPE.h"  // DSD_CONV_RETRIEVE
#include "../../headers/ws/ds/dsVECTOR.h"
#include "../../headers/ws/ds/dsTSTRING.h"
#include "../../headers/ws/ds/dsSTRID.h"

// Protected: look up the built-in descriptor row for the SSL task-fn `fnName`, seed the task's
// after-fire config (InitAF), and resolve its behaviour-name argument (`bhvName`). When the fn is
// unknown, clear `bhvName` and fall back to the default force-AF checker.
void aiTASK::ParseTaskFnSpec(int paramsOffset, aiTASK_DATA_FN *task, dsSTRID &fnName,
                             dsTSTRING<char> &bhvName, dsVECTOR<dsDATA, 8> &sslParams)
{
    // Scan the descriptor table for a matching fn-name id.
    int idx = 0;
    while (idx < AI_SSL_TASK_FN_INFO_COUNT && sslTaskFnInfo[idx].fnName.id != fnName.id)
        ++idx;

    if (idx >= AI_SSL_TASK_FN_INFO_COUNT)
    {
        // Unknown task-fn: no behaviour name, no result checks, default force-AF checker.
        bhvName = dsTSTRING<char>("");
        task->InitAF(false, false, &aiBRAIN::CheckCallForceAF_Default);
        return;
    }

    const AI_SSL_TASK_FN_INFO &row = sslTaskFnInfo[idx];
    // The row's fnChecker slot is the packed pointer-to-member checker delegate.
    task->InitAF(row.checkFnResult, row.checkBhvResults,
                 *reinterpret_cast<const aiForceAFfnChecker *>(row.fnChecker));

    int bhvNameParamIdx = row.bhvNameParamIdx;
    if (bhvNameParamIdx < 0 || bhvNameParamIdx + paramsOffset >= sslParams.nElem)
    {
        // No behaviour-name argument supplied.
        bhvName = dsTSTRING<char>("");
        return;
    }

    int p = bhvNameParamIdx + paramsOffset;
    if (!sslParams[p].type || sslParams[p].type == dsDATA_TYPE_STORAGE<NULL_TYPE>::dataType)
    {
        // The slot is empty: publish our (auto-generated) behaviour name back into it.
        // Deviation: the decompiler's typed dsDATA(bhvName) constructor is expressed here via
        // StoreValue<dsTSTRING<char>> (its body), which is what the ctor forwards to.
        dsDATA nameData;
        nameData.type = nullptr;
        nameData.StoreValue<dsTSTRING<char>>(bhvName);
        sslParams[p].StoreValue(nameData);
    }
    else
    {
        // The slot holds a value: retrieve it as a string; a non-empty result overrides bhvName,
        // an empty one is instead overwritten with our generated name.
        dsTSTRING<char> given("");
        if (!sslParams[p].GetValue<dsTSTRING<char>>(given, DSD_CONV_RETRIEVE))
            bhvName = dsTSTRING<char>("");

        if (given.pBuffer->strLen)
            bhvName = given;
        else
            sslParams[p].SetValue<dsTSTRING<char>>(&bhvName);
    }
}
