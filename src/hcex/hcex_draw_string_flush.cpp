/* hcex_draw_string_flush @0x823E4B00 — if any text has been buffered into hcexDrawString (by
 * hcex_draw_string), package it as a one-entry "text" parameter list and fire the "asHaloMsg" ws-engine
 * event with it, then clear the buffer. dsVECTOR_PARAM_LIST / dsSTRID / dsEVENT_MGR are ws-engine
 * boundaries.
 *
 * DEVIATION: the raw decompile carried the dsSTRID keys via unassigned `dsSTRID **v0`/`v2` locals and
 * read `*v0`/`*v2`. Per the disassembly (and the same pattern already documented in hcex_fire_event.c)
 * the dsSTRID constructor returns `this` in r3; modeled here as the plain by-value key it constructs. */

#include <string.h>   /* memset */
#include "../headers/hcex/hcex_param_list_boundary.h"

extern dsTSTRING_flat hcexDrawString;

void hcex_draw_string_flush(void)
{
    if ( !hcexDrawString.pBuffer->strLen )
        return;

    dsVECTOR_PARAM_LIST params;
    memset(&params, 0, 20);
    /* source-location cookie for the param list's allocator (ds_param.h:98) */
    params.storage.__cl.file = "d:\\projects\\code\\common\\incl.sys\\ds\\ds_param.h";
    params.storage.__cl.line = 98;

    dsSTRID text_key;
    dsSTRID_ctor(&text_key, "text", false);
    dsVECTOR_PARAM_LIST_Set_tstring(&params, text_key, &hcexDrawString);

    dsEVENT_MGR *event_mgr = gEventMgr;
    dsSTRID event_key;
    dsSTRID_ctor(&event_key, "asHaloMsg", false);
    int event = dsEVENT_MGR_RegisterEvent(event_mgr, event_key);
    dsEVENT_MGR_SignalEvent(gEventMgr, event, &params);

    dsTSTRING_Clear(&hcexDrawString);
    dsVECTOR_dsPARAM_dtor(&params.storage);
}
