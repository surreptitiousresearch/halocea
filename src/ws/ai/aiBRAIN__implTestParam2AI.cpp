#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ds/dsCONST_ARRAY.h"
#include "../../headers/ws/ds/REF_TYPE.h"
#include "../../headers/ws/ds/dsVECTOR_PARAM_LIST.h"

// aiBRAIN::implTestParam2AI(ds_data::REF_TYPE<dsVECTOR_PARAM_LIST>) @ 0x8325B808
// ?implTestParam2AI@aiBRAIN@@UAAXV?$REF_TYPE@VdsVECTOR_PARAM_LIST@@@ds_data@@@Z (virtual)
//
// Native worker for the "TestParam2AI" SSL callback: append the param-list reference `params` onto the
// translation-unit-local AI-test task queue (a fixed-capacity 10-slot array). The trailing Back() read
// is present in the original (its result is discarded here). `params` releases on return.

// File-static task queue (mangled ?tasks@@3V?$dsCONST_ARRAY@V?$REF_TYPE@VdsVECTOR_PARAM_LIST@@@ds_data@@$09@@A).
static dsCONST_ARRAY<ds_data::REF_TYPE<dsVECTOR_PARAM_LIST>, 10> tasks;

void aiBRAIN::implTestParam2AI(ds_data::REF_TYPE<dsVECTOR_PARAM_LIST> params)
{
    tasks.Add(params);
    tasks.Back();
    // params destructor self-releases at scope end.
}
