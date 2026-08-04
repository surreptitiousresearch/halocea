/* hcex_fire_plr_event1 @0x823E47F0 — fire a named ws-engine event carrying one string parameter plus the
 * originating player index. Builds a growable dsVECTOR_PARAM_LIST with { key1: val1 (string), "player_id":
 * plr_idx }, interns and registers the event name with the global event manager, then signals it. This is
 * the string-payload sibling of hcex_fire_plr_event (which carries only player_id).
 *
 * The decompiler rendered the thiscall with a tail of phantom int args (a5..a14, an artifact of the
 * variadic-looking ws param-list templates) and reused one stack slot for the apCL cookie and successive
 * dsSTRID temporaries; the real prototype takes the event name, player index, and one key/value string pair.
 * dsVECTOR_PARAM_LIST / dsSTRID / dsEVENT_MGR are ws-engine boundaries. */

#include <string.h>   /* memset */
#include "../headers/hcex/hcex_param_list_boundary.h"

extern "C" void hcex_fire_plr_event1(const char *eventName, int plr_idx, const char *key1, const char *val1)
{
    int player_id = plr_idx;

    dsVECTOR_PARAM_LIST params;
    memset(&params, 0, 20);
    /* source-location cookie for the param list's allocator (ds_param.h:98) */
    params.storage.__cl.file = "d:\\projects\\code\\common\\incl.sys\\ds\\ds_param.h";
    params.storage.__cl.line = 98;

    dsTSTRING_flat valStr;
    valStr.pBuffer = 0;
    dsTSTRING_UnsafeInit(&valStr, val1, -1, 0);

    dsSTRID key1_key;
    dsSTRID_ctor(&key1_key, key1, false);
    dsVECTOR_PARAM_LIST_Set_tstring(&params, key1_key, &valStr);

    dsTSTRING_BUF_HEADER<char> *valBuf = valStr.pBuffer;
    if ( --valBuf->refCount == 0 )
        dlFree(valBuf);

    dsSTRID player_id_key;
    dsSTRID_ctor(&player_id_key, "player_id", false);
    dsVECTOR_PARAM_LIST_Set_int(&params, player_id_key, &player_id);

    dsEVENT_MGR *event_mgr = gEventMgr;
    dsSTRID event_key;
    dsSTRID_ctor(&event_key, eventName, false);
    int event = dsEVENT_MGR_RegisterEvent(event_mgr, event_key);
    dsEVENT_MGR_SignalEvent(gEventMgr, event, &params);

    dsVECTOR_dsPARAM_dtor(&params.storage);
}
