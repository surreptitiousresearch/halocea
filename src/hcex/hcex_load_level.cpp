/* hcex_load_level @0x823D9E60 — signal the ws-engine "OnLevelWon" event to advance to the next level,
 * passing the target level name as a "level" string parameter. Resets the bridge frame counter, lazily
 * registers the OnLevelWon event id on first call (guarded by a compiler init-once flag), builds a one-entry
 * fixed parameter list and signals the event, then destroys the entry's typed value.
 * dsEVENT_MGR / dsSTRID / dsTSTRING_flat are ws-engine boundaries. */

#include "../headers/hcex/hcex_param_list_boundary.h"

extern int hcex_call_frame;
extern unsigned int _S3_0;      /* $S3_0 — compiler init-once guard for evid_OnLevelWon */
extern int evid_OnLevelWon;

extern "C" void hcex_load_level(const char *levelName)
{
    hcex_call_frame = 0;

    if ( (_S3_0 & 1) == 0 )
    {
        dsEVENT_MGR *event_mgr = gEventMgr;
        _S3_0 |= 1u;
        dsSTRID on_level_won_key;
        dsSTRID_ctor(&on_level_won_key, "OnLevelWon", false);
        evid_OnLevelWon = dsEVENT_MGR_RegisterEvent(event_mgr, on_level_won_key);
    }

    dsSTATIC_PARAM_LIST_1 params;
    params.length = 0;
    params.list = 0;
    dsSTRID_ctor_default(&params.storage[0].id);
    params.storage[0].data.type = 0;
    params.length = 0;
    params.list = params.storage;

    dsTSTRING_flat level;
    level.pBuffer = 0;
    dsTSTRING_UnsafeInit(&level, levelName, -1, 0);
    dsSTATIC_PARAM_LIST_1_Add_tstring(&params, "level", &level);

    if ( --level.pBuffer->refCount == 0 )
        dlFree(level.pBuffer);

    dsEVENT_MGR_SignalEvent(gEventMgr, evid_OnLevelWon, &params);

    if ( params.storage[0].data.type )
        dsDATA_TYPE_Destroy(params.storage[0].data.type, &params.storage[0].data);
}
