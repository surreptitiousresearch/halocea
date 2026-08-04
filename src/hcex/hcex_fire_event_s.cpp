/* hcex_fire_event_s @0x823D9D38 — string-valued sibling of hcex_fire_event_i: fire a named ws-engine event
 * carrying a single string parameter (paramName = paramValue). Builds a one-entry fixed parameter list
 * (dsSTATIC_PARAM_LIST<1>) in place with the value stored as a dsTSTRING_flat, registers/signals the event, then
 * destroys the entry's typed value.
 *
 * NOTE (verified against disasm at 0x823D9E28, `li r5,0`): SignalEvent is called with a NULL param list —
 * the event is signalled with no parameters even though the entry is built. Reproduced faithfully.
 * dsEVENT_MGR / dsSTRID / dsDATA / dsTSTRING_flat are ws-engine boundaries. */

#include "../headers/hcex/hcex_param_list_boundary.h"

extern "C" void hcex_fire_event_s(const char *eventName, const char *paramName, const char *paramValue)
{
    dsSTATIC_PARAM_LIST_1 params;
    dsSTRID_ctor_default(&params.storage[0].id);
    params.storage[0].data.type = 0;
    params.length = 0;
    params.list = params.storage;

    dsTSTRING_flat value;
    value.pBuffer = 0;
    dsTSTRING_UnsafeInit(&value, paramValue, -1, 0);

    dsSTRID param_key;
    dsSTRID_ctor(&param_key, paramName, false);
    dsPARAM *entry = &params.storage[params.length];
    entry->id = param_key;
    dsDATA_SetValue_tstring(&entry->data, &value);
    ++params.length;

    if ( --value.pBuffer->refCount == 0 )
        dlFree(value.pBuffer);

    dsEVENT_MGR *event_mgr = gEventMgr;
    dsSTRID event_key;
    dsSTRID_ctor(&event_key, eventName, false);
    int event = dsEVENT_MGR_RegisterEvent(event_mgr, event_key);
    dsEVENT_MGR_SignalEvent(gEventMgr, event, 0);   /* NULL param list — see NOTE */

    if ( params.storage[0].data.type )
        dsDATA_TYPE_Destroy(params.storage[0].data.type, &params.storage[0].data);
}
