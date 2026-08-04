#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiBRAIN_boundary.h" // _apLog
#include "../../headers/ws/ds/dsCONST_ARRAY.h"
#include "../../headers/ws/ds/REF_TYPE.h"
#include "../../headers/ws/ds/dsVECTOR_PARAM_LIST.h"

// aiBRAIN::implTestParamFromAI() @ 0x8325B858
// ?implTestParamFromAI@aiBRAIN@@UAAABV?$REF_TYPE@VdsVECTOR_PARAM_LIST@@@ds_data@@XZ (virtual)
//
// Native worker for the "TestParamFromAI" SSL function: return the front param-list reference of the
// translation-unit-local AI-test task queue (the same 10-slot array written by implTestParam2AI). When
// the queue is empty, log an error under this unit's instance name and return a lazily-constructed,
// process-lifetime empty REF_TYPE instead.
//
// The front read is `tasks[0]`; its dsCONST_ARRAY::operator[] carries the IsValidIdx STRONG_ASSERT
// seen inline at 0x8325B8DC (ds_const_list.h:166).

// File-static task queue (mangled ?tasks@@3V?$dsCONST_ARRAY@V?$REF_TYPE@VdsVECTOR_PARAM_LIST@@@ds_data@@$09@@A).
static dsCONST_ARRAY<ds_data::REF_TYPE<dsVECTOR_PARAM_LIST>, 10> tasks;

const ds_data::REF_TYPE<dsVECTOR_PARAM_LIST> &aiBRAIN::implTestParamFromAI()
{
    if (tasks.nEntry <= 0)
    {
        _apLog("~AI,Error~'%s': attempt to take a task from empty queue", names.nameInst);
        static const ds_data::REF_TYPE<dsVECTOR_PARAM_LIST> empty; // lazy, atexit-destroyed
        return empty;
    }
    return tasks[0];
}
