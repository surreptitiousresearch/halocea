#pragma once
#include "sslSCRIPTED_FUNCTION_BASE.h"
// ssl subsystem: one node of a class event's handler chain -- a compiled function (same layout as
// sslSCRIPTED_FUNCTION) with an extra next-pointer linking sibling handlers registered for the
// same event. DB-verified layout (types_members sslEVENT_HANDLER): anonymous
// sslSCRIPTED_FUNCTION_BASE@0 (96 bytes), pNext@96 — size 100. Referenced (as sslCLASS_ELEMENT's
// TYPE_EVENT union arm) from sslCLASS.h / sslCLASS_ELEMENT.h.

struct sslEVENT_HANDLER : sslSCRIPTED_FUNCTION_BASE {
    sslEVENT_HANDLER *pNext; // 0x60 next handler chained on the same event
};
