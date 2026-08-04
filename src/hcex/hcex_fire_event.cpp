/* hcex_fire_event @0x823CF230 — hcex bridge: fire a named ws-engine event by name. Interns the event name
 * into a dsSTRID, registers it with the global event manager (gEventMgr) to obtain a numeric event id, then
 * signals that event with no parameters. dsEVENT_MGR / dsSTRID are ws-engine boundaries.
 *
 * DEVIATION: the raw decompile carried the dsSTRID via an unassigned `dsSTRID **v2` and read `*v2`. Per the
 * disassembly the dsSTRID constructor returns `this` in r3 and RegisterEvent takes the dsSTRID by value
 * (a single `id` pointer); modeled here as the plain by-value pass. */

#include "../headers/hcex/hcex_ds_boundary.h"

extern "C" void hcex_fire_event(const char *eventName)
{
    dsEVENT_MGR *event_mgr = gEventMgr;
    dsSTRID event_name;

    dsSTRID_ctor(&event_name, eventName, false);
    int event = dsEVENT_MGR_RegisterEvent(event_mgr, event_name);
    dsEVENT_MGR_SignalEvent(gEventMgr, event, (const void *)0);
}
