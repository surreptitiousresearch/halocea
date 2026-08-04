/* hcex_fire_event_i @0x823D5300 — fire a named ws-engine event carrying a single integer parameter
 * (paramName = paramValue). Builds a one-entry fixed parameter list (dsSTATIC_PARAM_LIST<1>) in place,
 * interns and registers the event name, signals it, then destroys any typed value the entry holds.
 *
 * The decompiler rendered a long tail of phantom int arguments (an artifact of the variadic-looking ws
 * param-list templates); the real prototype is (eventName, paramName, paramValue).
 *
 * NOTE (verified against disasm at 0x823D53BC, `li r5,0`): SignalEvent is called with a NULL param list —
 * the event is signalled with no parameters even though the entry is built. Reproduced faithfully.
 * dsEVENT_MGR / dsSTRID / dsDATA are ws-engine boundaries. */

#include "../headers/hcex/hcex_param_list_boundary.h"

extern "C" void hcex_fire_event_i(const char *eventName, const char *paramName, int paramValue)
{
    int value = paramValue;

    dsSTATIC_PARAM_LIST_1 params;
    dsSTRID_ctor_default(&params.storage[0].id);
    params.storage[0].data.type = 0;
    params.length = 0;
    params.list = params.storage;

    dsSTRID param_key;
    dsSTRID_ctor(&param_key, paramName, false);
    dsPARAM *entry = &params.storage[params.length];
    entry->id = param_key;
    dsDATA_SetValue_int(&entry->data, &value);
    ++params.length;

    dsEVENT_MGR *event_mgr = gEventMgr;
    dsSTRID event_key;
    dsSTRID_ctor(&event_key, eventName, false);
    int event = dsEVENT_MGR_RegisterEvent(event_mgr, event_key);
    dsEVENT_MGR_SignalEvent(gEventMgr, event, 0);   /* NULL param list — see NOTE */

    if ( params.storage[0].data.type )
        dsDATA_TYPE_Destroy(params.storage[0].data.type, &params.storage[0].data);
}
