#pragma once
#include "dsFUNC_PTR_BASE.h"
#include "dsDATA.h"
#include "dsSTRID.h"

// ws-engine ds — a scripted-callable bound to an SSL object + function name. DB-verified layout
// (types_members dsFUNC_PTR_SSL): base dsFUNC_PTR_BASE@0, obj@4 (dsDATA), funcName@12 (dsSTRID)
// — size 16.
struct dsFUNC_PTR_SSL : dsFUNC_PTR_BASE {
    dsDATA  obj;      // 0x04 boxed target object reference
    dsSTRID funcName; // 0x0C scripted function name id

    // 0x825D4688 (??0dsFUNC_PTR_SSL@@QAA@VdsDATA@@VdsSTRID@@@Z) — construct from a boxed object and a
    // function-name id.
    dsFUNC_PTR_SSL(dsDATA obj, dsSTRID funcName);
};
