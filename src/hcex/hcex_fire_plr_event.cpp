/* hcex_fire_plr_event @0x823E4718 — fire a named ws-engine event carrying a single "player_id" integer
 * parameter. Builds a one-entry growable parameter list (dsVECTOR_PARAM_LIST), sets player_id = plr_idx,
 * interns and registers the event name with the global event manager, then signals it with the list.
 *
 * The decompiler rendered the thiscall with a long tail of phantom int arguments (a3..a14, an artifact of
 * the variadic-looking ws param-list templates); the real prototype takes only the event name and player
 * index. dsVECTOR_PARAM_LIST / dsEVENT_MGR / dsSTRID are ws-engine boundaries. */

#include <string.h>   /* memset */
#include "../headers/hcex/hcex_param_list_boundary.h"

extern "C" void hcex_fire_plr_event(const char *eventName, int plr_idx)
{
    int player_id = plr_idx;

    dsVECTOR_PARAM_LIST params;
    memset(&params, 0, 20);
    /* source-location cookie for the param list's allocator (ds_param.h:98) */
    params.storage.__cl.file = "d:\\projects\\code\\common\\incl.sys\\ds\\ds_param.h";
    params.storage.__cl.line = 98;

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
