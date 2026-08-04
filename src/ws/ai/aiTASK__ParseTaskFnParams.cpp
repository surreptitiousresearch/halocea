// aiTASK::ParseTaskFnParams(int, aiTASK_DATA_FN*, dsVECTOR<dsDATA,8>&) @ 0x832627A0
// ?ParseTaskFnParams@aiTASK@@IAA_NHPAVaiTASK_DATA_FN@@AAV?$dsVECTOR@VdsDATA@@$07@@@Z
#include "../../headers/ws/ai/aiTASK.h"
#include "../../headers/ws/ai/aiTASK_DATA_FN.h"
#include "../../headers/ws/ds/dsDATA.h"
#include "../../headers/ws/ds/dsDATA_TYPE.h"
#include "../../headers/ws/ds/ds_boundary.h" // dsDATA_TYPE_STORAGE<>::dataType
#include "../../headers/ws/ds/NULL_TYPE.h"
#include "../../headers/ws/ds/dsVECTOR.h"

// Protected: forward the call's trailing scripted params (indices [paramsOffset .. last-meaningful])
// to `task` as after-fire fn parameters. Trailing null / NULL_TYPE params are trimmed first.
bool aiTASK::ParseTaskFnParams(int paramsOffset, aiTASK_DATA_FN *task, dsVECTOR<dsDATA, 8> &sslParams)
{
    int lastMeaningful = sslParams.nElem - 1;
    if (lastMeaningful < paramsOffset)
        return true;

    // Scan down from the top, trimming trailing null / NULL_TYPE-valued params.
    while (!sslParams[lastMeaningful].type ||
           sslParams[lastMeaningful].type == dsDATA_TYPE_STORAGE<NULL_TYPE>::dataType)
    {
        if (--lastMeaningful < paramsOffset)
            return true;
    }

    for (int i = paramsOffset; i <= lastMeaningful; ++i)
        task->AddFnParam(&sslParams[i]);

    return true;
}
